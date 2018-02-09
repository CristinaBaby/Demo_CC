#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "AdapterScreen.h"
#include "IAPManager.h"
#include "SceneManager.h"
#include "Global.h"
#include "AdsManager.h"
#include "HomeLayer.h"
#include "AdsManager.h"
//#include "GameController.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
    
}

AppDelegate::~AppDelegate()
{
    
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }
//    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    kIAPManager->initIAP("IAPInfo.csv");
#endif
//
    
    //获取屏幕大小
    kAdapterScreen->screenSize = glview->getFrameSize();
    log("screenSize------%f,%f",kAdapterScreen->screenSize.width,kAdapterScreen->screenSize.height);

    //适配
    kAdapterScreen->setUpScreenAdapter(960, 640);

    srand((int)time(0));

    log("screenSize------%f,%f",kAdapterScreen->screenSize.width,kAdapterScreen->screenSize.height);

    // turn on display FPS
//    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
//    AdsManager::getInstance()->preloadAllAds();
//    AdsManager::getInstance()->preloadAds(ADS_TYPE::kTypeBannerAds);
    
//  SceneChangeManager- >enterGameScene();
//  SceneChangeManager->enterHomeScene();
//  SceneChangeManager->enterSelectScene();
    SceneChangeManager->enterLoadingScene();

//    g_choose_waste="chocolate";
//    SceneChangeManager->enterDecScene();

//  SceneChangeManager->enterShopScene();

//  SceneChangeManager->enterMarket();
//  SceneChangeManager->enterMarketAction();
//  SceneChangeManager->enterFavScene();
//  SceneChangeManager->enterMakeScene();
//  SceneChangeManager->enterIceCreamScene();
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
     Director::getInstance()->stopAnimation();
//    Director::getInstance()->pause();
     CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
//    Director::getInstance()->resume();

    
    if (IAPManager::getInstance()->isShowAds())
    {
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
