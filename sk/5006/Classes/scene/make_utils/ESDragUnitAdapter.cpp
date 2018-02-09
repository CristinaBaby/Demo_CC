//
//  ESDragUnitAdapter.cpp
//  ccats1053
//
//  Created by liji on 16/1/14.
//
//

#include "ESDragUnitAdapter.h"
ESDragUnitAdapter::ESDragUnitAdapter()
{
    _listener = nullptr;
    _scroll = nullptr;
    _checkMoving = true;
    _isMoving = false;
    _moveDisX = 0.f;
    _moveDisY = 0.f;
}

ESDragUnitAdapter::~ESDragUnitAdapter()
{


}


ui::ImageView* ESDragUnitAdapter::getUnit(int idx)
{
//    ui::ImageView* bg = ui::ImageView::create();

    auto btn = ui::ImageView::create(StringUtils::format(_pathFormat.c_str(),idx));
//    bg->addChild(btn);
    btn->setTag(idx);
    this->initListener();
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener->clone(), btn);
        
    return btn;
}

//new

void ESDragUnitAdapter::initListener()
{
    if (!_listener)
    {
        _listener = EventListenerTouchOneByOne::create();
        _listener->onTouchBegan=CC_CALLBACK_2(ESDragUnitAdapter::onTouchBegan, this);
        _listener->onTouchMoved=CC_CALLBACK_2(ESDragUnitAdapter::onTouchMoved, this);
        _listener->onTouchEnded=CC_CALLBACK_2(ESDragUnitAdapter::onTouchEnd, this);
        _listener->onTouchCancelled=CC_CALLBACK_2(ESDragUnitAdapter::onTouchCancelled, this);
        _listener->setSwallowTouches(true);
    }

}


bool ESDragUnitAdapter::onTouchBegan(Touch* touch, Event* event)
{
    auto target = static_cast<ui::Widget*>(event->getCurrentTarget());
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);

    if (!_scroll->isEnabled())
    {
        return false;
    }
    
    if (rect.containsPoint(locationInNode))
    {
        _scroll->onTouchBegan(touch, event);

        if (_itemTouchBegan)
        {
            _itemTouchBegan(target , touch);
        }
        
        return true;
    }
    
    
    
    return false;
}

void ESDragUnitAdapter::onTouchMoved(Touch* touch, Event* event)
{
    auto target = static_cast<ui::Widget*>(event->getCurrentTarget());
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    auto offset = touch->getLocation() - touch->getStartLocation();

    auto wp = touch->getStartLocation();
    auto p = touch->getLocation();
    auto delta = touch->getLocation()-touch->getPreviousLocation();
    float k = (p.y-wp.y)/(p.x-wp.x);
    log ("%f, ====" , k);
    _moveDisX += delta.x;
    _moveDisY += delta.y;
    log ("%f _moveDis %f====" , abs(_moveDisX),abs(_moveDisY));
    log ("%f delta %f ====" , delta.x,delta.y);

    if (!_scroll->isEnabled())
    {
        _checkMoving = false;
    }
    
    if (_isMoving)
    {
        if (_itemTouchMoved)
        {
            _itemTouchMoved(target, touch);
        }
        log ("isDragging");

        return;
    }
    
    if (abs(_moveDisX) > 10.f || abs(_moveDisY) > 10)
    {
        if (_checkMoving)
        {
            
            if (k > 0.8 || k < -2.4)
            {
                log ("start drag");

                _isMoving = true;
            }
            else
            {
                _scroll->onTouchMoved(touch, event);
                _checkMoving = false;
            }
        }
        else
        {
            _scroll->onTouchMoved(touch, event);
        }
    }
    
}

void ESDragUnitAdapter::onTouchEnd(Touch* touch, Event* event)
{
    
    auto target = static_cast<ui::Widget*>(event->getCurrentTarget());
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    auto wp = target->getParent()->convertToWorldSpace(Vec2::ZERO);
    auto p = touch->getLocation();

    if (_itemTouchEnded)
        _itemTouchEnded(target,touch);
    
    if (!_isMoving)
    {
        _scroll->onTouchEnded(touch, event);
    }
    _isMoving = false;
    _checkMoving = true;
    _moveDisX = 0.f;
    _moveDisY = 0.f;
}

void ESDragUnitAdapter::onTouchCancelled(Touch* touch, Event* event)
{
    _scroll->onTouchCancelled(touch, event);


}



