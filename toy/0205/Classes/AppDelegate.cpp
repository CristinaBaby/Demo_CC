#include "AppDelegate.h"
#include "HomeScene.h"
#include "Model/AppConfigs.h"
#include "Model/CBPackManager.h"
#include "Model/STVisibleRect.h"
#include "Model/CBAppManager.h"
#include "Layer/AdsLoadingLayer.h"
#include "AudioHelper.h"
#include "GameScene.h"
#include "UICpp.h"

USING_NS_CC;
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
    searchPaths.push_back("cocos");
    searchPaths.push_back("animation");
    searchPaths.push_back("sound");
    searchPaths.push_back("cocos/choose-1");
    searchPaths.push_back("config");
    searchPaths.push_back("fonts");
    
    FileUtils::getInstance()->setSearchPaths(searchPaths);
    Director::getInstance()->setProjection(Director::Projection::_2D);

    //check server pack undate info
    xPack->checkServerPackUpdateInfo();
    
    xAds->preloadAllAds();
    xAds->showAds(kTypeBannerAds);
    xAdsEx->setBannerShow(true);
    
    //create loading scene
    auto scene = HomeScene::create();
    director->runWithScene(scene);
    
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
        auto layer = scene->getChildByName("colouringLayer");
        if (layer) {
            auto game = (GameScene*)scene;
            game->save();
        }
    }
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    xApp->requestFullScreenAd();
    
    if (xAudio->getMusicVolume() > FLT_EPSILON)
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
