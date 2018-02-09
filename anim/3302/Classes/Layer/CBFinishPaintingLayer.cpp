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

USING_NS_CC;
CB_BEGIN_NAMESPACE

#define TEXT_FINISH_PAINTING "If you wish to save your painting to\nthe photo gallery or just quit?"
#define TEXT_FONT_SIZE       22
#define TEXT_FONT_NAME       "Helvetica_Rounded_LT_Black.ttf"

#define RES_SAVE_BG          "ui05_save_box.png"
#define RES_SAVE_PICTURE_BG  "ui05_save_picture.png"

#define RES_BACK_BUTTON      "ui07_public_back.png"
#define RES_BACK_BUTTON_H    "ui07_public_back_h.png"

#define RES_QUIT_BUTTON      "ui05_popup_btn_quit.png"
#define RES_QUIT_BUTTON_H    "ui05_popup_btn_quit_h.png"

#define RES_SAVE_BUTTON      "ui05_btn_save.png"
#define RES_SAVE_BUTTON_H    "ui05_btn_save_h.png"

FinishPaintingLayer::~FinishPaintingLayer()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

FinishPaintingLayer* FinishPaintingLayer::create(const std::string& pictureName)
{
    FinishPaintingLayer *layer = new (std::nothrow)(FinishPaintingLayer);
    if (layer && layer->init(pictureName))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
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
        _backButton = Sprite::create(RES_BACK_BUTTON);
        _backButton->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
        _backButton->setPosition(Vec2(_backButton->getContentSize().width/5, Utils::SCREEN_HEIGHT - _backButton->getContentSize().width/5));
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
        _saveBg->setPosition(Vec2(30, 30));
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
        label->setPosition(Vec2(35, _bgSprite->getContentSize().height - 80));
        label->setTextColor(Color4B(152, 52, 0, 255));
        _bgSprite->addChild(label);
        
        //no button
        _quitButton = Sprite::create(RES_QUIT_BUTTON);
        CC_BREAK_IF(!_quitButton);
        _quitButton->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        _quitButton->setPosition(Vec2(340, _bgSprite->getContentSize().height/2));
        _bgSprite->addChild(_quitButton);
        
        //yes button
        _saveButton = Sprite::create(RES_SAVE_BUTTON);
        CC_BREAK_IF(!_saveButton);
        _saveButton->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        _saveButton->setPosition(Vec2(340, 34));
        _bgSprite->addChild(_saveButton);
        
        //touch event
        //set touch events
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = CC_CALLBACK_2(FinishPaintingLayer::onTouchBegan, this);
        touchEventListener->onTouchMoved = CC_CALLBACK_2(FinishPaintingLayer::onTouchMoved, this);
        touchEventListener->onTouchEnded = CC_CALLBACK_2(FinishPaintingLayer::onTouchEnded, this);
        touchEventListener->onTouchCancelled = CC_CALLBACK_2(FinishPaintingLayer::onTouchCancelled, this);
        
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        AppManager::getInstance()->setBannerAdVisible(true);
        
        rcode = true;
    } while (0);
    
    return rcode;
}

