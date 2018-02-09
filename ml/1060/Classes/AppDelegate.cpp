#include "AppDelegate.h"
#include "CMVisibleRect.h"
#include "LQ_adLoadingLayer.h"
#include "LoadingLayer.h"
USING_NS_CC;

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
        srand((unsigned)time(NULL));
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("MyDonutShop", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("MyDonutShop");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    CMVisibleRect::setupVisibleRect(960, 640);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IAPManager::getInstance()->initIAP("IAP.csv");
#endif
    if(IAPManager::getInstance()->isShowAds()){
        AdsManager::getInstance()->preloadAllAds();
    }
    vector<string> _filePath;
    _filePath.push_back("cocostudio/");
    FileUtils::getInstance()->setSearchPaths(_filePath);
    register_all_packages();
    auto _scne = Scene::create();
    _scne->addChild(LoadingLayer::create());
   // _scne->addChild(dynamic_cast<Layer*>(DynObject::createDynObject("Pricelayer")));
    director->runWithScene(_scne);
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
    if(IAPManager::getInstance()->isShowAds()){
        LQ_adLoadingLayer::isChangeLayer = false;
        LQ_adLoadingLayer::showLoading<LQ_adLoadingLayer>(false);
    }
    // if you use SimpleAudioEngine, it must resume here
     SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
