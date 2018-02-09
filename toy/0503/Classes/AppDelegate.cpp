#include "AppDelegate.h"
#include "HomeScene.h"
#include "Model/STVisibleRect.h"
#include "Model/MiniGameManager.h"
#include "ColorBookScene.h"
#include "SpotDifferenceScene.h"
#include "JigsawPuzzleScene.h"
#include "RecordAudioScene.h"
#include "AppManager.h"
#include "PlayAudioScene.h"
#include "AdsManager.h"

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
    searchPaths.push_back("ui");
    searchPaths.push_back("ui/ui01_home");
    searchPaths.push_back("ui/ui02_page");
    searchPaths.push_back("ui/ui03_kids_bedtime");
    searchPaths.push_back("ui/ui04_select");
    searchPaths.push_back("ui/ui05_puzzle");
    searchPaths.push_back("ui/ui06_public");
    searchPaths.push_back("ui/ui07_pupup");
    searchPaths.push_back("ui/ui08_spot");
    searchPaths.push_back("ui/ui09_colorbook");
    searchPaths.push_back("ui/ui010_parent");
    searchPaths.push_back("pattern");
    searchPaths.push_back("fonts");
    searchPaths.push_back("sounds");
    searchPaths.push_back("res");
    searchPaths.push_back("res/logos");
    searchPaths.push_back("res/Reading");

    searchPaths.push_back("Json");
    searchPaths.push_back("cocostudio");
    searchPaths.push_back("cocostudio/json");
    searchPaths.push_back("cocostudio/studio_ui");

    FileUtils::getInstance()->setSearchPaths(searchPaths);
    
    //debug
    log("%s %f %f",FileUtils::getInstance()->getWritablePath().c_str(), Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
 
    //init data
    MiniGameManager::getInstance()->initSpotData();
    MiniGameManager::getInstance()->initJigsawData();
    MiniGameManager::getInstance()->initColorBookData();
    MiniGameManager::getInstance()->initRecordData();

    MiniGameManager::getInstance()->setupPens("config/Pens.csv");
    MiniGameManager::getInstance()->setupPatterns("config/Patterns.csv");
    
    //set projection
    Director::getInstance()->setProjection(Director::Projection::_2D);

    // create a scene. it's an autorelease object
    auto scene = BaseScene::createScene<HomeScene>();
//    auto layer = dynamic_cast<JigsawPuzzleScene*>(scene->getChildren().at(0));
//    layer->startGameWithGameLevel(1);

    // run
    director->runWithScene(scene);
    
    if (!AppManager::getInstance()->getHasRemoveAds())
    {
        //preload all ads
        AdsManager::getInstance()->preloadAllAds();
        AdsManager::getInstance()->setLayoutBanner(kLayoutBottomCenter);
        
        /* request ads */
        AppManager::getInstance()->requestBannerAd();
        AppManager::getInstance()->setBannerAdVisible(false);
    }

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    //save record info
    MiniGameManager::getInstance()->saveRecordInfo();
    
    Director::getInstance()->stopAnimation();

    auto scene = Director::getInstance()->getRunningScene();
    if (scene->getChildren().size() > 0)
    {
        //如果当前是录音场景，此时resume不要弹广告，否则会把广告声音录进去
        auto layer = dynamic_cast<RecordAudioScene*>(scene->getChildren().at(0));
        if (!layer)
            /* -----Ads Logic----- */
            AppManager::getInstance()->requestFullScreenAd();
    }

    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}







