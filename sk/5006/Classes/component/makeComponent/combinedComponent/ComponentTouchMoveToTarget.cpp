//
//  ComponentTouchMoveToTarget.cpp
//
//  Created by tanshoumei on 1/25/16.
//
//

#include "component/makeComponent/combinedComponent/ComponentTouchMoveToTarget.h"

ComponentTouchMoveToTarget::~ComponentTouchMoveToTarget()
{
    if (!_owner)
        return;
    if (_componentTouchMove)
    {
        _componentTouchMove->getOwner()->removeComponent(_componentTouchMove);
        _componentTouchMove = nullptr;
    }
    
    if (_compPosition)
    {
        _compPosition->getOwner()->removeComponent(_compPosition);
        _compPosition = nullptr;
    }
}

ComponentTouchMoveToTarget* ComponentTouchMoveToTarget::create(const cocos2d::Vec2 &targetPointWorld)
{
    auto ret = new ComponentTouchMoveToTarget;
    ret->init(targetPointWorld);
    ret->autorelease();
    return ret;
}

bool ComponentTouchMoveToTarget::init(const cocos2d::Vec2 &targetPointWorld)
{
    setName(typeid(*this).name());
    
    _targetPointWorld = targetPointWorld;
    
    _whenMoveToTarget = kMoveToTargetWhenTouchEnded;
    
    _backToPointLocal.x = -INT_MAX;
    
    _maxDistanceForMoveToTarget = 600;
    return true;
}

void ComponentTouchMoveToTarget::setEnableMove(bool enable)
{
    if (!_componentTouchMove)
        return;
    _componentTouchMove->setEnabled(enable);
}

void ComponentTouchMoveToTarget::start()
{
    if(!getOwner())
        return;
    _backToPointLocal = getOwner()->getPosition();
    _componentTouchMove = ComponentTouchMove::create();
    _compPosition = ComponentDistanceToTarget::create(_targetPointWorld);
    
    _componentTouchMove->touchBeganCall = [=](Touch* touch)
    {
        if(touchBeganCall)
        {
            touchBeganCall(touch);
        }
    };
    _componentTouchMove->touchMovedCall = [=](Touch* touch)
    {
        if(_whenMoveToTarget == kMoveToTargetWhenTouchMove)
        {
            if(_compPosition->getCurrentDis() < _maxDistanceForMoveToTarget)
            {
                CCLOG("%s dis:%.2f", __FILE__, _compPosition->getCurrentDis());
                _compPosition->getOwner()->removeComponent(_compPosition);
                _compPosition = nullptr;
                
                _componentTouchMove->setEnabled(false);
                this->doMoveToTarget();
            }
        }
        if(touchMovedCall)
        {
            touchMovedCall(touch);
        }
    };
    _componentTouchMove->touchEndedCall = [=](Touch* touch)
    {
        if (!_componentTouchMove)
            return;
        
        if (!_componentTouchMove->isEnabled())
        {
            _componentTouchMove->getOwner()->runAction(CallFunc::create([=](){
                
                _componentTouchMove->getOwner()->removeComponent(_componentTouchMove);
                _componentTouchMove = nullptr;
            }));
        }
        
        if(_whenMoveToTarget == kMoveToTargetWhenTouchEnded)
        {
            if(_compPosition->getCurrentDis() < _maxDistanceForMoveToTarget)
            {
                CCLOG("%s dis:%.2f", __FILE__, _compPosition->getCurrentDis());
                _compPosition->getOwner()->removeComponent(_compPosition);
                _compPosition = nullptr;
                
                _componentTouchMove->getOwner()->runAction(CallFunc::create([=](){
                   
                    _componentTouchMove->getOwner()->removeComponent(_componentTouchMove);
                    _componentTouchMove = nullptr;
                }));
                this->doMoveToTarget();
            }
            else
            {
                this->doBackToAction();
            }
        }
        if(touchEndedCall)
        {
            touchEndedCall(touch);
        }
    };
    
    getOwner()->addComponent(_compPosition);
    _compPosition->start();
    getOwner()->addComponent(_componentTouchMove);
}

void ComponentTouchMoveToTarget::pause()
{
    if(_componentTouchMove)
        _componentTouchMove->pause();
}

void ComponentTouchMoveToTarget::resume()
{
    if(_componentTouchMove)
        _componentTouchMove->resume();
}

void ComponentTouchMoveToTarget::doMoveToTarget()
{    
    auto moveBy = MoveBy::create(0.3, _targetPointWorld - getOwner()->convertToWorldSpaceAR(Vec2::ZERO));
    auto call = CallFunc::create([=](){
        if(componentTouchMoveToTargetDoneCall)
            componentTouchMoveToTargetDoneCall();
    });
    getOwner()->runAction(Sequence::create(moveBy, call, nullptr));
}

void ComponentTouchMoveToTarget::moveToTargetDone()
{
    if(_backToPointLocal.x > -1000000)
    {
        doBackToAction();
    }
}

void ComponentTouchMoveToTarget::doBackToAction()
{
    if(_backToPointLocal.x < -1000000 || _isAnimation)
        return;
    
    _isAnimation = true;
    getOwner()->runAction(Sequence::create(MoveTo::create(0.3, _backToPointLocal), CallFunc::create([=](){
        
        backToDone();
        _isAnimation = false;
    }), nullptr));
}

void ComponentTouchMoveToTarget::backToDone()
{
    if(componentTouchMoveBackDoneCall)
        componentTouchMoveBackDoneCall();
}