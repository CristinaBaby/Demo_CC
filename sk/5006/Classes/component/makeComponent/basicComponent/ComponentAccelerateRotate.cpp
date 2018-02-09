//
//  ComponentAcceleteRotate.cpp
//
//  Created by tanshoumei on 1/26/16.
//
//

#include "component/makeComponent/basicComponent/ComponentAccelerateRotate.h"

ComponentAccelerateRotate* ComponentAccelerateRotate::create(float endRotation)
{
    auto ret = new ComponentAccelerateRotate;
    ret->init(endRotation);
    ret->autorelease();
    return ret;
}

bool ComponentAccelerateRotate::init(float endRotation)
{
    ComponentBase::init();
    
    _endRotation = endRotation;
    
    return true;
}

ComponentAccelerateRotate::ComponentAccelerateRotate():
_deviationRotation(0.1),
_endRotation(-45)
{
    CCLOG("name:%s", (typeid(*this).name()));
    setName(typeid(*this).name());
}


void ComponentAccelerateRotate::onAdd()
{
    ComponentBase::onAdd();
}

void ComponentAccelerateRotate::onRemove()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(_listenerAccleration);
}

void ComponentAccelerateRotate::start()
{
    if(!getOwner())
        return;
    _minRotationForRotate = fminf(_startRotation, _endRotation);
    _maxRotationForRotate = fmaxf(_startRotation, _endRotation);
    _startRotation = getOwner()->getRotation();
    if(_listenerAccleration == nullptr)
    {
        _listenerAccleration = EventListenerAcceleration::create(CC_CALLBACK_2(ComponentAccelerateRotate::_acclerateCall, this));
    }
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listenerAccleration, getOwner());
}

void ComponentAccelerateRotate::pause()
{
    if(_listenerAccleration)
        _listenerAccleration->setEnabled(false);
}

void ComponentAccelerateRotate::resume()
{
    if(_listenerAccleration)
        _listenerAccleration->setEnabled(true);
}

void ComponentAccelerateRotate::stop()
{
    pause();
}

void ComponentAccelerateRotate::_acclerateCall(cocos2d::Acceleration *acceleration, cocos2d::Event * e)
{
    __CCLOGWITHFUNCTION("x:%.2f, y:%.2f, z:%.2f", acceleration->x, acceleration->y, acceleration->z);
    if (componentAccCall)
    {
        componentAccCall(acceleration , e);
    }
    
    if(getOwner() && _endRotation != 0.f)
    {
        auto newRotation = _startRotation + acceleration->x * 90;
        newRotation = clampf(newRotation, _minRotationForRotate, _maxRotationForRotate);
        if(fabs(newRotation - _endRotation) <= _deviationRotation)
        {
            newRotation = _endRotation;
            getOwner()->setRotation(newRotation);
            if(componentAccelerateRotateDoneCall)
            {
                componentAccelerateRotateDoneCall();
            }
            stop();
        }
        else
            getOwner()->setRotation(newRotation);
    }
}