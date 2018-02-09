#include "AppDelegate.h"
#include "Model/AppConfigs.h"
#include "Scene/CBLoadingScene.h"
#include "Scene/CBGameScene.h"
#include "Model/CBPackManager.h"
#include "Model/STVisibleRect.h"
#include "Model/CBColouringManager.h"
#include "Model/CBAppManager.h"
#include "Module/AnalyticX.h"
#include "Layer/AdsLoadingLayer.h"
#include "ScribbleLineFrameNode.h"

USING_NS_CC;
USING_CB_NAMESPACE;
using namespace std;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("ColorBook");
        director->setOpenGLView(glview);
    }

    namespaceST::STVisibleRect::setupVisibleRect(director->getOpenGLView()->getFrameSize(), Size(DESIGN_WIDTH, DESIGN_HEIGHT));

    //set resource search path
    vector<string> searchPaths;
    searchPaths.push_back("UI");
    searchPaths.push_back("UI/UI01-Home");
    searchPaths.push_back("UI/UI02-Settings");
    searchPaths.push_back("UI/UI03-Pack");
    searchPaths.push_back("UI/UI04-Play");
    searchPaths.push_back("UI/UI05-Popup");
    searchPaths.push_back("UI/UI06-Loading");
    searchPaths.push_back("UI/UI07-Public");
    searchPaths.push_back("config");
    searchPaths.push_back("pattern");
    searchPaths.push_back("fonts");
    searchPaths.push_back("sound");
    searchPaths.push_back("UI/ParentalGate");
    searchPaths.push_back("UI/Disclaimer");
    
    FileUtils::getInstance()->setSearchPaths(searchPaths);
    Director::getInstance()->setProjection(Director::Projection::_2D);

    //setup pack info
    PackManager::getInstance()->setupLocalPacksInfo(LOCAL_PACKS_FILE_NAME);
    PackManager::getInstance()->setupServerPacksInfo(SERVER_PACKS_FILE_NAME);
    
    //setup pens info
    ColouringManager::getInstance()->setupPens1("pens1.plist");
    ColouringManager::getInstance()->setupPens2("pens2.plist");
    ColouringManager::getInstance()->setupPatterns("pattern.plist");

    //check server pack undate info
    PackManager::getInstance()->checkServerPackUpdateInfo();
    
    //create loading scene
    auto scene = LoadingScene::createScene();
    director->runWithScene(scene);

    /* Flurry */
    AnalyticX::flurryStartSession(FLURRY_ID);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    
    auto scene = Director::getInstance()->getRunningScene();
   
    if (scene) {
        auto layer = scene->getChildByName("GameScene");
        if (layer) {
            auto game = (GameScene*)layer;
            game->savingActionForEnterBackground();
        }
    }
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    xApp->showLoading(false);
    
    if (AppManager::getInstance()->isOpenBgm())
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
