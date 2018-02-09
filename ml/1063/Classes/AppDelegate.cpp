#include "AppDelegate.h"
//#include "CMVisibleRect.h"
#include "LoadingLayer.h"
#include "LQ_adLoadingLayer.h"
#include "MainScene.h"
#include "AdapterScreen.h"
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
        glview = GLViewImpl::createWithRect("WeddingFoodMaker", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("WeddingFoodMaker");
#endif
        director->setOpenGLView(glview);
    }
    // turn on display FPS
    director->setDisplayStats(false);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    // Set the design resolution
    
    // if the frame's height is larger than the height of medium size.
    //    CMVisibleRect::setupVisibleRect(640, 960);
    kAdapterScreen->setUpScreenAdapter(640,960);
    
    register_all_packages();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IAPManager::getInstance()->initIAP("IAP.csv");
#endif
    if(IAPManager::getInstance()->isShowAds())
        AdsManager::getInstance()->preloadAllAds();
    vector<string> searchPath;
    searchPath.push_back("cocostudio/");
    FileUtils::getInstance()->setSearchPaths(searchPath);
    // create a scene. it's an autorelease object
    // run
    Director::getInstance()->runWithScene(LoadingLayer::createScene());
    //MainScene::goToMainScene();
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    
    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    
    if(IAPManager::getInstance()->isShowAds()&&!AdsManager::getInstance()->isFullScreenShowing()){
        LQ_adLoadingLayer::isChangeLayer = false;
        AdLoadingLayerBase::showLoading<LQ_adLoadingLayer>(false);
    }
    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
