//
//  CBAppManager.cpp
//  ColorBook
//
//  Created by maxiang on 4/21/15.
//
//

#include "CBAppManager.h"
#include "../Module/STSystemFunction.h"
#include "../Scene/CBPackScene.h"
#include "../Scene/CBPictureScene.h"
#include "../Layer/AdsLoadingLayer.h"
#include "../Layer/CBQuitGameLayer.h"


USING_NS_CC;
CB_BEGIN_NAMESPACE

static AppManager *s_SingletonAppManager = nullptr;

AppManager* AppManager::getInstance()
{
    if (!s_SingletonAppManager) {
        s_SingletonAppManager = new (std::nothrow)(AppManager);
        
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(SOUND_BGM, true);

        bool isOpenBgm = UserDefault::getInstance()->getBoolForKey(UserDefaultKey_isOpenBgm, true);
        bool isOpenSfx = UserDefault::getInstance()->getBoolForKey(UserDefaultKey_isOpenSfx, true);
        s_SingletonAppManager->setOpenBgm(isOpenBgm);
        s_SingletonAppManager->setOpenEffect(isOpenSfx);
        
        /* set ads delegate */
//        s_SingletonAppManager->_stAds.setAdsDelegate(s_SingletonAppManager);
        CCASSERT(s_SingletonAppManager, "ABORT: create AppManager failed!");
    }
    
    return s_SingletonAppManager;
}

AppManager::AppManager():_saveTimes(0)
{}

AppManager::~AppManager()
{
    s_SingletonAppManager = nullptr;
}

#pragma mark- Sound

void AppManager::setOpenBgm(bool isOpen)
{
    _isOpenBgm = isOpen;
    playBgm();
}

void AppManager::setOpenEffect(bool isOpen)
{
    _isOpenEffect = isOpen;
}

void AppManager::playBgm()
{
    if (_isOpenBgm)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }
    else
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
}

void AppManager::playButtonEffect()
{
    if (_isOpenEffect)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_BUTTON);
    }
}

void AppManager::playPensChangeEffect()
{
    if (_isOpenEffect)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_CHANGE_PENS);
    }
}

void AppManager::playFinishColorEffect()
{
    if (_isOpenEffect)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_COLORED);
    }
}

#pragma mark- Rate us logic

void AppManager::rateUsLogic()
{
    _saveTimes++;
    if (_saveTimes % 3 == 0)
    {
        STSystemFunction function = STSystemFunction();
        bool israte =UserDefault::getInstance()->getBoolForKey("isRate", false);
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        function.rating(APP_ID, "If you like this app, please rate our game!");
        
#else
        
        if(!israte)
        {
            auto quitLayer = QuitGameLayer::create();
            quitLayer->setText("If you like this\n app, please rate\n our game!");
            quitLayer->setYesActionCallback([=](){
                STSystemFunction sys;
                sys.rating();
                UserDefault::getInstance()->setBoolForKey("isRate", true);
            });
            Director::getInstance()->getRunningScene()->addChild(quitLayer);
        }
        
#endif
    }
}

#pragma mark- Ads methods

void AppManager::requestBannerAd()
{
    AdsManager::getInstance()->showAds(kTypeBannerAds);
}

void AppManager::requestFullScreenAd()
{
 AdLoadingLayerBase::showLoading<AdsLoadingLayer>(false);
}

void AppManager::showBannerAd()
{
    AdsManager::getInstance()->showAds(kTypeBannerAds);
}

void AppManager::hideBannerAd()
{
      AdsManager::getInstance()->removeAds(kTypeBannerAds);
}

#pragma mark- Ads delegate methods

//void AppManager::onBannerLoadSuccessfully()
//{
//    
//}
//
//void AppManager::onBannerLoadFailed(int errorCode)
//{
//    
//}
//
//void AppManager::onBannerClicked()
//{
//    
//}
//
//void AppManager::onBannerDismissed()
//{
//    
//}
//
//void AppManager::onInterstitialAdLoaded()
//{
//    log("full screen ad dismissed");
//    
//    auto pictureLayer = dynamic_cast<PictureScene*>(cocos2d::Director::getInstance()->getRunningScene()->getChildren().at(0));
//    if (pictureLayer)
//    {
//        auto pageController = dynamic_cast<STPageController*>(pictureLayer->getChildByTag(1234));
//        if (pageController)
//        {
//            pageController->backToOriginal();
//        }
//        return;
//    }
//    
//    auto packLayer = dynamic_cast<PackScene*>(cocos2d::Director::getInstance()->getRunningScene()->getChildren().at(0));
//    if (packLayer)
//    {
//        auto pageController = dynamic_cast<STPageController*>(packLayer->getChildByTag(1234));
//        if (pageController)
//        {
//            pageController->backToOriginal();
//        }
//    }
//}
//
//void AppManager::onInterstitialAdFailed(int errorCode)
//{
//    
//}
//
//void AppManager::onInterstitialShown()
//{
//    
//}
//
//void AppManager::onInterstitialDismissed()
//{
//    
//}
void AppManager::onAdsLoaded(ADS_TYPE adType)
{
    
}



CB_END_NAMESPACE