//
//  CBHomeScene.cpp
//  ColorBook
//
//  Created by maxiang on 4/21/15.
//
//

#include "CBHomeScene.h"

#include "../Model/CBUtils.h"
#include "../Layer/CBSettingLayer.h"
#include "../Scene/CBPackScene.h"
#include "../Model/CBAppManager.h"
#include "../Model/STVisibleRect.h"
#include "../Module/STSystemFunction.h"
#include "../Model/CBColouringManager.h"
#include "../Layer/CBDownloadFaildLayer.h"
#include "../Layer/CBQuitGameLayer.h"
#include "../Module/AnalyticX.h"
#include "../Layer/CBDisclaimerLayer.h"

USING_NS_CC;
CB_BEGIN_NAMESPACE

#define RES_BG                  "ui01_home_bg.jpg"
#define RES_HOME_LOGO           "ui07_home_logo.png"

#define RES_PLAY_BUTTON         "ui01_btn_play.png"
#define RES_PLAY_BUTTON_H       "ui01_btn_play_h.png"

#define RES_SETTING_BUTTON      "ui01_btn_settings.png"
#define RES_SETTING_BUTTON_H    "ui01_btn_settings_h.png"

#define RES_MORE_BUTTON         "ui01_btn_more.png"
#define RES_MORE_BUTTON_H       "ui01_btn_more_h.png"

#define RES_RESTORE_BUTTON      "ui01_btn_restore.png"
#define RES_RESTORE_BUTTON_H    "ui01_btn_restore_h.png"

#define RES_BUTTON_POLICY       "ui01_btn_privacy.png"
#define RES_BUTTON_POLICY_H     "ui01_btn_privacy_h.png"

#define TAG_QUIT_GAME_LAYER     123

HomeScene::~HomeScene()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool HomeScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    bool rcode = false;
    do {
        
        //back ground
        Sprite *background = Sprite::create(RES_BG);
        CC_BREAK_IF(!background);
        background->setPosition(Utils::getInstance()->getScreenCenter());
        addChild(background);
        
        //logo
        Sprite *logo = Sprite::create(RES_HOME_LOGO);
        CC_BREAK_IF(!logo);
        namespaceST::STVisibleRect::setPosition(logo, 480, 493);
        addChild(logo);
        
        //play
        Sprite *play = Sprite::create(RES_PLAY_BUTTON);
        CC_BREAK_IF(!play);
        namespaceST::STVisibleRect::setPosition(play, 480, 275);
        addChild(play);
        
        //setting
        Sprite *setting = Sprite::create(RES_SETTING_BUTTON);
        CC_BREAK_IF(!setting);
        namespaceST::STVisibleRect::setPosition(setting, 200, 80);
        addChild(setting);
        
        //more
        Sprite *more = Sprite::create(RES_MORE_BUTTON);
        namespaceST::STVisibleRect::setPosition(more, 480, 80);
        addChild(more);
        
        //restore
        Sprite *restore = Sprite::create(RES_RESTORE_BUTTON);
        namespaceST::STVisibleRect::setPosition(restore, 760, 80);
        addChild(restore);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        restore->setVisible(false);
        
        more->setPosition(restore->getPosition());
#endif

        //policy
//        Sprite *policy = Sprite::create(RES_BUTTON_POLICY);
//        policy->setVisible(false);
//        namespaceST::STVisibleRect::setPosition(policy, 845, 595, true, false, true, false);
//        addChild(policy);

        //touch event listen
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [=](Touch* touch, Event* event) {
            
            Point point = touch->getLocation();
            if (play->getBoundingBox().containsPoint(point))
            {
                play->setTexture(RES_PLAY_BUTTON_H);
            }
            else if (setting->getBoundingBox().containsPoint(point))
            {
                setting->setTexture(RES_SETTING_BUTTON_H);
            }
            else if (more->getBoundingBox().containsPoint(point))
            {
                more->setTexture(RES_MORE_BUTTON_H);
            }
            else if (restore->getBoundingBox().containsPoint(point))
            {
                restore->setTexture(RES_RESTORE_BUTTON_H);
            }
//            else if (policy->getBoundingBox().containsPoint(point))
//            {
//                policy->setTexture(RES_BUTTON_POLICY_H);
//            }
            return true;
        };
        
        touchEventListener->onTouchEnded = [=](Touch* touch, Event* event) {
            
            Point point = touch->getLocation();
            if (play->getBoundingBox().containsPoint(point))
            {
                play->setTexture(RES_PLAY_BUTTON);
                HomeScene::playAction();
            }
            else if (setting->getBoundingBox().containsPoint(point))
            {
                setting->setTexture(RES_SETTING_BUTTON);
                HomeScene::settingAction();
            }
            else if (more->getBoundingBox().containsPoint(point))
            {
                more->setTexture(RES_MORE_BUTTON);
                HomeScene::moreAction();
            }
            else if (restore->getBoundingBox().containsPoint(point))
            {
                restore->setTexture(RES_RESTORE_BUTTON);
                HomeScene::restoreAction();
            }
//            else if (policy->getBoundingBox().containsPoint(point))
//            {
//                policy->setTexture(RES_BUTTON_POLICY);
//                HomeScene::policyAction();
//            }
        };
        
        touchEventListener->onTouchMoved = [=](Touch* touch, Event* event) {
            
            Point point = touch->getLocation();
            
            play->setTexture(RES_PLAY_BUTTON);
            setting->setTexture(RES_SETTING_BUTTON);
            more->setTexture(RES_MORE_BUTTON);
            restore->setTexture(RES_RESTORE_BUTTON);
            //policy->setTexture(RES_BUTTON_POLICY);

            if (play->getBoundingBox().containsPoint(point))
            {
                play->setTexture(RES_PLAY_BUTTON_H);
            }
            else if (setting->getBoundingBox().containsPoint(point))
            {
                setting->setTexture(RES_SETTING_BUTTON_H);
            }
            else if (more->getBoundingBox().containsPoint(point))
            {
                more->setTexture(RES_MORE_BUTTON_H);
            }
            else if (restore->getBoundingBox().containsPoint(point))
            {
                restore->setTexture(RES_RESTORE_BUTTON_H);
            }
//            else if (policy->getBoundingBox().containsPoint(point))
//            {
//                policy->setTexture(RES_BUTTON_POLICY_H);
//            }
        };
        
        touchEventListener->onTouchCancelled = [=](Touch* touch, Event* event) {
            
            play->setTexture(RES_PLAY_BUTTON);
            setting->setTexture(RES_SETTING_BUTTON);
            more->setTexture(RES_MORE_BUTTON);
            restore->setTexture(RES_RESTORE_BUTTON);
            //policy->setTexture(RES_BUTTON_POLICY);
        };
        
        auto listener = EventListenerKeyboard::create();
        listener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* event)
        {
            if (code == EventKeyboard::KeyCode::KEY_BACK)
            {
                if (getChildByTag(TAG_QUIT_GAME_LAYER) == NULL)
                {
                    auto quitLayer = QuitGameLayer::create();
                    quitLayer->setYesActionCallback(CC_CALLBACK_0(HomeScene::quitGame, this));
                    quitLayer->setNoActionCallback(CC_CALLBACK_0(HomeScene::cancelQuitGame, this));
                    quitLayer->setTag(TAG_QUIT_GAME_LAYER);
                    this->addChild(quitLayer);
                }
            }
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        AppManager::getInstance()->setBannerAdVisible(false);
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        //show terms if needed
//        if (AppManager::getInstance()->isFirstLaunchApp())
//        {
//            auto terms = DisclaimerLayer::create();
//            addChild(terms);
//            
//            AppManager::getInstance()->setIsFirstLaunchApp(false);
//        }
#else
        
#endif
        
        rcode = true;
    } while (0);
    
    return rcode;
}

