//
//  CBParentalGateLayer.cpp
//  ColorBook
//
//  Created by maxiang on 4/30/15.
//
//

#include "CBParentalGateLayer.h"
#include "ui/CocosGUI.h"
#include "../Model/CBAppManager.h"

USING_NS_CC;
using namespace ui;


#define RES_BOX_BG              "box.png"
#define RES_BUTTON_CLOSE        "close.png"
#define RES_BUTTON_AIRPLANE     "airplane.png"
#define RES_BUTTON_FIREBALLOON  "fireballoon.png"
#define RES_BUTTON_SHIP         "ship.png"

#define FONT_NAME   "HELVETICA_NEUE.ttf"

void ParentalGateLayer::setDelegate(ParentalGateProtocol *delegate, int tag)
{
    _delegate = delegate;
    _tag = tag;
    scheduleOnce(schedule_selector(ParentalGateLayer::update),0);
}

ParentalGateLayer* ParentalGateLayer::create()
{
    ParentalGateLayer *layer = new (std::nothrow)(ParentalGateLayer);
    if (layer && layer->init())
    {
        layer->_delegate = nullptr;
        layer->_callback = 0;
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

void ParentalGateLayer::update(float delta)
{
    if (_delegate)
        _delegate->parentalGatePass(_tag);
    if (_callback)
        _callback();
    close();
}

bool ParentalGateLayer::init()
{
    Color4B color = {0, 0, 0, 225};
    if (!LayerColor::initWithColor(color))
        return false;
    
    do {
        float width  = CCDirector::getInstance()->getVisibleSize().width;
        float height = CCDirector::getInstance()->getVisibleSize().height;
        
        //background image
        _bgSprite = Sprite::create(RES_BOX_BG);
        CC_BREAK_IF(!_bgSprite);
        _bgSprite->setPosition(Point(width/2, height/2));
        addChild(_bgSprite);
        
        //tap labe
        Label *label1 = Label::createWithTTF("Tap the", FONT_NAME, 30);
        label1->setAnchorPoint(Point(0.f, 0.5f));
        label1->setPosition(Point(_bgSprite->getContentSize().width*0.05f, _bgSprite->getContentSize().height/3*2));
        _bgSprite->addChild(label1);
        
        //value
        char *valueString = (char *)malloc(20);
        generateRandomValue(*valueString);
        Label *valueLable = Label::createWithTTF(valueString, FONT_NAME, 30);
        valueLable->setAnchorPoint(Point(0.f, 0.5f));
        valueLable->setPosition(Point(label1->getPosition().x+label1->getContentSize().width, label1->getPosition().y));
        Color3B color = {248, 215, 58};
        valueLable->setColor(color);
        _bgSprite->addChild(valueLable);
        delete valueString;
        
        //times label
        Label *label2 = Label::createWithTTF("times", FONT_NAME, 30);
        label2->setAnchorPoint(Point(0.f, 0.5f));
        label2->setPosition(Point(valueLable->getPosition().x+valueLable->getContentSize().width, valueLable->getPosition().y));
        _bgSprite->addChild(label2);
        
        //close button
        auto closeButton = Button::create(RES_BUTTON_CLOSE);
        CC_BREAK_IF(!closeButton);
        closeButton->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                this->closeAction();
            }
        });
        closeButton->setPosition(Vec2(_bgSprite->getContentSize().width - 30, _bgSprite->getContentSize().height - 30));
        
        //item1
        auto item1Button = Button::create(RES_BUTTON_SHIP);
        CC_BREAK_IF(!item1Button);
        item1Button->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                this->item1Action();
            }
        });
        item1Button->setPosition(Vec2(95, 135));
        
        //item3
        auto item2Button = Button::create(RES_BUTTON_AIRPLANE);
        CC_BREAK_IF(!item2Button);
        item2Button->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                this->item2Action();
            }
        });
        item2Button->setPosition(Vec2(275, 135));
        
        //item3
        auto item3Button = Button::create(RES_BUTTON_FIREBALLOON);
        CC_BREAK_IF(!item3Button);
        item3Button->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                this->item3Action();
            }
        });
        item3Button->setPosition(Vec2(455, 135));
        
        _bgSprite->addChild(closeButton);
        _bgSprite->addChild(item1Button);
        _bgSprite->addChild(item2Button);
        _bgSprite->addChild(item3Button);

        _tapCount = 0;
        
        //touch event listen
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [](Touch* touch, Event* event) {
            
            return true;
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        return true;
    } while (0);
    
    return false;
}


void ParentalGateLayer::closeAction()
{
    close();
}

void ParentalGateLayer::item1Action()
{
    chooseItem(ItemType_Ship);
}

void ParentalGateLayer::item2Action()
{
    chooseItem(ItemType_AirPlane);
}

void ParentalGateLayer::item3Action()
{
    chooseItem(ItemType_AirBall);
}

void ParentalGateLayer::setCallback(const parentalGateCallback& callback)
{
    _callback = callback;
}

void ParentalGateLayer::chooseItem(ItemType type)
{
    if (type==_itemType)
        _tapCount++;
    else
        _tapCount = 0;
    
    if (_tapCount == _itemTapTimes)
    {
        if (_delegate)
            _delegate->parentalGatePass(_tag);
        if (_callback)
            _callback();
        close();
    }
}

void ParentalGateLayer::showInLayer(Scene *scene)
{
    scene->addChild(this, 200);
    this->setScale(0.0);
    
    //animation show
    ScaleTo *scaleTo = ScaleTo::create(0.2f, 1.0f);
    this->runAction(scaleTo);
}

void ParentalGateLayer::generateRandomValue(char& valueString)
{
    _itemType = (ItemType)(arc4random()%3);
    _itemTapTimes = arc4random()%2+2;
    
    std::string typeString;
    
    if (_itemType==ItemType_Ship) typeString = "Ship";
    if (_itemType==ItemType_AirPlane) typeString = "Airplane";
    if (_itemType==ItemType_AirBall) typeString = "Hot Air Balloon";
    
    sprintf(&valueString, " %s %d ", typeString.c_str(), _itemTapTimes);
}

void ParentalGateLayer::close()
{
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    removeFromParent();
}

