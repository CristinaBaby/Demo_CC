//
//  RewardManager.cpp
//  BHUG1062
//
//

#include "RewardManager.h"
#include "cocos2d.h"
#include "StringHelper.h"
#include "SSCInternalLibManager.h"
#include "3rdparty_depends/crossplatform/headers/analytics/Analytics.h"
#include <sys/time.h>
#include "audio/include/SimpleAudioEngine.h"

using namespace cocos2d;

#define kRewardLastLockAllTimeKey "RewardLastLockAllTimeKey"

static RewardManager* _s_RewardManager = nullptr;
bool RewardManager::s_showFullAds = true;


RewardManager* RewardManager::getInstance()
{
    if (!_s_RewardManager) {
        _s_RewardManager = new RewardManager();
    }
    return _s_RewardManager;
}

RewardManager::RewardManager()
{
    auto strLastLockAllTime = UserDefault::getInstance()->getStringForKey(kRewardLastLockAllTimeKey, "");
    if(strLastLockAllTime.length() == 0)
    {
        _lastLockAllSecondsAt0ClockOfThatDay = getTodaySecondAt0Clock();
        UserDefault::getInstance()->setStringForKey(kRewardLastLockAllTimeKey, to_string(_lastLockAllSecondsAt0ClockOfThatDay));
        return;
    }
    _lastLockAllSecondsAt0ClockOfThatDay = atol(strLastLockAllTime.c_str());
    Director::getInstance()->getScheduler()->schedule(schedule_selector(RewardManager::reLockTimeCheckSchedule), this, 10.0f, kRepeatForever, 10.0f, false);
}

void RewardManager::loadConfig(string csvPath)
{
    SSCCSVParse* parse = SSCCSVParse::create(csvPath.c_str());
    if (!parse)
    {
        CCLOG("warning:load config failed:%s", csvPath.c_str());
        return;
    }
    auto rows = parse->getRows();
    for (int i = 1; i < rows; i++) {
        string iapId = parse->getDatas(i, REWARD_IAPID);
        string moduleName = parse->getDatas(i, REWARD_ModuleName);
        string keyInModule = parse->getDatas(i, REWARD_KeyInModule);
        string sIndex = parse->getDatas(i, REWARD_Indexs);
        
        if( iapId != "")
        {
            RewardInfoItem item = {iapId, moduleName, keyInModule, -1};
            strArray arrComma;
            StringHelper::splitWithForm(sIndex, &arrComma);
            if(arrComma.size() == 0)
            {
                mapRewardItems[item.getKey()] = item;
                return;
            }
            for(auto str: arrComma)
            {
                strArray arrLine;
                StringHelper::split(str, "-", &arrLine);
                if(arrLine.size() <= 1)
                {
                    item.index = atoi(str.c_str());
                    mapRewardItems[item.getKey()] = item;
                }
                else
                {
                    int start = atoi(arrLine[0].c_str()), end = atoi(arrLine[1].c_str());
                    for(int i = start; i <= end; ++i)
                    {
                        item.index = i;
                        mapRewardItems[item.getKey()] = item;
                    }
                }
            }
        }
    }
}

string RewardManager::getItemKey(string moduleName, string keyInModule, const int inx)
{
    return moduleName + keyInModule + to_string(inx);
}

RewardInfoItem RewardManager::getRewardInfoItem(string moduleName, string keyInModule, const int inx)
{
    auto key = getItemKey(moduleName, keyInModule, inx);
    if(mapRewardItems.find(key) != mapRewardItems.end())
        return mapRewardItems[key];
    return RewardInfoItem();
}

void RewardManager::showRewardAds(const RewardInfoItem &item)
{
    if(!isLocked(item))
        return;
    auto sys = SSCInternalLibManager::getInstance();
    if (!sys->checkNetworkAvailable()) {
        Analytics::getInstance()->sendEvent("RequestVideoFailed-NIC", "No internet connected!");
        if(showRewardFalseCall)
        {
            showRewardFalseCall();
        }
        return;
    }
    
    _rewardRequestResult = AdsManager::getInstance()->showAds(ADS_TYPE::kTypeRewardedAds);
    log("======kTypeRewardedAds %d====",_rewardRequestResult);
    if(!_rewardRequestResult)
    {
//        AdsManager::getInstance()->preloadAds(ADS_TYPE::kTypeRewardedAds);
        
        
        if(showRewardLoadingCall)
        {
            showRewardLoadingCall();
        }
        bool result = showRewardFailedHandleAndroid();
        log("======kTypeRewardedAdsFailed %d====",result);
        if(!result)
            return;
        _waitingUnLockItemInfo = item;
        registerAdsEvents();
        this->onAdsRewarded("", 0, false);
        return;
        
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        showRewardFailedHandleIos();
//        return;
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        if(showRewardLoadingCall)
//        {
//            showRewardLoadingCall();
//        }
//        bool result = showRewardFailedHandleAndroid();
//        log("======kTypeRewardedAdsFailed %d====",result);
//        if(!result)
//            return;
//        _waitingUnLockItemInfo = item;
//        registerAdsEvents();
//        this->onAdsRewarded("", 0, false);
//        return;
//#endif
    }
    else
    {
        //如果reward请求成功，开启超时检测，超过5秒没有显示广告，就移除ads loading
        Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(RewardManager::removeLoadingSchedule, this), this, 5, 0, 0, false, "removeLoadingSchedule");
        
        //reward广告虽然请求成功了，但是还没有显示出来
        _rewardAdsExpanded = false;
    }
    
    s_showFullAds = false;
    
    log("=========onAdsCollapsed %d 1111=========",s_showFullAds);
    
    if(showRewardLoadingCall)
    {
        showRewardLoadingCall();
    }
    _waitingUnLockItemInfo = item;
    registerAdsEvents();
}

