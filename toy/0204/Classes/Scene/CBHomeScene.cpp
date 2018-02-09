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
#include "../Layer/AdsLoadingLayer.h"
#include "AdLoadingLayerBase.h"


USING_NS_CC;
CB_BEGIN_NAMESPACE

#define RES_BG                  "ui01_home_bg.jpg"
#define RES_HOME_LOGO           "ui07_home_logo.png"

#define RES_PLAY_BUTTON         "ui01_btn_play.png"
#define RES_SETTING_BUTTON      "ui01_btn_settings.png"
#define RES_MORE_BUTTON         "ui01_btn_more.png"
#define RES_RESTORE_BUTTON      "ui01_btn_restore.png"

#define TAG_QUIT_GAME_LAYER     123

HomeScene::~HomeScene()
{
    //Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool HomeScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    bool rcode = false;
    do {
        
        _uiLayer = Layer::create();
        addChild(_uiLayer);
        
        //back ground
        Sprite *background = Sprite::create(RES_BG);
        CC_BREAK_IF(!background);
        background->setPosition(Utils::getInstance()->getScreenCenter());
        _uiLayer->addChild(background);
        
        //logo3
        Sprite *logo = Sprite::create(RES_HOME_LOGO);
        CC_BREAK_IF(!logo);
        logo->setPosition(Vec2(Utils::SCREEN_WIDTH/2, Utils::SCREEN_HEIGHT*2/3+45));
        addChild(logo);
        
        //play
        auto play = ui::Button::create(RES_PLAY_BUTTON);
        CC_BREAK_IF(!play);
        namespaceST::STVisibleRect::setPosition(play, 480, 275);
        play->addTouchEventListener(CC_CALLBACK_2(HomeScene::touchDown, this));
        play->setTag(playTag);
        _uiLayer->addChild(play, 1);
        
        //setting
        auto setting = ui::Button::create(RES_SETTING_BUTTON);
        CC_BREAK_IF(!setting);
        namespaceST::STVisibleRect::setPosition(setting, 200, 80);
        setting->setTag(settingTag);
        setting->addTouchEventListener(CC_CALLBACK_2(HomeScene::touchDown, this));
        _uiLayer->addChild(setting);
        
        //more
        auto more = ui::Button::create(RES_MORE_BUTTON);
        namespaceST::STVisibleRect::setPosition(more, 480, 80);
        more->setTag(moreTag);
        more->addTouchEventListener(CC_CALLBACK_2(HomeScene::touchDown, this));
        _uiLayer->addChild(more, 1);
        
        //restore
        auto restore = ui::Button::create(RES_RESTORE_BUTTON);
        namespaceST::STVisibleRect::setPosition(restore, 760, 80);
        restore->setTag(restoreTag);
        restore->addTouchEventListener(CC_CALLBACK_2(HomeScene::touchDown, this));
        _uiLayer->addChild(restore);
        
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
        namespaceST::STVisibleRect::setPosition(setting, 287, 80);
        namespaceST::STVisibleRect::setPosition(more, 674, 80);
        restore->removeFromParent();
#endif
        
        AppManager::getInstance()->hideBannerAd();
        AppManager::getInstance()->setOnHomeEnter(true);
        rcode = true;
        
        auto listener = EventListenerKeyboard::create();
        listener->onKeyReleased = [&](EventKeyboard::KeyCode code, Event* event)
        {
            if(code == EventKeyboard::KeyCode::KEY_BACK && !_quit)
            {
                _quit = QuitGameLayer::create();
                _quit->setYesActionCallback(CC_CALLBACK_0(HomeScene::quitGame, this));
                _quit->setNoActionCallback([&](){
                    _quit = nullptr;
                });
                addChild(_quit, 2);
            }
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
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
    AppManager::getInstance()->hideBannerAd();
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
void HomeScene::touchDown(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type){
    if(ui::Widget::TouchEventType::ENDED != type) return;
    
    auto button = (Button*)pSender;
    int tag = button->getTag();
    
    switch (tag) {
        case playTag:
            HomeScene::playAction();
            break;
        case settingTag:
            HomeScene::settingAction();
            break;
        case moreTag:
            HomeScene::moreAction();
            break;
        case restoreTag:
            HomeScene::restoreAction();
            break;
        default:
            break;
    }
    
}

void HomeScene::playAction()
{
    AppManager::getInstance()->playButtonEffect();
    UserDefault::getInstance()->setBoolForKey("tipPlay", true);
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);

    log("Play Action!");
    auto scene = PackScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

void HomeScene::settingAction()
{
    log("Setting Action!");
    AppManager::getInstance()->playButtonEffect();

    SettingLayer *settingLayer = SettingLayer::create();
    settingLayer->setPosition(Point::ZERO);
                              
    addChild(settingLayer);
}

void HomeScene::moreAction()
{
    AppManager::getInstance()->playButtonEffect();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ParentalGateLayer *layer = ParentalGateLayer::create();
    layer->setDelegate(this, 1);
    layer->showInLayer(this);
#else
    STSystemFunction *sf = new STSystemFunction;
    sf->showMoreGame();
    CC_SAFE_DELETE(sf);
    log("More Action!");
#endif
}

void HomeScene::restoreAction()
{
    AppManager::getInstance()->playButtonEffect();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ParentalGateLayer *layer = ParentalGateLayer::create();
    layer->setDelegate(this, 2);
    layer->showInLayer(this);
#else
    log("restore Action!");
    
    _billing.setIABDelegate(this);
    _billing.restore();
#endif
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

void HomeScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    if (AppManager::getInstance()->isGoBackToHome()) {
    }
    AppManager::getInstance()->setIsGoBackToHome(false);
}


CB_END_NAMESPACE
