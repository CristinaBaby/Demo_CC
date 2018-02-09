//
//  ComponentTurning.cpp
//  BHUG1062
//
//  Created by liji on 16/4/20.
//
//

#include "ComponentTurning.hpp"


ComponentTurning::ComponentTurning()
{
    _direction  = kComponentTurningDirectionClockwise;

}


void ComponentTurning::onAdd()
{
    
    if(getOwner())
    {
        auto eventListenerTouchOneByOne = EventListenerTouchOneByOne::create();

        eventListenerTouchOneByOne->onTouchBegan = CC_CALLBACK_2(ComponentTurning::onTouchBegan, this);
        
        eventListenerTouchOneByOne->onTouchMoved = CC_CALLBACK_2(ComponentTurning::onTouchMoved, this);
        
        eventListenerTouchOneByOne->onTouchEnded = CC_CALLBACK_2(ComponentTurning::onTouchEnded, this);
        
        eventListenerTouchOneByOne->onTouchCancelled = CC_CALLBACK_2(ComponentTurning::onTouchCancelled, this);
        eventListenerTouchOneByOne->setSwallowTouches(true);
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListenerTouchOneByOne, getOwner());
    }

}


bool ComponentTurning::onTouchBegan(Touch* touch, Event* event)
{
    Node* parent = getOwner()->getParent();
    if(parent)
    {
        Point ptInParent = parent->convertTouchToNodeSpace(touch);
        bool isTouchIn = getOwner()->getBoundingBox().containsPoint(ptInParent);
        return isTouchIn;
    }
    return false;


}

void ComponentTurning::onTouchMoved(Touch* touch, Event* event)
{

    Point po = touch->getLocation();
    
    Vec2 temp2 = po - getOwner()->getPosition();
    Vec2 temp3 = touch->getPreviousLocation() - getOwner()->getPosition();
    float theAngle = CC_RADIANS_TO_DEGREES(temp2.getAngle(temp3));
    
    if ((_direction == kComponentTurningDirectionClockwise && temp2.getAngle(temp3) > 0)
        ||
        (_direction == kComponentTurningDirectionAnticlockwise && temp2.getAngle(temp3) < 0))
    {
        _orignalRotate += theAngle;
        
        if (_turningCallback && abs(theAngle)>1)
        {
            _turningCallback(_orignalRotate);
        }
        else if(_turningStop)
        {
            _turningStop();
        }
    }
    
    
    log("%f theAngle",theAngle);
    log("%f _orignalRotate",_orignalRotate);
    
    getOwner()->setRotation(_orignalRotate);

}

void ComponentTurning::onTouchEnded(Touch* touch, Event* event)
{
    if (_turningStop)
    {
        _turningStop();
    }

}

void ComponentTurning::onTouchCancelled(Touch* touch, Event* event)
{


}
