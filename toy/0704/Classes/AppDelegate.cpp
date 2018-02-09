#include "AppDelegate.h"
#include "HomeScene.h"
#include "Model/AppConfigs.h"
#include "PackManager.h"
#include "Model/STVisibleRect.h"
#include "Model/CBAppManager.h"
#include "Layer/AdsLoadingLayer.h"
#include "AudioHelper.h"
#include "GameScene.h"
#include "UICpp.h"
#include "SVGSource.h"
#include "audio/include/SimpleAudioEngine.h"
#include "CBPackScene.h"
#include "LockManager.h"


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
//void AppDelegate::initGLContextAttrs()
//{
//    //set OpenGL context attributions,now can only set six attributions:
//    //red,green,blue,alpha,depth,stencil
//    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
//    
//    GLView::setGLContextAttrs(glContextAttrs);
//}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("ColorBook");
        director->setOpenGLView(glview);
    }

    Size design_size(DESIGN_WIDTH, DESIGN_HEIGHT);
    Size realSize(design_size.width, design_size.height);
    Size screenSize = Director::getInstance()->getWinSizeInPixels();
    float rate_width=screenSize.width/realSize.width;
    float rate_height=screenSize.height/realSize.height;
    float minScale = rate_width>rate_height?rate_height:rate_width;
    screenSize = Size(screenSize.width/minScale, screenSize.height/minScale);
    vgCreateContextMNK( screenSize.width, screenSize.height, VG_RENDERING_BACKEND_TYPE_OPENGLES20);
    
    namespaceST::STVisibleRect::setupVisibleRect(director->getOpenGLView()->getFrameSize(), design_size);

    //set resource search path
    vector<string> searchPaths;
    searchPaths.push_back("cocos");
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
    
    getcurrTime();
    
    //create loading scene
    auto scene = HomeScene::create();
    director->runWithScene(scene);
    
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    
    auto scene = Director::getInstance()->getRunningScene();
    if (scene->getName().compare("GameScene") == 0) {
        auto layer = scene->getChildByName("colouringLayer");
        if (layer) {
            auto game = (GameScene*)scene;
            game->save();
        }
    }
    
    xLock->saveNetworkState();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();

    if (xApp->getEnableFullAds()) {
        xApp->requestFullScreenAd();
    }
    
    auto scene = Director::getInstance()->getRunningScene();
    if (scene->getName().compare("GameScene") == 0) {
        ((GameScene*)scene)->networkChanged();
    }
    else if(scene->getName().compare("PackScene") == 0)
    {
        ((PackScene*)scene)->networkChanged();
    }
    
    
}
