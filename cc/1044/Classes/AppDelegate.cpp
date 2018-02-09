#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "AdapterScreen.h"
#include "IAPManager.h"
#include "SceneManager.h"
#include "STAds.h"
#include "CFSystemFunction.h"
#include "Global.h"
#include "AdLoadingLayerBase.h"
#include "AdsLoadingLayer.h"
#include "HomeLayer.h"


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
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    kIAPManager->initIAP("IAPInfo.csv");
#endif
    
    
    //获取屏幕大小
    kAdapterScreen->screenSize = glview->getFrameSize();
    log("screenSize------%f,%f",kAdapterScreen->screenSize.width,kAdapterScreen->screenSize.height);
    
    //适配
    kAdapterScreen->setUpScreenAdapter(640, 960);
    
    srand((int)time(0));

    log("screenSize------%f,%f",kAdapterScreen->screenSize.width,kAdapterScreen->screenSize.height);

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    
//  SceneChangeManager->enterGameScene();
//  SceneChangeManager->enterHomeScene();
//  SceneChangeManager->enterSelectScene();
    SceneChangeManager->enterLoadingScene();

    
    //设置两个游戏状态值   0：锁住 1：解锁动画 2：解锁完成
    
    if(UserDefault::getInstance()->getBoolForKey("first", true)){
        
        UserDefault::getInstance()->setIntegerForKey(IcePop, 0);
        UserDefault::getInstance()->setIntegerForKey(IceCream, 0);
        
        UserDefault::getInstance()->setBoolForKey("first", false);
        UserDefault::getInstance()->flush();
    
    }
    
    
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

//    if (!STAds::isInterstitialShowing)
//    {
//        auto call = []()
//        {
//            if(!AdLoadingLayerBase::s_currentInstance)
//            {
////                auto adLoadingLayer = AdsLoadingLayer::create();
////                Director::getInstance()->getRunningScene()->addChild(adLoadingLayer);
//            }
//            else
//                AdLoadingLayerBase::s_currentInstance->loadAd();
//        };
//        //防止android黑屏
//        Director::getInstance()->getRunningScene()->runAction(Sequence::create(CallFunc::create(call), NULL));
//    }
    if (IAPManager::getInstance()->isShowAds())
    {
        AdLoadingLayerBase::showLoading<AdsLoadingLayer>(false);
        
    }
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
