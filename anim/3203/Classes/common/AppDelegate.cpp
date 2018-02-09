#include "AppDelegate.h"
#include "XCVisibleRect.h"
#include "GameHeader.h"
#include "UserDataManager.h"
#include "HomeScene.h"
#include "IAPManager.h"
#include "LoadingLayer.h"
#include "STSystemFunction.h"
//#include "SPAScene.h"
//#include "AnimalModel.h"

USING_NS_CC;

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
    
    XCVisibleRect::setVisibleRect(glview->getFrameSize(), Size(640, 960));
    glview->setDesignResolutionSize(XCVisibleRect::getRealDesignSize().width, XCVisibleRect::getRealDesignSize().height, ResolutionPolicy::SHOW_ALL);
    
    // turn on display FPS
    director->setDisplayStats(false);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
//    AdsManager::getInstance()->preloadAllAds();
    // create a scene. it's an autorelease object
    auto scene = HomeScene::create();
    
//    MoreGameLoader::getInstance()->loadIcon();
    
    // run
    director->runWithScene(scene);
    
    STSystemFunction func;
    func.showNewsBlast();
    
    if (!UserDataManager::isOpenMusic()) {
        UserDataManager::closeMusic();
    }
    
    if (!UserDataManager::isOpenSound()) {
        UserDataManager::closeSound();
    }
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/menu.mp3", true);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    
    Director::getInstance()->stopAnimation();
    if (UserDataManager::isOpenSound()) {
        CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
    
    if (UserDataManager::isOpenMusic()) {
        CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
    }
    
    // if you use SimpleAudioEngine, it must be pause
//     SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    
    Director::getInstance()->startAnimation();
    
    if (UserDataManager::isOpenSound()) {
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }
    
    if (UserDataManager::isOpenMusic()) {
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
    }
    
    if (!IAPManager::getInstance()->isAdRemoved() && !LoadingLayer::s_currentInstance) {
        LoadingLayer::showLoading<LoadingLayer>(false, Director::getInstance()->getRunningScene(), 99999);
    }
    
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(kGlobal_WillEnterForegroundEvent);
    // if you use SimpleAudioEngine, it must resume here
//     SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
