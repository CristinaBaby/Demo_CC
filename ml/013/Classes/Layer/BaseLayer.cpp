//
//  BaseLayer.cpp
//  Make
//
//  Created by QAMAC01 on 15-1-19.
//
//

#include "BaseLayer.h"
#include "KSVisibleRect.h"
#include "Config.h"
#include "GameMaster.h"
#include "IAPManager.h"
bool BaseLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    shouldShowCross_ = true;
    
    this -> initBtn();
    this -> initListener();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    log("aaaaaaaaa");
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(BaseLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
#endif

    
    return true;
}

void BaseLayer::initListener()
{
    listener_ = EventListenerTouchOneByOne::create();
}

void BaseLayer::setBackground(std::string pzFileName)
{
    bg_ = Sprite::create(pzFileName);
    bg_-> setPosition(KSVisibleRect::getPosition(KSVisibleRect::getDesignSize().width/2, KSVisibleRect::getDesignSize().height/2));
    addChild(bg_);
    

}

void BaseLayer::setHint(std::string pzFileName)
{
    if (hintSprite_)
    {
        hintSprite_ -> setVisible(true);
        hintSprite_ -> setTexture(pzFileName);
    }
}


void BaseLayer::initBtn()
{
    
    float yDistance =  100 ;
    
    nextBtn_ = ui::Button::create("button/btn_next.png");
    nextBtn_ -> setPosition(KSVisibleRect::getPosition(10, 480,kBorderRight,kBorderNone));
    nextBtn_ -> setAnchorPoint(Vec2(1, 0.5));
    nextBtn_ -> addTouchEventListener(CC_CALLBACK_2(BaseLayer::touchEvent, this));
    addChild(nextBtn_, 99, kBtnNext);

    newOneBtn_ = ui::Button::create("button/btn_home.png");
    newOneBtn_ -> setPosition(KSVisibleRect::getPosition(10, 10,kBorderRight,kBorderTop));
    newOneBtn_ -> setAnchorPoint(Vec2(1, 1));
    newOneBtn_ -> addTouchEventListener(CC_CALLBACK_2(BaseLayer::touchEvent, this));
    addChild(newOneBtn_, 99, kBtnHome);

    
    resetBtn_ = ui::Button::create("button/btn_reset.png");
    resetBtn_ -> setAnchorPoint(Vec2(1, 0));
    resetBtn_ -> setPosition(KSVisibleRect::getPosition(10, 200+yDistance,kBorderRight,kBorderBottom));
    resetBtn_ -> addTouchEventListener(CC_CALLBACK_2(BaseLayer::touchEvent, this));
    addChild(resetBtn_, 99, kBtnReset);

    resetGameBtn_ = ui::Button::create("button/btn_reset.png");
    resetGameBtn_ -> setPosition(Vec2(200, 300));
    resetGameBtn_ -> addTouchEventListener(CC_CALLBACK_2(BaseLayer::touchEvent, this));
    addChild(resetGameBtn_, 99, kBtnResetGame);

    backBtn_ = ui::Button::create("button/btn_back.png");
    backBtn_ -> setAnchorPoint(Vec2(0, 0.5));
    backBtn_ -> setPosition(KSVisibleRect::getPosition(10, 480,kBorderLeft,kBorderNone));
    backBtn_ -> addTouchEventListener(CC_CALLBACK_2(BaseLayer::touchEvent, this));
    addChild(backBtn_, 99, kBtnBack);
    
    favBtn_ = ui::Button::create("button/btn_fav.png");
    favBtn_ -> setAnchorPoint(Vec2(1, 1));
    favBtn_ -> setPosition(KSVisibleRect::getPosition(10+110, 10,kBorderRight,kBorderTop));
    favBtn_ -> addTouchEventListener(CC_CALLBACK_2(BaseLayer::touchEvent, this));
    addChild(favBtn_, 99, kBtnFav);

    hintSprite_ = Sprite::create();
    hintSprite_ -> setPosition(KSVisibleRect::getPosition(320, 67+yDistance));
    this -> addChild(hintSprite_,99);
    
    moreBtnInShare_ = ui::Button::create("button/btn_more.png");
    moreBtnInShare_ -> setAnchorPoint(Vec2(1, 0));
    moreBtnInShare_ -> setPosition(KSVisibleRect::getPosition(10, 10+yDistance,kBorderRight,kBorderBottom));
    moreBtnInShare_ -> addTouchEventListener(CC_CALLBACK_2(BaseLayer::touchEvent, this));
    addChild(moreBtnInShare_, 99, kBtnMore);

    resetGameBtn_ -> setVisible(false);
    nextBtn_ -> setVisible(false);
    resetBtn_ -> setVisible(false);
    backBtn_ -> setVisible(false);
    favBtn_ -> setVisible(false);
    newOneBtn_ -> setVisible(false);
    hintSprite_ -> setVisible(false);
    moreBtnInShare_ -> setVisible(false);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    nextBtn_ -> setPosition(KSVisibleRect::getPosition(10, 10+100,kBorderRight,kBorderBottom));
//    backBtn_ -> setPosition(KSVisibleRect::getPosition(10, 10+100,kBorderLeft,kBorderBottom));

#endif
    
}

void BaseLayer::refreshBtn()
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    float yDistance =  IAPManager::getInstance() -> isShowAds()? 200 : 100;
    resetBtn_ -> setPosition(KSVisibleRect::getPosition(10, 60+yDistance,kBorderRight,kBorderBottom));
    hintSprite_ -> setPosition(KSVisibleRect::getPosition(320, 67+yDistance));

    
