//
//  CBLoadingScene.cpp
//  ColorBook
//
//  Created by maxiang on 4/21/15.
//
//

#include "CBLoadingScene.h"
#include "../Scene/CBHomeScene.h"
#include "../Model/CBAppManager.h"
#include "../Model/CBColouringManager.h"
#include "../Model/CBUtils.h"
#include "../Layer/AdsLoadingLayer.h"
#include "AdLoadingLayerBase.h"

USING_NS_CC;
CB_BEGIN_NAMESPACE

#define RES_LOADING_BG      "ui06_loading_bg.jpg"
#define RES_PROGRESS_BG     "ui06_loading_bar.png"
#define RES_PROGRESS        "ui06_loading_sroll.png"
#define RES_HOME_LOGO       "ui07_home_logo.png"

LoadingScene::~LoadingScene()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool LoadingScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    bool rcode = false;
    do {
        //set background
        Sprite *background = Sprite::create(RES_LOADING_BG);
        CC_BREAK_IF(!background);
        background->setPosition(Utils::getInstance()->getScreenCenter());
        addChild(background);
        
        Sprite *logo = Sprite::create(RES_HOME_LOGO);
        CC_BREAK_IF(!logo);
        logo->setPosition(Vec2(Utils::SCREEN_WIDTH/2, Utils::SCREEN_HEIGHT*2/3+45));
        addChild(logo);
        
        //init progress background
        Sprite *progressbg = Sprite::create(RES_PROGRESS_BG);
        CC_BREAK_IF(!progressbg);
        progressbg->setPosition(Director::getInstance()->getVisibleSize().width/2, Director::getInstance()->getVisibleSize().height/3-progressbg->getContentSize().height/2);
        addChild(progressbg);
        
        //init progress
        Sprite *progress = Sprite::create(RES_PROGRESS);
        _progress = ProgressTimer::create(progress);
        CC_BREAK_IF(!_progress);
        _progress->setPosition(progressbg->getPosition());
        _progress->setType(ProgressTimer::Type::BAR);
        _progress->setMidpoint(Vec2(0, 1));
        _progress->setBarChangeRate(Vec2(1, 0));
        _progress->setPercentage(0);
        addChild(_progress);
        
        //progress
        ProgressTo *progressTo = ProgressTo::create(1, 100);
        _progress->runAction(Sequence::create(progressTo, CallFunc::create(CC_CALLBACK_0(LoadingScene::finishLoad, this)), NULL));
        
        //load bgm
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(SOUND_BUTTON);
        
        //add texture cache
        std::vector<Pen> *pens1 = ColouringManager::getInstance()->getPens1();
        for (auto iterator = pens1->begin(); iterator != pens1->end(); ++iterator)
        {
            Director::getInstance()->getTextureCache()->addImage(iterator->penLogo);
        }
        
        std::vector<Pen> *pens2 = ColouringManager::getInstance()->getPens2();
        for (auto iterator = pens2->begin(); iterator != pens2->end(); ++iterator)
        {
            Director::getInstance()->getTextureCache()->addImage(iterator->penLogo);
        }
        
        std::vector<Pattern> *patterns = ColouringManager::getInstance()->getPatterns();
        for (auto iterator = patterns->begin(); iterator != patterns->end(); ++iterator)
        {
            Director::getInstance()->getTextureCache()->addImage(iterator->patternLogo);
        }
        AppManager::getInstance()->setIsGoBackToHome(false);

 
        rcode = true;
    } while (0);
    
    return rcode;
}

#pragma mark- next scene

void LoadingScene::finishLoad()
{
    auto scene = HomeScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

void LoadingScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    bool isFirstLaunch = UserDefault::getInstance()->getBoolForKey(UserDefaultKey_FirstTimeLaunchApp, true);
    if (isFirstLaunch)
    {
        /* if is first time launch app, DO NOT request full screen ads */
        UserDefault::getInstance()->setBoolForKey(UserDefaultKey_FirstTimeLaunchApp, false);
        UserDefault::getInstance()->flush();
        UserDefault::getInstance()->setBoolForKey("firstIntoApp", true);
        AppManager::getInstance()->setIsFirstLaunchApp(true);
    }
    else
    {
        /* -----Ads Logic----- */
        AppManager::getInstance()->setIsFirstLaunchApp(false);
        UserDefault::getInstance()->setBoolForKey("firstIntoApp", false);
    }
}


CB_END_NAMESPACE