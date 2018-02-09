#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "KSVisibleRect.h"
#include "GameMaster.h"
#include "FavManager.h"
#include "IAPManager.h"
#include "AudioController.h"
#include "AdsLoadingLayer.h"
USING_NS_CC;
#define DEFAULT_SCREEN_WIDTH 640
#define DEFAULT_SCREEN_HEIGHT 960

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
    
    // turn on display FPS
    director->setDisplayStats(false);
    Size degisnSize = Size(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
    
    KSVisibleRect::setupVisibleRect(glview->getFrameSize(), degisnSize);
    
    glview->setDesignResolutionSize(
                                    KSVisibleRect::getRealDesignSize().width,
                                    KSVisibleRect::getRealDesignSize().height,
                                    ResolutionPolicy::SHOW_ALL);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
//    FileUtils::getInstance() -> addSearchPath("sound");
    FavManager::getInstance() -> init();
    AudioController::getInstance() -> init();
    GameMaster::getInstance() -> init();
    //    // create a scene. it's an autorelease object
    
//GameMaster::getInstance() -> goToDecorationLayer();
    GameMaster::getInstance()->goToLoadingPage();
    AdsManager::getInstance()->preloadAllAds();
    

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    AdvertiseAdapter::preloadAllAds();
    IAPManager::getInstance() -> initIAP("IAP.csv");

#endif


    //    // run
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    

    if (IAPManager::getInstance()->isShowAds())
    {
        AdsLoadingLayer::showLoading<AdsLoadingLayer>(false);
    }
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
