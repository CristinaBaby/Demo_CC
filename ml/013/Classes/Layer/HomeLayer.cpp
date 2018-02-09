//
//  HomeLayer.cpp
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-7-31.
//
//

#include "HomeLayer.h"
#include "GameMaster.h"
#include "KSVisibleRect.h"
#include "AudioController.h"
#include "Config.h"
#include "GameMaster.h"
//#include "ShopLayer.h"
//#include "STVisibleRect.h"
#include "STSystemFunction.h"
#include "SimpleAudioEngine.h"
#include "IAPManager.h"
#include "SettingView.h"
#include "AdsLoadingLayer.h"
#include "Analytics.h"

Scene* HomeLayer::scene()
{
    Scene *pScene = Scene::create();
    pScene -> addChild(HomeLayer::create());
    return pScene;

}

bool HomeLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //    ui初始化
    initWithUI();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(HomeLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
#endif

    return true;
}

void HomeLayer::initWithUI()
{
    this -> initBackground();
    this -> initAnimation();
    this -> initButton();
}



void HomeLayer::initButton()
{
    auto playBtn = ui::Button::create("start/btn_play.png");
    playBtn -> setPosition(KSVisibleRect::getPosition(319.f, 192.f));
    playBtn -> addTouchEventListener(CC_CALLBACK_2(HomeLayer::touchEvent, this));
    addChild(playBtn, 20, kBtnPlay);
    playBtn -> setOpacity(0);
    
    auto scale = Sequence::create(ScaleTo::create(0.5, 1.1),ScaleTo::create(0.5, 1), NULL);
    playBtn -> runAction(RepeatForever::create(scale));
    
    auto moreBtn = ui::Button::create("start/btn_more.png");
    moreBtn -> setAnchorPoint(Vec2(1, 0));
    moreBtn -> setPosition(KSVisibleRect::getPosition(60, 60-200,kBorderRight,kBorderBottom));
    moreBtn -> addTouchEventListener(CC_CALLBACK_2(HomeLayer::touchEvent, this));
    addChild(moreBtn, 20, kBtnMore);

    auto favBtn = ui::Button::create("start/btn_favorits.png");
    favBtn -> setAnchorPoint(Vec2(0, 0));
    favBtn -> setPosition(KSVisibleRect::getPosition(60, 60-200,kBorderLeft,kBorderBottom));
    favBtn -> addTouchEventListener(CC_CALLBACK_2(HomeLayer::touchEvent, this));
    addChild(favBtn, 20, kBtnFav);
    
    auto policyBtn = ui::Button::create("start/btn_pp.png");
    policyBtn -> setAnchorPoint(Vec2(0, 0));
    policyBtn -> setPosition(KSVisibleRect::getPosition(20, 60,kBorderLeft,kBorderTop));
    policyBtn -> addTouchEventListener(CC_CALLBACK_2(HomeLayer::touchEvent, this));
    addChild(policyBtn, 20, kBtnPolicy);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    auto shopBtn = ui::Button::create("start/btn_shopping.png");
    shopBtn -> setAnchorPoint(Vec2(1, 0));
    shopBtn -> setPosition(KSVisibleRect::getPosition(60, 60-200,kBorderRight,kBorderBottom));
    shopBtn -> addTouchEventListener(CC_CALLBACK_2(HomeLayer::touchEvent, this));
    addChild(shopBtn, 20, kBtnShop);
    

    playBtn -> setPosition(KSVisibleRect::getPosition(319.f, 292.f+50));
    
    shopBtn -> runAction(Sequence::create(
                                          DelayTime::create(2.5),
                                          MoveBy::create(0.2, Vec2(0, 200)),

                                          NULL));
    moreBtn -> setAnchorPoint(Vec2(0.5, 0));
    moreBtn -> setPosition(KSVisibleRect::getPosition(320, 60-200,kBorderNone,kBorderBottom));

#endif
    
    
    // button animation
    
    playBtn -> runAction(Sequence::create(
                                          DelayTime::create(2),
                                          FadeIn::create(0.5),
                                          NULL));
    favBtn -> runAction(Sequence::create(
                                         DelayTime::create(2.5),
                                         MoveBy::create(0.2, Vec2(0, 200)),
                                         NULL));
    moreBtn -> runAction(Sequence::create(
                                         DelayTime::create(2.5),
                                         MoveBy::create(0.2, Vec2(0, 200)),
                                          NULL));
}


void HomeLayer::initBackground()
{
    //    //background
    auto bg = Sprite::create("start/default_bg.png");
    bg-> setPosition(KSVisibleRect::getPosition(KSVisibleRect::getDesignSize().width/2, KSVisibleRect::getDesignSize().height/2));
    addChild(bg);

}

