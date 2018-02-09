//
//  RewardManager.cpp
//  BHUG1062
//
//

#include "RewardManager.h"
#include "CSVParse.h"
#include "cocos2d.h"
#include "StringHelp.h"
#include "SSCInternalLibManager.h"
#include "Analytics.h"
#include <sys/time.h>
#include "audio/include/SimpleAudioEngine.h"
#include "ConfigManager.h"
#include "SSCIAPManager.h"

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


void RewardManager::showRewardAds(const string key, const int inx)
{
    if (!ConfigManager::getInstance()->getVideoUnLocked(key, inx)) {
        //        return;
//        STSystemFunction sys;
        if (!SSCInternalLibManager::getInstance()->checkNetworkAvailable()) {
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
//            AdsManager::getInstance()->preloadAds(ADS_TYPE::kTypeRewardedAds);
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            showRewardFailedHandleIos();
            return;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            bool result = showRewardFailedHandleAndroid();
            log("======kTypeRewardedAdsFailed %d====",result);
            if(!result)
                return;
            _waitingUnLockItemInfo = {key, inx};
            this->onAdsRewarded("", 0, false);
#endif
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
        
        _waitingUnLockItemInfo = {key, inx};
        registerAdsEvents();
    }
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

bool RewardManager::isLocked(const RewardInfoItem &item)
{
    return !ConfigManager::getInstance()->getVideoUnLocked(item.keyInModule, item.index);
}


void RewardManager::unLocked(const RewardInfoItem &item)
{
    ConfigManager::getInstance()->setVidioUnlocked(item.keyInModule, item.index);
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(kRewardItemStatusChangedNotify);
}

//bool RewardManager::isLocked(string key)
//{
//    return IAPManager::getInstance()->getItemIsBought(key);
//}
//
//
//void RewardManager::unLocked(string key)
//{
//    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(kRewardItemStatusChangedNotify);
//}
//
void RewardManager::lockAll()
{
    _lastLockAllSecondsAt0ClockOfThatDay = getTodaySecondAt0Clock();
    //    UserDefault::getInstance()->setStringForKey(kRewardLastLockAllTimeKey, to_string(_lastLockAllSecondsAt0ClockOfThatDay));
    //    UserDefault::getInstance()->flush();
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

