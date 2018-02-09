#include "AppDelegate.h"
#include "Scene/HomeScene.h"
#include "Model/STVisibleRect.h"
#include "Model/ResManager.h"
#include "Model/PaintingManager.h"
#include "AppManager.h"
#include "Layer/AdsLoadingLayer.h"

USING_NS_CC;
using namespace std;

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
    
    namespaceST::STVisibleRect::setupVisibleRect(director->getOpenGLView()->getFrameSize(), Size(DESIGN_WIDTH, DESIGN_HEIGHT));
    
    //set resource search path
    vector<string> searchPaths;
    searchPaths.push_back("UI");
    searchPaths.push_back("UI/ui00_loading");
    searchPaths.push_back("UI/ui01_home");
    searchPaths.push_back("UI/ui02_select");
    searchPaths.push_back("UI/ui03_print");
    searchPaths.push_back("UI/ui04_dress");
    searchPaths.push_back("UI/ui05_photo");
    searchPaths.push_back("UI/ui06_photo_album");
    searchPaths.push_back("UI/ui07_shop");
    searchPaths.push_back("UI/ui08_public");
    searchPaths.push_back("UI/ui09_popup");
    searchPaths.push_back("pattern");
    searchPaths.push_back("fonts");
    searchPaths.push_back("sounds");
    searchPaths.push_back("res");
    searchPaths.push_back("Json");
    FileUtils::getInstance()->setSearchPaths(searchPaths);

    //debug
    log("%s %f %f",FileUtils::getInstance()->getWritablePath().c_str(), Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
    
    //set projection
    Director::getInstance()->setProjection(Director::Projection::_2D);
    
    //setup pens
    PaintingManager::getInstance()->setupPens(DATA_PATH_PENS);
    
    //setup pattern
    PaintingManager::getInstance()->setupPatterns(DATA_PATH_PATTERN);
    
    //setup local res data
    ResManager::getInstance()->setupResData(DATA_PATH_RES);
    
    //setup saved photos data
    std::string savedPhotosPath = FileUtils::getInstance()->getWritablePath() + DATA_PATH_SAVED_PHOTOS;
    ResManager::getInstance()->setupSavedPhotos(savedPhotosPath);
    
    //setup painted res data
    std::string paintResPath = FileUtils::getInstance()->getWritablePath() + DATA_PATH_PAINT_RES;
    ResManager::getInstance()->setupPaintResData(paintResPath);

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = HomeScene::createScene();
    
    // run
    director->runWithScene(scene);
    
    //is first time launch app
    bool isFirstTime = UserDefault::getInstance()->getBoolForKey(UserDefaultKey_FirstTimeLaunchApp, true);
    if (isFirstTime)
    {
        AppManager::getInstance()->setIsFirstTimeLaunchApp(true);
        UserDefault::getInstance()->setBoolForKey(UserDefaultKey_FirstTimeLaunchApp, false);
        UserDefault::getInstance()->flush();
    }
    else
    {
        AppManager::getInstance()->setIsFirstTimeLaunchApp(false);
    }

    if (!AppManager::getInstance()->isRemoveAds())
    {
        /* request ads */
        AppManager::getInstance()->requestBannerAd();
        AppManager::getInstance()->hideBannerAd();
    }

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    if (!AppManager::getInstance()->isRemoveAds())
    {
        AdLoadingLayerBase::showLoading<AdsLoadingLayer>(false);
    }

    if (AppManager::getInstance()->isOpenSound())
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