void RewardManager::showRewardAds(string moduleName, string keyInModule, const int inx)
{
    auto key = getItemKey(moduleName, keyInModule, inx);
    showRewardAds(key);
}

void RewardManager::showRewardAds(const string key)
{
    if(mapRewardItems.find(key) == mapRewardItems.end())
        return;
    showRewardAds(mapRewardItems[key]);
}

void RewardManager::showRewardFailedHandleIos()
{
    if(showRewardFalseCall)
    {
        showRewardFalseCall();
    }
}

bool RewardManager::showRewardFailedHandleAndroid()
{
    bool result = AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
    if(!result)
    {
        if(removeRewardLoadingCall)
        {
            removeRewardLoadingCall();
        }
        if(showRewardFalseCall)
        {
            showRewardFalseCall();
        }
    }
    return result;
}

void RewardManager::removeLoadingSchedule(float dt)
{
    if(removeRewardLoadingCall)
    {
        removeRewardLoadingCall();
    }
    
    //如果在移除loading的时候，reward广告返回结果是true，但是没有成功展开，说明reward超时，此时进入
    //reward请求失败逻辑
    if (_rewardRequestResult && !_rewardAdsExpanded)
    {
        unRegisterAdsEvents();
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        showRewardFailedHandleIos();
        return;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        bool result = showRewardFailedHandleAndroid();
        log("======kTypeRewardedAdsFailed %d====",result);
        if(!result)
            return;
        this->onAdsRewarded("", 0, false);
#endif
    }
}

void RewardManager::onAdsExpanded(ADS_TYPE adType)
{
    Director::getInstance()->getScheduler()->unschedule("removeLoadingSchedule", this);
    
    if (ADS_TYPE::kTypeRewardedAds == adType)
    {
        //reward广告成功显示
        _rewardAdsExpanded = true;
        
        CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
}

void RewardManager::onAdsCollapsed(ADS_TYPE adType)
{
    log("onRewardAdsCollapsed====== %d=====",adType);
    Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(RewardManager::removeLoadingSchedule, this), this, 5, 0, 0, false, "removeLoadingSchedule");
}

void RewardManager::onAdsRewarded(std::string, int, bool skip)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    Director::getInstance()->getScheduler()->unschedule("removeLoadingSchedule", this);
    
    unRegisterAdsEvents();
    if(removeRewardLoadingCall)
    {
        removeRewardLoadingCall();
    }
    if(skip)
    {
        return;
    }
    unLocked(_waitingUnLockItemInfo);
}

void RewardManager::onAdsLoadFailed(std::string error, ADS_TYPE adType)
{
    Director::getInstance()->getScheduler()->unschedule("removeLoadingSchedule", this);
    
    if (adType == ADS_TYPE::kTypeRewardedAds)
    {
        this->onAdsRewarded("", 0, true);
    }
}

bool RewardManager::isLocked(string key)
{
    if(mapRewardItems.find(key) == mapRewardItems.end())
        return false;
    return UserDefault::getInstance()->getBoolForKey(key.c_str(), true);
}


bool RewardManager::isLocked(const RewardInfoItem &item)
{
    return isLocked(item.getKey());
}


void RewardManager::unLocked(const RewardInfoItem &item)
{
    return unLocked(item.getKey());
}

void RewardManager::unLocked(string key)
{
    if(mapRewardItems.find(key) == mapRewardItems.end())
        return;
    UserDefault::getInstance()->setBoolForKey(key.c_str(), false);
    UserDefault::getInstance()->flush();
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(kRewardItemStatusChangedNotify);
}

void RewardManager::lockAll()
{
    for(auto itor: mapRewardItems)
    {
        UserDefault::getInstance()->setBoolForKey(itor.second.getKey().c_str(), true);
    }
    _lastLockAllSecondsAt0ClockOfThatDay = getTodaySecondAt0Clock();
    UserDefault::getInstance()->setStringForKey(kRewardLastLockAllTimeKey, to_string(_lastLockAllSecondsAt0ClockOfThatDay));
    UserDefault::getInstance()->flush();
}

bool RewardManager::isItTimeToLockAll()
{
    time_t now = time(nullptr);
    tm* localTm = localtime(&now);;
    time_t nowLocal;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    nowLocal = timegm(localTm);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    nowLocal = mktime(localTm);
#endif
    
    long diffSecond = nowLocal - _lastLockAllSecondsAt0ClockOfThatDay;
    //    __CCLOGWITHFUNCTION("diffSecond:%ld", diffSecond);
    return diffSecond >= secondsOfOneDay;
}

long RewardManager::getTodaySecondAt0Clock()
{
    time_t now = time(nullptr);
    tm* localTm = localtime(&now);;
    time_t nowLocal;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    nowLocal = timegm(localTm);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    nowLocal = mktime(localTm);
#endif
    __CCLOGWITHFUNCTION("%ld, %ld, %ld", nowLocal, nowLocal - nowLocal % secondsOfOneDay, nowLocal % secondsOfOneDay);
    return nowLocal - nowLocal % secondsOfOneDay;
}

void RewardManager::reLockTimeCheckSchedule(float dt)
{
    if(isItTimeToLockAll())
    {
        __CCLOGWITHFUNCTION("time to re lock all");
        lockAll();
    }
}

void RewardManager::registerAdsEvents()
{
    AdsManager::getInstance()->delegate = this;
}

void RewardManager::unRegisterAdsEvents()
{
    if(AdsManager::getInstance()->delegate == this)
        AdsManager::getInstance()->delegate = nullptr;
}

