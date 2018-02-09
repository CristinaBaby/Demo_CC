#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "AdapterScreen.h"
#include "SceneManager.h"
#include "EatLayer.h"
#include "OutMoldScene.h"
#include "IAPManager.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("MLABS1061_GummyMaker", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("MLABS1061_GummyMaker");
#endif
        director->setOpenGLView(glview);
    }

//    director->setDisplayStats(true);
    director->setAnimationInterval(1.0 / 60);
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    kIAPManager->initIAP("IAPInfo.csv");
#endif
    
    //获取屏幕大小
    kAdapterScreen->screenSize = glview->getFrameSize();
    log("screenSize------%f,%f",kAdapterScreen->screenSize.width,kAdapterScreen->screenSize.height);
    
    //适配
    kAdapterScreen->setUpScreenAdapter(960, 640);
    
    if (kIAPManager->isShowAds()) {
        AdsManager::getInstance()->preloadAllAds();
    }
    
    
      srand((int)time(0));
    SceneChangeManager->enterLoadingScene();
    
//    SceneChangeManager->enterMakeMoldScene();
//    SceneChangeManager->enterCookMakeScene();
//    SceneChangeManager->enterPopFruitScene();
//    SceneChangeManager->enterCookMixScene();
//    SceneChangeManager->enterHomeScene();
//    SceneChangeManager->enterShopScene();
//    SceneChangeManager->enterEatScene();
//    auto scene = EatLayer::createScene();
//    Director::getInstance()->runWithScene(scene);
//    Scene* scene = OutMoldScene::createScene();
//    Director::getInstance()->replaceScene(scene);
//    Director::getInstance()->replaceScene(TransitionSlideInR::create(1.0,scene));
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
        AdsLoadingLayer::m_bIsFrom = true;
        AdLoadingLayerBase::showLoading<AdsLoadingLayer>(false);
        
    }
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
