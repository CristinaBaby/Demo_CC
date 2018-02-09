#include "AppDelegate.h"
#include "AppConfig.h"
#include "utilities/STMultiScreenHelper.h"
#include "scenes/LoadingScene.h"
#include "utilities/STResManager.h"
#include "utilities/STVisibleRect.h"
#include "utilities/SceneManager.h"
#include "modules/AnalyticX.h"
#include "Spot/SpotScene.h"

#include "helpers/AdHelper.h"
#include "helpers/AudioHelper.h"

USING_NS_CC;


AppDelegate::AppDelegate() {
    isFirstRunOnAndroid = true;
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("Bedtime Story");
        director->setOpenGLView(glview);
    }
    
    // initialize MultiScreenHelper static value.
    cocos2d::Rect rect = MultiScreenHelper::getRect(director->getVisibleOrigin(), director->getVisibleSize());
    log("%f %f", rect.origin.x, rect.origin.y);
    log("%f %f", rect.size.width, rect.size.height);
    
    //screenSize, degisnSize
    V::setupVisibleRect(glview->getFrameSize(), Size(960, 640));
    
    
    // set ui search path
    FileUtils::getInstance()->setSearchPaths(namespaceST::STResManager::getInstance()->getFileSearchPath());
    
    //
//    Size vsize = Director::getInstance()->getVisibleSize();
//    float scaleX = vsize.width / dw;
//    float scaleY = vsize.height/ dh;
//    UI_SCALE = MIN(scaleX, scaleY);
    
    Director::getInstance()->setProjection(Director::Projection::_2D);
    // create a scene. it's an autorelease object
    xScene->forward(S_LOADING);
    
    //TBD
    // start flurry session.
    AnalyticX::flurryStartSession(st_flurry_key);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    
    auto scene = Director::getInstance()->getRunningScene();
    auto layer = dynamic_cast<SpotScene*>(scene->getChildByName("SpotScene"));
    
    if (nullptr != layer)
    {
        layer->onPauseCallback(nullptr);
    }
    
    //将所有音效都停止了, 音乐系统会帮你停止
    xAudio->stopAllSounds();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    //MX: request every time
    AdHelper::requestInterstitialAds();
    
    auto scene = Director::getInstance()->getRunningScene();
    auto layer = dynamic_cast<SpotScene*>(scene->getChildByName("SpotScene"));
    
    if (nullptr != layer)
    {
        layer->onResumeCallback(nullptr);
    }
}
