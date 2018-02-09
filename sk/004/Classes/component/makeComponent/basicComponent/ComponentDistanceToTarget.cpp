//
//  ComponentDistanceToTarget.cpp
//
//  Created by tanshoumei on 1/25/16.
//
//

#include "component/makeComponent/basicComponent/ComponentDistanceToTarget.h"

ComponentDistanceToTarget* ComponentDistanceToTarget::create(const cocos2d::Vec2 &targetPointWorld)
{
    ComponentDistanceToTarget* ret = new ComponentDistanceToTarget();
    ret->init(targetPointWorld);
    ret->autorelease();
    return ret;
}

ComponentDistanceToTarget* ComponentDistanceToTarget::create(const cocos2d::Rect &targetRectWorld)
{
    ComponentDistanceToTarget* ret = new ComponentDistanceToTarget();
    ret->init(targetRectWorld);
    ret->autorelease();
    return ret;
}

ComponentDistanceToTarget::ComponentDistanceToTarget():
_currentDis(FLT_MAX)
{
    setName(typeid(ComponentDistanceToTarget).name());
}

ComponentDistanceToTarget::~ComponentDistanceToTarget()
{
    stop();
}

bool ComponentDistanceToTarget::init(const cocos2d::Vec2 &targetPointWorld)
{
    _targetType |= kTargetTypePosition;
    _targetPoint = targetPointWorld;
    return true;
}

bool ComponentDistanceToTarget::init(const cocos2d::Rect &targetRectWorld)
{
    _targetType |= kTargetTypeRect;
    _targetRect = targetRectWorld;
    return true;
}

void ComponentDistanceToTarget::onAdd()
{
    ComponentBase::onAdd();
}

void ComponentDistanceToTarget::onRemove()
{
    stop();
}

void ComponentDistanceToTarget::start()
{
    Director::getInstance()->getScheduler()->schedule(schedule_selector(ComponentDistanceToTarget::_checkTargetSchedule), this, 0.05, false);
}

void ComponentDistanceToTarget::stop()
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(ComponentDistanceToTarget::_checkTargetSchedule), this);
}

void ComponentDistanceToTarget::pause()
{
    Director::getInstance()->getScheduler()->pauseTarget(this);
}

void ComponentDistanceToTarget::resume()
{
    Director::getInstance()->getScheduler()->resumeTarget(this);
}

void ComponentDistanceToTarget::_checkTargetSchedule(float dt)
{
    if(getOwner() && (_targetType != kTargetTypeNone) )
    {
        _currentDis = FLT_MAX;
        if(_targetType && kTargetTypePosition)
        {
            auto pt = getOwner()->convertToWorldSpaceAR(Vec2::ZERO);
            _currentDis = getOwner()->convertToWorldSpaceAR(Vec2::ZERO).distance(_targetPoint);
        }
        else if(_targetType && kTargetTypeRect)
        {
            _currentDis = _targetRect.containsPoint(getOwner()->convertToWorldSpaceAR(Vec2::ZERO)) ? 1: 0;
        }
        if(ComponentDistanceToTargetCallback)
            ComponentDistanceToTargetCallback(_currentDis);
    }
}