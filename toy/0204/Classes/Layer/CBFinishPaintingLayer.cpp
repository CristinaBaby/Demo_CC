//
//  CBFinishPaintingLayer.cpp
//  ColorBook
//
//  Created by maxiang on 4/27/15.
//
//

#include "CBFinishPaintingLayer.h"
#include "../Model/CBUtils.h"
#include "../Model/CBAppManager.h"
#include "../Layer/AdsLoadingLayer.h"
#include "AdLoadingLayerBase.h"

USING_NS_CC;
CB_BEGIN_NAMESPACE

#define TEXT_FINISH_PAINTING "Do you want to save your painting to\nphoto gallery and start another one?"
#define TEXT_FONT_SIZE       26
#define TEXT_FONT_NAME       "HELVETICA_NEUE.ttf"

#define RES_SAVE_BG          "ui05_save_box.png"
#define RES_SAVE_PICTURE_BG  "ui05_save_picture.png"

#define RES_BACK_BUTTON      "ui05_popup_btn_back.png"

#define RES_QUIT_BUTTON      "ui05_popup_btn_quit.png"
#define RES_QUIT_BUTTON_H    "ui05_popup_btn_quit.png"

#define RES_SAVE_BUTTON      "ui05_btn_save.png"
#define RES_SAVE_BUTTON_H    "ui05_btn_save.png"

FinishPaintingLayer::~FinishPaintingLayer()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

FinishPaintingLayer* FinishPaintingLayer::create(cocos2d::Image *image)
{
    FinishPaintingLayer *layer = new (std::nothrow)(FinishPaintingLayer);
    if (layer && layer->init(image))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool FinishPaintingLayer::init(cocos2d::Image *image)
{
    if (! LayerColor::initWithColor(Color4B(0, 0, 0, 200)))
    {
        return false;
    }
    
    bool rcode = false;
    do {
        //back
        _backButton = ui::Button::create(RES_BACK_BUTTON);
        _backButton->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
        _backButton->setPosition(Vec2(_backButton->getContentSize().width/5, Utils::SCREEN_HEIGHT - _backButton->getContentSize().width/5));
        _backButton->setTag(backTag);
        addChild(_backButton);
        
        //background
        _bgSprite = Sprite::create(RES_SAVE_BG);
        CC_BREAK_IF(!_bgSprite);
        _bgSprite->setPosition(Vec2(Utils::getInstance()->getScreenCenter()));
        addChild(_bgSprite);
        
        //save picture bg
        _saveBg = Sprite::create(RES_SAVE_PICTURE_BG);
        CC_BREAK_IF(!_saveBg);
        _saveBg->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        _saveBg->setPosition(Vec2(40, 30));
        _saveBg->setScale(0.9);
        _bgSprite->addChild(_saveBg);
        
        //create picture
        auto texture = new Texture2D();
        texture->initWithImage(image);
        texture->autorelease();
        auto picture = Sprite::createWithTexture(texture);
        if (picture)
        {
            //fix
            float scaleX = _saveBg->getContentSize().width / picture->getContentSize().width;
            float scaleY = _saveBg->getContentSize().height / picture->getContentSize().height;
            
            float scale = scaleX > scaleY ? scaleY : scaleX;
            picture->setScale(scale - 0.02/* bg have some mask */);
            picture->setPosition(Vec2(_saveBg->getContentSize().width/2, _saveBg->getContentSize().height/2));
            _saveBg->addChild(picture);
        }
        
        //text
        auto label = Label::createWithTTF(TEXT_FINISH_PAINTING, TEXT_FONT_NAME, TEXT_FONT_SIZE);
        CC_BREAK_IF(!label);
        label->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
        label->setPosition(Vec2(40, _bgSprite->getContentSize().height - 30));
        label->setTextColor(Color4B(152, 52, 0, 255));
        _bgSprite->addChild(label);
        
        //no button
        _quitButton = ui::Button::create(RES_QUIT_BUTTON);
        CC_BREAK_IF(!_quitButton);
        _quitButton->setTag(quitTag);
        _quitButton->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        _quitButton->setPosition(Vec2(345, _bgSprite->getContentSize().height/2+20));
        _bgSprite->addChild(_quitButton);
        
        //yes button
        _saveButton = ui::Button::create(RES_SAVE_BUTTON);
        CC_BREAK_IF(!_saveButton);
        _saveButton->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        _saveButton->setPosition(Vec2(345, 60));
        _saveButton->setTag(saveTag);
        _bgSprite->addChild(_saveButton);
        
        //touch event
        //set touch events
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [this](Touch *touch, Event *unused_event){
            return true;
        };
        
        _quitButton->addTouchEventListener(CC_CALLBACK_2(FinishPaintingLayer::onButton, this));
        _backButton->addTouchEventListener(CC_CALLBACK_2(FinishPaintingLayer::onButton, this));
        _saveButton->addTouchEventListener(CC_CALLBACK_2(FinishPaintingLayer::onButton, this));
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        AppManager::getInstance()->showBannerAd();
        
        rcode = true;
    } while (0);
    
    return rcode;
}

#pragma mark- Action methods

void FinishPaintingLayer::setQuitCallback(const std::function<void()>& callback)
{
    _quitCallback = callback;
}

void FinishPaintingLayer::setSaveCallback(const std::function<void()>& callback)
{
    _saveCallback = callback;
}

void FinishPaintingLayer::quitAction()
{
    AppManager::getInstance()->playButtonEffect();
    
    if (_quitCallback) {
        _quitCallback();
    }
    
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    removeFromParentAndCleanup(true);
}

void FinishPaintingLayer::saveAction()
{
    AppManager::getInstance()->playButtonEffect();

    if (_saveCallback) {
        _saveCallback();
    }
    
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    removeFromParentAndCleanup(true);
    xApp->showLoading(false);
}

void FinishPaintingLayer::backAction()
{
    AppManager::getInstance()->playButtonEffect();
    
    AppManager::getInstance()->hideBannerAd();

    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    removeFromParentAndCleanup(true);
}

#pragma mark- Touch events

void FinishPaintingLayer::onButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type){
    if(cocos2d::ui::Widget::TouchEventType::ENDED != type) return;
    
    auto widget = (Widget*)pSender;
    int tag = widget->getTag();
    
    switch (tag) {
        case quitTag:
            quitAction();
            break;
        case saveTag:
            saveAction();
            break;
        case backTag:
            backAction();
            break;
        default:
            break;
    }
}


CB_END_NAMESPACE