void HomeLayer::initAnimation()
{
    
    auto p = ParticleSystemQuad::create("particle/star_snow.plist");
    p -> setPosition(Vec2(320, 880));
    this -> addChild(p,1);

    
    
    auto snowcone = Sprite::create("start/start_snowcone.png");
    snowcone -> setPosition(KSVisibleRect::getPosition(320, 284+900));
    snowcone -> setScale(0.9);
    this -> addChild(snowcone,1);
    snowcone -> runAction(Sequence::create(
                                           MoveBy::create(1, Vec2(0, -900)),
                                                               NULL));
    
    
    
    
    auto logo = Sprite::create("start/logo.png");
    logo -> setAnchorPoint(Vec2(0.5, 1));
    logo -> setPosition(KSVisibleRect::getPosition(320, 50,kBorderNone,kBorderTop));
    this -> addChild(logo,3);
    logo -> setScale(0);
    
    
    
    logo->runAction(Sequence::create(
                                     DelayTime::create(1.8),
                                     CallFunc::create([&]
                                                      {
                                                          CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/start_logo.mp3");
                                                      }),

                                     ScaleTo::create(0.1, 10, 0.1),
                                     ScaleTo::create(0.1, 0.9, 0.9),
                                     NULL));


}


void HomeLayer::touchEvent(Ref *obj,Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    

    
    switch (type)
    {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:

            break;
            
        case Widget::TouchEventType::ENDED:
            
            switch (btn -> getTag())
        {
            case kBtnPlay:
            {
                btn -> setTouchEnabled(false);
                CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/start.mp3");
                
                if (IAPManager::getInstance()->isShowAds())
                {
                    AdsLoadingLayer::showLoading<AdsLoadingLayer>(false);
                    AdsLoadingLayer::loadingDoneCallback =[=]
                    {
                        GameMaster::getInstance() -> goToMakeIceLayer();
                        AdsManager::getInstance() -> showAds(kTypeBannerAds);
                    };
                }
                else
                {
                    GameMaster::getInstance() -> goToMakeIceLayer();
                }
            }
                
                break;
            case kBtnPolicy:
            {
                STSystemFunction sf;
                sf.openURL("https://www.makerlabs.net/privacy/");
                
            }
                break;
            case kBtnMore:
            {
                GameMaster::getInstance() -> goToMoreGame();

            }
                break;
            case kBtnFav :
            {
                GameMaster::getInstance() -> goToFavLayer();
            }
        
                break;
            case kBtnShop:
            {
//                GameMaster::getInstance() -> goToShopLayer();
                
                _shopSelView = ShopSelView::create();
                _shopSelView -> setPosition(KSVisibleRect::getPosition(0, 0));
                this -> addChild(_shopSelView,9999);

            }
                break;
            case kBtnSound:
            {
                
                auto setting = SettingView::create();
                setting -> setPosition(KSVisibleRect::getPosition(0, 0));
                addChild(setting, 999, 10);
            }
                break;
            default:
                break;
        }
            break;
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            
            break;
            
        default:
            break;
    }
    
}



void HomeLayer::onEnter()
{
    Node::onEnter();
    Director::getInstance()->getTextureCache() -> removeUnusedTextures();
    AdsManager::getInstance() -> removeAds(kTypeBannerAds);
    GameMaster::getInstance()->setHidebanner(true);

    

}

void HomeLayer::onExit()
{
    Node::onExit();
    Director::getInstance()->getTextureCache() -> removeUnusedTextures();
    GameMaster::getInstance()->setHidebanner(false);

}


#pragma mark dialog
void HomeLayer::onNegativeClick(void*)
{
    
}

void HomeLayer::onPositiveClick(void* type)
{
    
    int lType = (uintptr_t)type;
    if (lType == kDialogReturnHome)
    {
        Analytics::getInstance()->endSession();
        Director::getInstance()->end();
    }
    if (lType == kDialogReset)
    {
        
    }
    
}

void HomeLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    
    log("1");
    
    CCLOG("%d", keyCode);
    
    if (getChildByTag(129))
    {
        log("2");

        this -> removeChildByTag(129);
        return;
    }
//    if (getChildByTag(10))
//    {
//        log("3");
//
//        dynamic_cast<Layer* >(getChildByTag(10)) -> removeFromParent();
//        return;
//    }
    
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE )
    {
        log("4");

        Dialog* dialog= Dialog::create(Size(440,327), (void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE);
        dialog->setContentText("Are you sure you want to exit?");
        dialog->setPositiveBtnText("");
        dialog->setNegativeBtnText("");
        dialog->setCallback(this);
        dialog->setFuncType(kDialogReturnHome);
        dialog -> setPosition(KSVisibleRect::getPosition(0,0, kBorderLeft ,kBorderBottom));
        addChild(dialog, 9999,129);
    }
    
}


