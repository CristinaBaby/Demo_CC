//
//  AppManager.cpp
//  ParfaitMaker
//
//  Created by maxiang on 7/30/15.
//
//

#include "AppManager.h"
#include "AdsLoadingLayer.h"
#include "AdsManager.h"

USING_NS_CC;

static AppManager *s_SingletonAppManager = nullptr;

AppManager* AppManager::getInstance()
{
    if (!s_SingletonAppManager) {
        s_SingletonAppManager = new (std::nothrow)(AppManager);
        
        s_SingletonAppManager->_isOpenSound = true;

        //if user remove all unlock items, remove ads
        s_SingletonAppManager->_hasRemoveAds = false;
        
        std::string userDefaultKey = std::string(UserDefaultKey_HasPurchasedItem_) + std::string(IAP_UNLOCK_All);
        bool hasUnlockAll = UserDefault::getInstance()->getBoolForKey(userDefaultKey.c_str(), false);
        s_SingletonAppManager->setHasUnlockAll(hasUnlockAll);
    }
    
    return s_SingletonAppManager;
}

AppManager::AppManager():
_isShowBannerAds(false)
{}

AppManager::~AppManager()
{
    s_SingletonAppManager = nullptr;
}

#pragma mark- Inner functions

bool AppManager::isDigit(const std::string& string)
{
    if (string == "")
        return false;
    
    for (auto iterator : string)
    {
        if (!isdigit(iterator))
            return false;
    }

    return true;
}

const std::string AppManager::getFormatTimeString(const int time) const
{
    if (time > 99 * 3600)
        return "∞";
    
    int h = time / 3600;
    int m = time % 3600 / 60;
    int s = time % 3600 % 60;
    
    std::string hstring = h < 10 ? "0"+std::to_string(h) : std::to_string(h);
    std::string mstring = m < 10 ? "0"+std::to_string(m) : std::to_string(m);
    std::string sstring = s < 10 ? "0"+std::to_string(s) : std::to_string(s);
    
    std::string timeString = hstring + ":" + mstring + ":" + sstring;
    return timeString;
}

const std::string AppManager::getFormatDateString() const
{
    struct timeval nowTimeval;
    gettimeofday(&nowTimeval, NULL);
    struct tm *tm;
    time_t time_sec;
    time_sec = nowTimeval.tv_sec;
    tm = localtime(&time_sec);
    int year = tm->tm_year;
    int month = tm->tm_mon;
    int day = tm->tm_mday;
    
    std::string formatString = std::to_string(year) + std::to_string(month) + std::to_string(day);
    
    return formatString;
}


#pragma mark- Sound

void AppManager::playEffect(const std::string& soundFile)
{
    if (_isOpenSound)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(soundFile.c_str());
    }
}

void AppManager::playBgm(const std::string& soundFile)
{
    if (_isOpenSound)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(soundFile.c_str(), true);
    }
}

void AppManager::playLoopEffect(const std::string& soundFile)
{
    if (!_isOpenSound)
        return;
    
    auto iterator = _loopEffectSoundMap.find(soundFile);
    if (iterator == _loopEffectSoundMap.end())
    {
        int soundId = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(soundFile.c_str(), true);
        _loopEffectSoundMap.insert(std::make_pair(soundFile, soundId));
    }
    else
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeEffect(iterator->second);
    }
}

void AppManager::pauseLoopEffect(const std::string& soundFile)
{
    auto iterator = _loopEffectSoundMap.find(soundFile);
    
    if (iterator != _loopEffectSoundMap.end())
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->pauseEffect(iterator->second);
    }
}

void AppManager::stopLoopEffect(const std::string& soundFile)
{
    auto iterator = _loopEffectSoundMap.find(soundFile);
    
    if (iterator != _loopEffectSoundMap.end())
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(iterator->second);
        _loopEffectSoundMap.erase(iterator);
    }
}

#pragma mark- Ads methods

void AppManager::requestCrossPromoAd()
{
    if (!_hasRemoveAds)
    {
        bool isShowBannerAds = _isShowBannerAds;
        AdLoadingLayerBase::loadingDoneCallback = [=](){
            
            log("MX: is show banner ads?%d",isShowBannerAds);
            this->setBannerAdVisible(isShowBannerAds);
        };
        
        AdsManager::getInstance()->setVisiable(kTypeBannerAds, false);
        AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true);
    }
}

void AppManager::requestFullScreenAd()
{
    if (!_hasRemoveAds)
    {
        bool isShowBannerAds = _isShowBannerAds;
        AdLoadingLayerBase::loadingDoneCallback = [=](){
            
            log("MX: is show banner ads?%d",isShowBannerAds);
            this->setBannerAdVisible(isShowBannerAds);
        };
        
        AdsManager::getInstance()->setVisiable(kTypeBannerAds, false);
        AdLoadingLayerBase::showLoading<AdsLoadingLayer>(false);
    }
}

void AppManager::requestBannerAd()
{
    if (!_hasRemoveAds)
    {
        AdsManager::getInstance()->showAds(kTypeBannerAds);
    }
}

void AppManager::setBannerAdVisible(bool visible)
{
    if (!_hasRemoveAds)
    {
        _isShowBannerAds = visible;
        log("MX: set shown banner ads?%d", _isShowBannerAds);
        
        AdsManager::getInstance()->setVisiable(kTypeBannerAds, visible);
    }
}


