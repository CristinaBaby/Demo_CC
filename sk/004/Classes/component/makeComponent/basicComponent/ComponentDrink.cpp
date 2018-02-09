//
//  ComponentDrink.cpp
//  BHUG1062
//
//  Created by maxiang on 3/23/16.
//
//

#include "ComponentDrink.h"

ComponentDrink::ComponentDrink():
_drinkSprite(nullptr),
_drinkMask(nullptr),
_drinkClippingNode(nullptr),
_isDrinking(false),
_drinkSpeed(1.0),
_eventListenerTouchOneByOne(nullptr),
_triggerDrinkTime(0.2),
_touchingTime(0.0),
_isTouching(false),
_isDrinkOver(false),
_isAutoDrink(false)
{
    Director::getInstance()->getScheduler()->schedule(schedule_selector(ComponentDrink::drinkTimer), this, 1.0/60.0, false);
}

ComponentDrink::~ComponentDrink()
{
    CC_SAFE_RELEASE_NULL(_drinkClippingNode);
    CC_SAFE_RELEASE_NULL(_drinkMask);

    Director::getInstance()->getScheduler()->unschedule(schedule_selector(ComponentDrink::drinkTimer), this);
}

ComponentDrink *ComponentDrink::create(Sprite *drinkMask)
{
    auto ret = new (std::nothrow) ComponentDrink();
    if (ret)
    {
        ret->init(drinkMask);
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ComponentDrink::init(Sprite *drinkMask)
{
    if (!ComponentBase::init())
        return false;
    
    CCASSERT(drinkMask, "* ComponentDrink: 遮罩不能为空! *");
    
    _drinkMask = drinkMask;
    _drinkMask->retain();
    
    _finishDrinkHeight = -_drinkMask->getContentSize().height + 3.0;
    
    if (!_drinkClippingNode)
    {
        _drinkClippingNode = ClippingNode::create();
        _drinkClippingNode->retain();
    }
    
    _triggerDrinkArea = Rect(0.0, 0.0, Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
    return true;
}

void ComponentDrink::onAdd()
{
    ComponentBase::onAdd();
    
    if (!_owner || !_drinkMask)
        return;
    
    if (!_drinkClippingNode->getParent())
    {
        _owner->addChild(_drinkClippingNode);
        _drinkClippingNode->setStencil(_drinkMask);
        _drinkClippingNode->setAlphaThreshold(0.9);
    }
}

void ComponentDrink::reset()
{
    if (!_drinkSprite)
        return;
    
    _isDrinkOver = false;
    _isDrinking = false;
    _isTouching = false;
    _touchingTime = 0.0;
    _drinkSprite->setPosition(_drinkSpriteOriPosition);
}

void ComponentDrink::start()
{
    CCASSERT(_owner, "* ComponentDrink: owner不能为空! *");
    
    if (_isDrinkOver)
    {
        CCLOG("* ComponentDrink: 已经喝完, 请重置 *");
        return;
    }
    
    if (!_isAutoDrink)
    {
        addTouchEventListener();
    }
    else
    {
        _isDrinking = true;
        if (onStartDrinkingCallBack)
            onStartDrinkingCallBack();
    }
}

void ComponentDrink::stop()
{
    CCASSERT(_owner, "* ComponentDrink: owner不能为空! *");
    
    if (_isDrinking)
    {
        removeTouchEventListener();
        if (onStopDrinkingCallBack)
            onStopDrinkingCallBack();
    }
    
    _touchingTime = 0.0;
    _isDrinking = false;
}

void ComponentDrink::setEnableAutoDrink(bool enable)
{
    _isAutoDrink = enable;
    if (_isAutoDrink)
        removeTouchEventListener();
    else
        addTouchEventListener();
}

void ComponentDrink::setDrinkSprite(Sprite *drinkSprite)
{
    CCASSERT(drinkSprite, "* ComponentDrink: drinkSprite不能为空! *");
    if (_drinkSprite)
        _drinkSprite->removeFromParent();
    
    _drinkSprite = drinkSprite;
    _drinkClippingNode->addChild(_drinkSprite);
    _drinkSpriteOriPosition = _drinkSprite->getPosition();
}

void ComponentDrink::drinkTimer(float time)
{
    CCASSERT(_triggerDrinkTime > 0, "* ComponentDrink: _triggerDrinkTime必须大于0 *");
    
    if (_isTouching && !_isDrinking &&
        !_isAutoDrink && !_isDrinkOver)
    {
        _touchingTime += time;
        if (_touchingTime >= _triggerDrinkTime)
        {
            _isDrinking = true;
            if (onStartDrinkingCallBack)
                onStartDrinkingCallBack();
            return;
        }
    }
    
    if (_isDrinking)
    {
        drink();
    }
}

void ComponentDrink::drink()
{
    CCASSERT(_drinkSpeed > 0, "* ComponentDrink: _drinkSpeed必须大于0 *");
    
    float x = _drinkSprite->getPosition().x;
    float y = _drinkSprite->getPosition().y;
    
    _drinkSprite->setPosition(Vec2(x, y - _drinkSpeed));
    
    if (y < -_finishDrinkHeight)
    {
        finishDrink();
    }
}

void ComponentDrink::finishDrink()
{
    _isDrinkOver = true;
    _touchingTime = 0.0;
    _isDrinking = false;
    
    removeTouchEventListener();
    
    if (onFinishDrinkCallBack)
        onFinishDrinkCallBack();
}

void ComponentDrink::addTouchEventListener()
{
    if (_eventListenerTouchOneByOne || !getOwner())
        return;
    
    _eventListenerTouchOneByOne = EventListenerTouchOneByOne::create();
    _eventListenerTouchOneByOne->onTouchBegan = CC_CALLBACK_2(ComponentDrink::onTouchBegan, this);
    _eventListenerTouchOneByOne->onTouchMoved = CC_CALLBACK_2(ComponentDrink::onTouchMoved, this);
    _eventListenerTouchOneByOne->onTouchEnded = CC_CALLBACK_2(ComponentDrink::onTouchEnded, this);
    _eventListenerTouchOneByOne->onTouchCancelled = CC_CALLBACK_2(ComponentDrink::onTouchCancelled, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_eventListenerTouchOneByOne, getOwner());
    _eventListenerTouchOneByOne->setSwallowTouches(true);
}

void ComponentDrink::removeTouchEventListener()
{
    if (_eventListenerTouchOneByOne)
    {
        Director::getInstance()->getEventDispatcher()->removeEventListener(_eventListenerTouchOneByOne);
        _eventListenerTouchOneByOne = nullptr;
    }
}

bool ComponentDrink::onTouchBegan(Touch* touch, Event* event)
{
    if(_triggerDrinkArea.containsPoint(touch->getLocation()))
    {
        _isTouching = true;
        return true;
    }
    return false;
}

void ComponentDrink::onTouchMoved(Touch* touch, Event* event)
{
    
}

void ComponentDrink::onTouchEnded(Touch* touch, Event* event)
{
    _isTouching = false;
    stop();
}

void ComponentDrink::onTouchCancelled(Touch* touch, Event* event)
{
    _isTouching = false;
    stop();
}