#pragma mark- Quit game for android

void HomeScene::quitGame()
{
    AnalyticX::flurryEndSession();
    Director::getInstance()->end();
    
    exit(0);
}

void HomeScene::cancelQuitGame()
{
    AppManager::getInstance()->setBannerAdVisible(false);
}

#pragma mark- Parental Gate

void HomeScene::parentalGatePass(int tag)
{
    if (tag == 1)
    {
        //for more game
        STSystemFunction *sf = new STSystemFunction;
        sf->showMoreGame();
        CC_SAFE_DELETE(sf);
        log("More Action!");
    }
    else if(tag == 2)
    {
        //for restore
        log("restore Action!");
        
        _billing.setIABDelegate(this);
        _billing.restore();
    }
}

#pragma mark- Actions

void HomeScene::playAction()
{
    AppManager::getInstance()->playButtonEffect();
    
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);

    log("Play Action!");
    auto scene = PackScene::createScene();
    PackScene::showCrossPromo = true;
    Director::getInstance()->replaceScene(scene);
}

void HomeScene::settingAction()
{
    log("Setting Action!");
    AppManager::getInstance()->playButtonEffect();

    SettingLayer *settingLayer = SettingLayer::create();
    settingLayer->setPosition(Vec2(0.1, 0));
    settingLayer->setPosition(Point::ZERO);
                              
    addChild(settingLayer);
}

void HomeScene::moreAction()
{
    AppManager::getInstance()->playButtonEffect();
    
    STSystemFunction *sf = new STSystemFunction;
    sf->showMoreGame();
    CC_SAFE_DELETE(sf);
    log("More Action!");
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    ParentalGateLayer *layer = ParentalGateLayer::create();
//    layer->setDelegate(this, 1);
//    layer->showInLayer(this);
//#else
//    STSystemFunction *sf = new STSystemFunction;
//    sf->showMoreGame();
//    CC_SAFE_DELETE(sf);
//    log("More Action!");
//#endif
}

void HomeScene::restoreAction()
{
    AppManager::getInstance()->playButtonEffect();
 
    _billing.setIABDelegate(this);
    _billing.restore();
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    ParentalGateLayer *layer = ParentalGateLayer::create();
//    layer->setDelegate(this, 2);
//    layer->showInLayer(this);
//#else
//    log("restore Action!");
//    
//    _billing.setIABDelegate(this);
//    _billing.restore();
//#endif
}

void HomeScene::policyAction()
{
    AppManager::getInstance()->playButtonEffect();
  
    log("Policy Action!");
}

#pragma mark- STIABDelegate

void HomeScene::purchaseSuccessful(const char* pid)
{
    log("MX: Purchase success!");
}

void HomeScene::purchaseFailed(const char *pid, int errorCode)
{
    log("MX: Purchase failed!");
}

void HomeScene::restoreSuccessful(const char* pid)
{
    if (std::string(pid) == IAP_UNLOCK_PATTERN_PEN)
    {
        ColouringManager::getInstance()->unlockPatternPen();
    }
    else if (std::string(pid) == IAP_UNLOCK_SPECIAL_PACK)
    {
        PackManager::getInstance()->unlockSpecialPack();
    }
    else
    {
        std::string userDefaultKey = UserDefaultKey_HasPurchasedPack + std::string(pid);
        UserDefault::getInstance()->setBoolForKey(userDefaultKey.c_str(), true);
        UserDefault::getInstance()->flush();
    }
    
    log("MX: Restore success!");
}

void HomeScene::restoreFailed(const char* pid, int errorCode)
{
    log("MX: Restore failed!");
}

CB_END_NAMESPACE