#endif

}


void BaseLayer::touchEvent(Ref *obj,ui::Widget::TouchEventType type)
{
    ui::Button *btn = dynamic_cast<ui::Button *>(obj);
    
    switch (type)
    {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            
            break;
            
        case ui::Widget::TouchEventType::ENDED:
            
            switch (btn -> getTag())
        {
            case kBtnNext:
            {
                btn -> setVisible(false);
                this -> nextBtnClicked();
                CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/next.mp3");
            }
                break;
            case kBtnMore:
            {
                GameMaster::getInstance() -> goToMoreGame();
            }
                break;

            case kBtnReset:
            {
                Dialog* dialog= Dialog::create(Size(440,327), (void*)kDialogReset, Dialog::DIALOG_TYPE_NEGATIVE);
                dialog -> setAnchorPoint(Vec2(0,0));
                //                    STVisibleRect::setPosition(dialog, -size.width/2, -size.height/2);
                dialog -> setPosition(KSVisibleRect::getPosition(0,0, kBorderLeft ,kBorderBottom));
                dialog->setContentText("Do you want to reset your Snow Cone?");
                dialog->setPositiveBtnText("");
                dialog->setNegativeBtnText("");
                dialog->setCallback(this);
                addChild(dialog, 9999,129);

            }
                break;
            case kBtnResetGame:
            {
                btn -> setVisible(false);
                this -> resetGameBtnClicked();
            }
                break;
            case kBtnBack :
            {
                Director::getInstance() -> popScene();
                CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/previouspage.mp3");
                btn -> setEnabled(false);
            }
                
                break;
            case kBtnShop:
            {
                //                GameMaster::getInstance() -> goToShopLayer();
                
            }
                break;
            case kBtnFav:
            {
                Dialog* dialog= Dialog::create(Size(440,327), (void*)kDialogFav, Dialog::DIALOG_TYPE_NEGATIVE);
                dialog -> setAnchorPoint(Vec2(0,0));
                //                    STVisibleRect::setPosition(dialog, -size.width/2, -size.height/2);
                dialog -> setPosition(KSVisibleRect::getPosition(0,0, kBorderLeft ,kBorderBottom));
                dialog->setContentText("Do you want to add your Snow Cone to favorite?");
                dialog->setPositiveBtnText("");
                dialog->setNegativeBtnText("");
                dialog->setCallback(this);
                addChild(dialog, 9999,129);            }
                break;
            case kBtnHome:
            {
                Dialog* dialog= Dialog::create(Size(440,327), (void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE);
                dialog -> setAnchorPoint(Vec2(0,0));
                //                    STVisibleRect::setPosition(dialog, -size.width/2, -size.height/2);
                dialog -> setPosition(KSVisibleRect::getPosition(0,0, kBorderLeft ,kBorderBottom));
                dialog->setContentText("Are you sure you want to start over?");
                dialog->setPositiveBtnText("");
                dialog->setNegativeBtnText("");
                dialog->setCallback(this);
                addChild(dialog, 9999,129);
                
            }
                break;
            case kBtnReEat:
            {
                this -> reeatClicked();
            }
                break;
            case kBtnPre:
            {
                this -> preBtnClicked();
            }

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

#pragma mark dialog
void BaseLayer::onNegativeClick(void*)
{
    
}

void BaseLayer::onPositiveClick(void* type)
{
    
    int lType = (uintptr_t)type;
    if (lType == kDialogReturnHome)
    {
        this -> newOneBtnClicked();
        CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/next.mp3");
    }
    if (lType == kDialogReset)
    {
        this -> resetBtnClicked();
        CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/reset.mp3");
    }
    if (lType == kDialogFav)
    {
        this -> favBtnClicked();
        CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/reset.mp3");
    }
    if (lType == kDialogReturnHomeBack)
    {
        CocosDenshion::SimpleAudioEngine::getInstance() -> stopAllEffects();
        if (IAPManager::getInstance()->isShowAds())
        {
            AdsLoadingLayer::showLoading<AdsLoadingLayer>(true);
            AdsLoadingLayer::loadingDoneCallback =[=]
            {
                GameMaster::getInstance() -> goToHomeLayer();
            };
        }
        else
        {
            GameMaster::getInstance() -> goToHomeLayer();
        }

        CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/next.mp3");
        
        
    }

}

void BaseLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    
    CCLOG("%d", keyCode);
    
    if (getChildByTag(129))
    {
        
        this -> removeChildByTag(129);
        return;
    }
    
    
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE )
    {
        Dialog* dialog= Dialog::create(Size(440,327), (void*)kDialogReturnHomeBack, Dialog::DIALOG_TYPE_NEGATIVE);
        dialog->setContentText("Are you sure you want to start over?");
        dialog->setPositiveBtnText("");
        dialog->setNegativeBtnText("");
        dialog->setCallback(this);
        dialog->setFuncType(kDialogReturnHome);
        dialog -> setPosition(KSVisibleRect::getPosition(0,0, kBorderLeft ,kBorderBottom));
        addChild(dialog, 9999,129);
    }
    
}

void BaseLayer::onEnter()
{
    Layer::onEnter();
    this -> refreshBtn();
}

void BaseLayer::onExit()
{
    Node::onExit();
    Director::getInstance()->getTextureCache() -> removeUnusedTextures();
    Director::getInstance()->getTextureCache() -> removeAllTextures();

}