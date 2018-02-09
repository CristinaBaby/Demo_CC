//
//  ComponentDecoration.cpp
//  
//
//  Created by maxiang on 4/25/16.
//
//

#include "ComponentDecoration.h"

ComponentDecoration::~ComponentDecoration()
{
    _frame->removeFromParent();
    _zoom->removeFromParent();
    _delete->removeFromParent();

    CC_SAFE_RELEASE_NULL(_frame);
    CC_SAFE_RELEASE_NULL(_zoom);
    CC_SAFE_RELEASE_NULL(_delete);

    Director::getInstance()->getScheduler()->unschedule(schedule_selector(ComponentDecoration::hideFrameTimer), this);
    Director::getInstance()->getEventDispatcher()->removeEventListener(_eventListenerTouchOneByOne);
}

bool ComponentDecoration::init()
{
    if (!ComponentBase::init())
        return false;
    
    _moveBoundingBox.setRect(0.0, 0.0, Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);

    return true;
}

void ComponentDecoration::onAdd()
{
    ComponentBase::onAdd();
    
    if (!_owner)
        return;
}

void ComponentDecoration::start()
{
    CCASSERT(_owner, "* ComponentEat: 请将组件添加到一个节点上! *");
    CCASSERT(_frame && _zoom && _delete, "* ComponentEat: 请先设置ui! *");

    float scalex = _owner->getContentSize().width / _frame->getContentSize().width;
    float scaley = _owner->getContentSize().height / _frame->getContentSize().height;
    float scale = scalex > scaley? scalex : scaley;
    _frame->setScale(scale + 0.4);
    _frame->setPosition(_owner->getContentSize()/2);
    _owner->addChild(_frame);
    
    Vec2 framePosition = _frame->getPosition();
    Vec2 bottomRightPoint = Vec2(framePosition.x + (_frame->getContentSize().width * _frame->getScale())/2, framePosition.y + (_frame->getContentSize().height * _frame->getScale())/2);
    _originalLenght = framePosition.distance(bottomRightPoint);
    
    _zoom->setPosition(Vec2(_frame->getContentSize().width, 0.0));
    _frame->addChild(_zoom);

    _delete->setPosition(Vec2(0.0, _frame->getContentSize().height));
    _frame->addChild(_delete);
    
    Director::getInstance()->getScheduler()->schedule(schedule_selector(ComponentDecoration::hideFrameTimer), this, 1.0, false);
    
    addTouchEventListener();
}

void ComponentDecoration::setMoveArea(const Rect& area)
{
    _moveBoundingBox = area;
}

void ComponentDecoration::setFrame(const std::string& frameFile)
{
    if (_frame)
        _frame->removeFromParent();
    CC_SAFE_RELEASE_NULL(_frame);
    _frame = Sprite::create(frameFile);
    _frame->retain();
}

void ComponentDecoration::setZoomButtonTexture(const std::string& zoomFile)
{
    if (_zoom)
        _zoom->removeFromParent();
    CC_SAFE_RELEASE_NULL(_zoom);
    _zoom = Sprite::create(zoomFile);
    _zoom->retain();
}

void ComponentDecoration::setDeleteButtonTexture(const std::string& deleteFile)
{
    if (_delete)
        _delete->removeFromParent();
    CC_SAFE_RELEASE_NULL(_delete);
    _delete = Sprite::create(deleteFile);
    _delete->retain();
}

#pragma mark- Hide Frame Timer

void ComponentDecoration::hideFrameTimer(float time)
{
    if (_pauseTimer)
        return;
    
    _noTouchTime++;
    if (_noTouchTime % 2 == 0 && _frame->isVisible())
    {
        setFrameVisible(false);
    }
}

void ComponentDecoration::setFrameVisible(bool visible)
{
    _frame->setVisible(visible);
    if (visible)
    {
        _pauseTimer = false;
    }
    else
    {
        _pauseTimer = true;
        _noTouchTime = 0;
    }
}

void ComponentDecoration::addTouchEventListener()
{
    if (_eventListenerTouchOneByOne || !getOwner())
        return;
    
    _eventListenerTouchOneByOne = EventListenerTouchOneByOne::create();
    _eventListenerTouchOneByOne->onTouchBegan = CC_CALLBACK_2(ComponentDecoration::onTouchBegan, this);
    _eventListenerTouchOneByOne->onTouchMoved = CC_CALLBACK_2(ComponentDecoration::onTouchMoved, this);
    _eventListenerTouchOneByOne->onTouchEnded = CC_CALLBACK_2(ComponentDecoration::onTouchEnded, this);
    _eventListenerTouchOneByOne->onTouchCancelled = CC_CALLBACK_2(ComponentDecoration::onTouchCancelled, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_eventListenerTouchOneByOne, getOwner());
    
    _eventListenerTouchOneByOne->setSwallowTouches(true);
}

