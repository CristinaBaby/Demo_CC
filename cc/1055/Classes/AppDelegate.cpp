#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "AdapterScreen.h"
#include "MainScene.h"
#include "KitchenLayer.h"
#include "MakeSceneOne.h"
#include "GameData.h"
#include "MakeOtherScene.h"
#include "HomeScene.h"
#include "AdsLoadingLayer.h"
#include "LoadingScene.h"
#include "IAPManager.h"

USING_NS_CC;

using namespace std;

AppDelegate::AppDelegate()
{
    
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
        glview = GLViewImpl::createWithRect("CCATS1054_CookieMaker", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("CCATS1054_CookieMaker");
#endif
        director->setOpenGLView(glview);
    }
    
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    kAdapterScreen->setUpScreenAdapter(960,640);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IAPManager::getInstance()->initIAP("IAPInfo.csv");
#endif
    vector<string> searchPath;
    searchPath.push_back("cocostudio/");
    FileUtils::getInstance()->setSearchPaths(searchPath);
    
    AdsManager::getInstance()->preloadAds(ADS_TYPE::kTypeCrosspromoAds);
    
    //DEBUG
//    GameDataInstance->setFlavor(FlavorTypeChololateChip);
//    GameDataInstance->setFlavor("chocolate");
//    GameDataInstance->setMold("star");
    GameDataInstance->setIsMoreGame(false);
    GameDataInstance->setBowlIndex(-1);
    GameDataInstance->setIsShowAcg(false);
//   log("------>%s",gameState.c_str());v
//   UserDefault::getInstance()->setStringForKey(GameState,GameStateOne);
//   UserDefault::getInstance()->flush();
//   GameDataInstance->setFlavor("chocolate");
//   GameDataInstance->setMold("star");
//   GameDataInstance->setFlavor(FlavorTypeChololateChip);
//   GameDataInstance->setGameType(GameTypeOne);
//   MainScene::goToMainScene();
    
//   UserDefault::getInstance()->setStringForKey(GameState,GameStateTwo);
//   UserDefault::getInstance()->flush();
    
    Director::getInstance()->runWithScene(LoadingScene::createScene());
    
    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    if(IAPManager::getInstance()->isShowAds())
        AdLoadingLayerBase::showLoading<AdsLoadingLayer>(false);

}