bool FinishPaintingLayer::init(const std::string& pictureName)
{
    if (! LayerColor::initWithColor(Color4B(0, 0, 0, 200)))
    {
        return false;
    }
    
    bool rcode = false;
    do {
        _pictureName = pictureName;
        
        //back
        _backButton = Sprite::create(RES_BACK_BUTTON);
        _backButton->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
        _backButton->setPosition(Vec2(_backButton->getContentSize().width/5, Utils::SCREEN_HEIGHT - _backButton->getContentSize().width/5));
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
        _saveBg->setPosition(Vec2(30, 30));
        _saveBg->setScale(0.9);
        _bgSprite->addChild(_saveBg);
        
        //create picture
        auto picture = Sprite::create(pictureName);
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
        label->setPosition(Vec2(35, _bgSprite->getContentSize().height - 80));
        label->setTextColor(Color4B(152, 52, 0, 255));
        _bgSprite->addChild(label);
        
        //no button
        _quitButton = Sprite::create(RES_QUIT_BUTTON);
        CC_BREAK_IF(!_quitButton);
        _quitButton->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        _quitButton->setPosition(Vec2(320, _bgSprite->getContentSize().height/2));
        _bgSprite->addChild(_quitButton);

        //yes button
        _saveButton = Sprite::create(RES_SAVE_BUTTON);
        CC_BREAK_IF(!_saveButton);
        _saveButton->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        _saveButton->setPosition(Vec2(320, 34));
        _bgSprite->addChild(_saveButton);
        
        //touch event
        //set touch events
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = CC_CALLBACK_2(FinishPaintingLayer::onTouchBegan, this);
        touchEventListener->onTouchMoved = CC_CALLBACK_2(FinishPaintingLayer::onTouchMoved, this);
        touchEventListener->onTouchEnded = CC_CALLBACK_2(FinishPaintingLayer::onTouchEnded, this);
        touchEventListener->onTouchCancelled = CC_CALLBACK_2(FinishPaintingLayer::onTouchCancelled, this);
        
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        AppManager::getInstance()->setBannerAdVisible(true);
        
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
}

void FinishPaintingLayer::backAction()
{
    AppManager::getInstance()->playButtonEffect();
    
    AppManager::getInstance()->setBannerAdVisible(false);

    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    removeFromParentAndCleanup(true);
}

#pragma mark- Touch events

bool FinishPaintingLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
    Point point = _bgSprite->convertToNodeSpace(touch->getLocation());
    
    if (_quitButton->getBoundingBox().containsPoint(point))
    {
        _quitButton->setTexture(RES_QUIT_BUTTON_H);
        return true;
    }
    else if(_saveButton->getBoundingBox().containsPoint(point))
    {
        _saveButton->setTexture(RES_SAVE_BUTTON_H);
        return true;
    }
    
    point = convertToNodeSpace(touch->getLocation());
    if(_backButton->getBoundingBox().containsPoint(point))
    {
        _backButton->setTexture(RES_BACK_BUTTON_H);
    }
    return true;
}

void FinishPaintingLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
    Point point = _bgSprite->convertToNodeSpace(touch->getLocation());
    
    if (_quitButton->getBoundingBox().containsPoint(point))
    {
        _quitButton->setTexture(RES_QUIT_BUTTON_H);
        return;
    }
    else if(_saveButton->getBoundingBox().containsPoint(point))
    {
        _saveButton->setTexture(RES_SAVE_BUTTON_H);
        return;
    }
    
    point = convertToNodeSpace(touch->getLocation());
    if(_backButton->getBoundingBox().containsPoint(point))
    {
        _backButton->setTexture(RES_BACK_BUTTON_H);
        return;
    }
    
    _quitButton->setTexture(RES_QUIT_BUTTON);
    _saveButton->setTexture(RES_SAVE_BUTTON);
    _backButton->setTexture(RES_BACK_BUTTON);
}

void FinishPaintingLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
    Point point = _bgSprite->convertToNodeSpace(touch->getLocation());
    
    if (_quitButton->getBoundingBox().containsPoint(point))
    {
        _quitButton->setTexture(RES_QUIT_BUTTON);
        quitAction();
        return;
    }
    else if(_saveButton->getBoundingBox().containsPoint(point))
    {
        _saveButton->setTexture(RES_SAVE_BUTTON);
        saveAction();
        return;
    }
    
    point = convertToNodeSpace(touch->getLocation());
    if(_backButton->getBoundingBox().containsPoint(point))
    {
        _backButton->setTexture(RES_BACK_BUTTON);
        backAction();
    }
}

void FinishPaintingLayer::onTouchCancelled(Touch *touch, Event *unused_event)
{
    _quitButton->setTexture(RES_QUIT_BUTTON);
    _saveButton->setTexture(RES_SAVE_BUTTON);
    _backButton->setTexture(RES_BACK_BUTTON);
}


CB_END_NAMESPACE