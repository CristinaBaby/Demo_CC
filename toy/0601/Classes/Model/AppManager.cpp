//
//  AppManager.cpp
//  PaintDress
//
//  Created by maxiang on 6/16/15.
//
//

#include "AppManager.h"
#include "../Module/STSystemFunction.h"
#include "../Layer/AdsLoadingLayer.h"
#include "AppConfigs.h"

static AppManager *s_SingletonAppManager = nullptr;

AppManager* AppManager::getInstance()
{
    if (!s_SingletonAppManager) {
        s_SingletonAppManager = new (std::nothrow)(AppManager);
        
        bool isOpenSound = UserDefault::getInstance()->getBoolForKey(UserDefaultKey_IsOpenSound, true);
        s_SingletonAppManager->setOpenSound(isOpenSound);
        s_SingletonAppManager->_hasRemoveAds = UserDefault::getInstance()->getBoolForKey(UserDefaultKey_HasRemoveAds, false);
        
        //coin
        int coin = UserDefault::getInstance()->getIntegerForKey(UserDefaultKey_Coins, DEFAULT_COIN);
        s_SingletonAppManager->_coin = coin;//coin;
        CCASSERT(s_SingletonAppManager, "ABORT: create AppManager failed!");
    }
    
    return s_SingletonAppManager;
}

AppManager::AppManager():
_coloringEffectId(0)
{}

AppManager::~AppManager()
{
    s_SingletonAppManager = nullptr;
}

#pragma mark- Sound

void AppManager::setOpenSound(bool isOpen)
{
    _isOpenSound = isOpen;
    if (isOpen)
    {
        playStartBgm();
    }
    else
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
    
    UserDefault::getInstance()->setBoolForKey(UserDefaultKey_IsOpenSound, isOpen);
    UserDefault::getInstance()->flush();
}

void AppManager::playStartBgm()
{
    if (_isOpenSound)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(SOUND_BGM_START, true);
    }
}

void AppManager::playDressBgm()
{
    if (_isOpenSound)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(SOUND_BGM_DRESS, true);
    }
}

void AppManager::playColorBgm()
{
    if (_isOpenSound)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(SOUND_BGM_COLOR, true);
    }
}

void AppManager::playSelectBgm()
{
    if (_isOpenSound)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(SOUND_BGM_SELECT, true);
    }
}

void AppManager::playPlayEffect()
{
    if (_isOpenSound)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_PLAY);
    }
}

void AppManager::playButtonEffect()
{
    if (_isOpenSound)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_BUTTON);
    }
}

void AppManager::playDressEffect()
{
    if (_isOpenSound)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_DRESS);
    }
}

void AppManager::playPopupEffect()
{
    if (_isOpenSound)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_POPUP);
    }
}

void AppManager::playColoringEffect()
{
    if (_isOpenSound)
    {
        _coloringEffectId = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_COLORING, true);
    }
}

void AppManager::pauseColoringEffect()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(_coloringEffectId);
}

void AppManager::playCameraEffect()
{
    if (_isOpenSound)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_CAMERA);
    }
}

#pragma mark- Rate us logic

void AppManager::rateUsLogic()
{
    STSystemFunction function = STSystemFunction();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    function.rating(APP_ID, "If you like this app, please rate our game!");
#else
    function.rating();
#endif
}


#pragma mark- Coins

int AppManager::addCoin(const int coin)
{
    _coin += coin;
    UserDefault::getInstance()->setIntegerForKey(UserDefaultKey_Coins, _coin);
    UserDefault::getInstance()->flush();
    return _coin;
}

bool AppManager::purchaseRes(Res& res)
{
    int price = res.price;
    if (_coin < price) {
        return false;
    }
    
    _coin -= price;
    
    //purchase user default
    std::string key1 = UserDefaultKey_HasPurchasedItem + res.resName;
    //purchase new res
    std::string key2 = UserDefaultKey_IsNewPurchaseRes + res.resName;
    res.isNewPurchasedRes = true;
    
    UserDefault::getInstance()->setBoolForKey(key1.c_str(), true);
    UserDefault::getInstance()->setBoolForKey(key2.c_str(), true);
    UserDefault::getInstance()->setIntegerForKey(UserDefaultKey_Coins, _coin);
    UserDefault::getInstance()->flush();
    
    return true;
}

void AppManager::removeAds()
{
    _stAds.removeAds();
    _hasRemoveAds = true;
    
    UserDefault::getInstance()->setBoolForKey(UserDefaultKey_HasRemoveAds, true);
    UserDefault::getInstance()->flush();
}

#pragma mark- Ads methods

void AppManager::requestBannerAd()
{
    _stAds.requestAds();
}

void AppManager::requestFullScreenAd()
{
#if (!DEBUG_REMOVE_FULLSCREEN_ADS)
    if (_hasRemoveAds) {
        return;
    }
    
    AdLoadingLayerBase::showLoading<AdsLoadingLayer>(false);
#endif
}

void AppManager::requestCrossPromoAd()
{
    if (_hasRemoveAds) {
        return;
    }
    
    AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true);
}

void AppManager::showBannerAd()
{
    if (_hasRemoveAds) {
        return;
    }
    _stAds.requestAds();
}

void AppManager::hideBannerAd()
{
    _stAds.removeAds();
}