void ComponentDecoration::removeTouchEventListener()
{
    if (_eventListenerTouchOneByOne)
    {
        Director::getInstance()->getEventDispatcher()->removeEventListener(_eventListenerTouchOneByOne);
        _eventListenerTouchOneByOne = nullptr;
    }
}

bool ComponentDecoration::onTouchBegan(Touch *touch, Event *unused_event)
{
    if (!_enabled)
        return false;
    
    _noTouchTime = 0;
    if (!_frame->isVisible())
    {
        Point touchDecorationPoint = _owner->convertToNodeSpace(touch->getLocation());
        
        //if frame is not visible
        Rect decorateBoudingbox;
        decorateBoudingbox.size = _owner->getContentSize();
        
        if (decorateBoudingbox.containsPoint(touchDecorationPoint))
        {
            setFrameVisible(true);
            if (onSelectCallback) {
                onSelectCallback();
            }
            _isMoving = true;
            return true;
        }
        else
        {
            return false;
        }
    }
    
    Point touchZoomPoint = _zoom->convertToNodeSpace(touch->getLocation());
    
    Rect zoomBoudingbox;
    zoomBoudingbox.size = _zoom->getContentSize();
    if (zoomBoudingbox.containsPoint(touchZoomPoint))
    {
        _isZooming = true;
        _zoom->setColor(Color3B::GRAY);
        _originalRotation = _owner->getRotation();
    }
    else
    {
        Point touchClosePoint = _delete->convertToNodeSpace(touch->getLocation());
        Rect closeBoudingbox;
        closeBoudingbox.size = _delete->getContentSize();
        if (closeBoudingbox.containsPoint(touchClosePoint))
        {
            _delete->setColor(Color3B::GRAY);
        }
        else
        {
            Point touchFramePoint = _frame->convertToNodeSpace(touch->getLocation());
            Rect frameBoudingbox;
            frameBoudingbox.size = _frame->getContentSize();
            if (frameBoudingbox.containsPoint(touchFramePoint))
            {
                _isMoving = true;
            }
            else
            {
                //touch in outside, hide the frame
                setFrameVisible(false);
                return false;
            }
        }
    }
    
    return true;
}

void ComponentDecoration::onTouchMoved(Touch *touch, Event *unused_event)
{
    _noTouchTime = 0;
    if (!_frame->isVisible())
        return;
    
    if (_isZooming)
    {
        Point position = touch->getLocation();
        
        float distance = position.distance(_owner->convertToWorldSpaceAR(Vec2::ZERO));
        float scale = distance / _originalLenght;
        _owner->setScale(scale);
        
        if(scale < 0.5)
            _owner->setScale(0.5);
        if(scale > 1.5)
            _owner->setScale(1.5);
        
        Vec2 temp1 = touch->getStartLocation() - _owner->convertToWorldSpaceAR(Vec2::ZERO);
        Vec2 temp2 = position - _owner->convertToWorldSpaceAR(Vec2::ZERO);
        float theAngle = CC_RADIANS_TO_DEGREES(temp1.getAngle(temp2));
        _owner->setRotation(_originalRotation - theAngle);
    }
    else if (_isMoving)
    {
        auto newPosition = _owner->getPosition() + touch->getDelta();
        //log("%f %f", newPosition.x, newPosition.y);
        if (_moveBoundingBox.containsPoint(_owner->convertToWorldSpaceAR(Vec2::ZERO) + touch->getDelta()))
            _owner->setPosition(newPosition);
    }
    else
    {
        Point touchClosePoint = _delete->convertToNodeSpace(touch->getLocation());
        Rect closeBoudingbox;
        closeBoudingbox.size = _delete->getContentSize();
        if (closeBoudingbox.containsPoint(touchClosePoint))
        {
            _delete->setColor(Color3B::GRAY);
        }
        else
        {
            _delete->setColor(Color3B::WHITE);
        }
    }
}

void ComponentDecoration::onTouchEnded(Touch *touch, Event *unused_event)
{
    _zoom->setColor(Color3B::WHITE);
    _isZooming = false;
    _isMoving = false;
    
    Point touchClosePoint = _delete->convertToNodeSpace(touch->getLocation());
    Rect closeBoudingbox;
    closeBoudingbox.size = _delete->getContentSize();
    if (closeBoudingbox.containsPoint(touchClosePoint))
    {
        if (onDeleteCallback) {
            onDeleteCallback();
        }
        _owner->removeFromParent();
    }
}

void ComponentDecoration::onTouchCancelled(Touch *touch, Event *unused_event)
{
    _zoom->setColor(Color3B::WHITE);
    _isZooming = false;
    _isMoving = false;
}

