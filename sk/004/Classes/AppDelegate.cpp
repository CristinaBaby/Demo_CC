#include "AppDelegate.h"
#include "CMVisibleRect.h"
#include "scene/loading/AdsLoadingScene.h"
//#include "SoundConfig.h"
#include "SSCIAPManager.h"
#include "RewardManager.h"
#include "HomeScene.h"
#include "Utils.h"
#include "LoadingTransitionScene.h"
#include "ShopScene.h"
#include "SceneHeader.h"
#include "LoadingPage.h"
#include "IAPAapter.h"
USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(640, 960);

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
        glview = GLViewImpl::createWithRect("Shake004", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("Shake004");
#endif
        director->setOpenGLView(glview);
    }
    
    // turn on display FPS
    director->setDisplayStats(false);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60.0);
    
    Director::getInstance()->setProjection(cocos2d::Director::Projection::_2D);
    CMVisibleRect::setupVisibleRect(designResolutionSize.width, designResolutionSize.height);
    
    std::vector<std::string> searchPaths;
    searchPaths.push_back("ui");
    searchPaths.push_back("res");
    searchPaths.push_back("ui/res");
    searchPaths.push_back("res/npc");
    searchPaths.push_back("ui/cocosstudio/public");
    searchPaths.push_back("ui/ex_Popsicle_png");
    searchPaths.push_back("sound/sound1/popcorn");
    
    FileUtils::getInstance()->setSearchPaths(searchPaths);
    
    register_all_packages();
//    AdsManager::getInstance()->preloadAllAds();
    
    CCLOG("\n%s\n", FileUtils::getInstance()->getWritablePath().c_str());
    
    srand(time(nullptr));
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    kIAPManager->initIAP("config/IAP.csv");
    kRewardManager->loadConfig("config/Reward_Ios.csv");
//    vector<string> oldIAPkeys =
//    {
//        "Shop0",
//        "Shop3",
//        "Shop1",
//        "Shop2",
//    };
//    kIAPManager->updateByOldIAPID(oldIAPkeys);

    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    kIAPManager->initIAP("config/IAP.csv");
    IAPAapter a;
    int index = a.getAndroidPlatformCode();
    log("===%d==" , index);
    if (index == 33)
    {
        kIAPManager->initIAP("config/IAP_A.csv");
        log("config/IAP_A.csv");
        kRewardManager->loadConfig("config/Reward_A.csv");

    }
    if (index == 32)
    {
        kIAPManager->initIAP("config/IAP_G.csv");
        log("config/IAP_G.csv");
        kRewardManager->loadConfig("config/Reward_G.csv");
    }

#endif
    
    // create a scene. it's an autorelease object
    
        director->runWithScene(LoadingPage::scene());
//        director->runWithScene(HomeScene::scene());
    
//    director->runWithScene(SodaMakeScene::scene());
//    director->runWithScene(SodaDecScene::scene());
//        director->runWithScene(SodaShareScene::scene());
    
//    director->runWithScene(TexasPizzaMakeScene::scene());
//        director->runWithScene(TexasPizzaDecScene::scene());
//        director->runWithScene(TexasPizzaShareScene::scene());
    
//    director->runWithScene(PopcornMakeScene::scene());
//    director->runWithScene(PopcornColorMakeScene::scene());
//    director->runWithScene(PopcornDecScene::scene());
//    director->runWithScene(PopcornShareScene::scene());
    
//    director->runWithScene(ChooseServeScene::scene());

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    
    
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    
    log("=========s_showFullAds %d 1111=========",RewardManager::s_showFullAds);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    if (!RewardManager::s_showFullAds)
    {
        log("=========s_showFullAds %d in=========",RewardManager::s_showFullAds);
        
        
        RewardManager::s_showFullAds = true;
        return;
    }
    
#endif
    
    log("=========applicationDidEnterBackground=========");
    AdsLoadingScene::showLoading<AdsLoadingScene>(true,nullptr,INT_MAX);
    
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
