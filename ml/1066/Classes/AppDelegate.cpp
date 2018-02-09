#include "AppDelegate.h"
#include "AdapterScreen.h"
#include "Global.h"
#include "SceneManager.h"
#include "AdsManager.h"
#include "IAPManager.h"
#include "SoundPlayer.h"
#include "AdsManager.h"

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
        glview = GLViewImpl::createWithRect("IceCream", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("IceCream");
#endif
        director->setOpenGLView(glview);
    }
    
    // init iap
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    kIAPManager->initIAP("IAP_MLABS1066.csv");
#endif
    
    kAdapterScreen->setUpScreenAdapter(DESIGN_WIDTH, DESIGN_HEIGHT);

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    std::vector<std::string> searchPath;
    searchPath.push_back("images/dialog");
    searchPath.push_back("sounds");
    searchPath.push_back("sounds/new_mp3");
    
    // cone
    searchPath.push_back("images/dec/decCone/icon_sprinkle");
    searchPath.push_back("images/dec/decCone/icon_sticker");
    searchPath.push_back("images/dec/decCone/icon_syrup");
    
    searchPath.push_back("images/dec/decCone");
    searchPath.push_back("images/dec/decCone");
    searchPath.push_back("images/dec/decCone");
    
    // icecream
    searchPath.push_back("images/dec/decIceCream/icon_candy");
    searchPath.push_back("images/dec/decIceCream/icon_fruit");
    searchPath.push_back("images/dec/decIceCream/icon_wafer");
    
    searchPath.push_back("images/dec/decIceCream");
    searchPath.push_back("images/dec/decIceCream");
    searchPath.push_back("images/dec/decIceCream");
    
    // choose page
    searchPath.push_back("images/step/choose/icon_icecream");
    
    FileUtils::getInstance()->setSearchPaths(searchPath);
    
//    AdsManager::getInstance()->preloadAllAds();
    
    
    // run
    SceneManager::getInstance()->enterLoadingScene();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SoundPlayer::getInstance()->pauseBackGroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SoundPlayer::getInstance()->resumeBackGroundMusic();
    
    if (kIAPManager->isShowAds())
    {
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
    }
}
