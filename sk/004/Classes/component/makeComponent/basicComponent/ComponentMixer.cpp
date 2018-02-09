//
//  ComponentMixer.cpp
//
//  Created by tanshoumei on 2/2/16.
//
//

#include "component/makeComponent/basicComponent/ComponentMixer.h"

ComponentMixer::ComponentMixer():
_mixerType(kMixerTypeDistance),
_mixDoneLength(5000),
_effectLocalPosition(Vec2::ZERO)
{
    _eventListenerTouchOneByOne = EventListenerTouchOneByOne::create();
    _eventListenerTouchOneByOne->retain();
    setName(typeid(ComponentMixer).name());
    
    _effectEffectWorld.origin = Director::getInstance()->getVisibleOrigin();
    _effectEffectWorld.size = Director::getInstance()->getVisibleSize();
}

ComponentMixer::~ComponentMixer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(_eventListenerTouchOneByOne);
    CC_SAFE_RELEASE_NULL(_eventListenerTouchOneByOne);
}

void ComponentMixer::setEffectiveArea(const cocos2d::Rect &world)
{
    _effectEffectWorld = world;
}

void ComponentMixer::start()
{
    if(!getOwner())
        return;
    _eventListenerTouchOneByOne->onTouchBegan = [=](Touch* touch, Event* )
    {
        Point ptInParent = getOwner()->getParent()->convertTouchToNodeSpace(touch);
        bool isTouchIn = isTouchInRecursionChildren(getOwner(), touch);
        if(isTouchIn)
        {
            if(_mixerType == kMixerTypeTimed)
            {
                Director::getInstance()->getScheduler()->schedule(schedule_selector(ComponentMixer::timerSchedule), this, 0.1, false);
            }
        }
        _previousPosition = getOwner()->getPosition();
        return isTouchIn;
    };
    
    _eventListenerTouchOneByOne->onTouchMoved = [=](Touch* touch, Event* )
    {
        if(this->isOwnerInEffectArea())
        {
            if(_mixerType == kMixerTypeDistance)
            {
                auto dis = (getOwner()->getPosition() - _previousPosition).length();
                _moveDistanceAll += dis;
                computeCompleteAndCallback();
                return;
            }
            else
            {
                Director::getInstance()->getScheduler()->resumeTarget(this);
            }
        }
        else
        {
            _previousPosition = getOwner()->getPosition();
            Director::getInstance()->getScheduler()->pauseTarget(this);
        }
    };
    
    _eventListenerTouchOneByOne->onTouchEnded = [=](Touch* touch, Event* )
    {
        if(_mixerType == kMixerTypeTimed){
            Director::getInstance()->getScheduler()->unschedule(schedule_selector(ComponentMixer::timerSchedule), this);
        }
        
        if (_componentMixPaused)
            _componentMixPaused();
    };
    
    _eventListenerTouchOneByOne->onTouchCancelled = _eventListenerTouchOneByOne->onTouchEnded;
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_eventListenerTouchOneByOne, -1);
}

void ComponentMixer::stop()
{
    _eventListenerTouchOneByOne->setEnabled(false);
    if(_mixerType == kMixerTypeTimed){
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(ComponentMixer::timerSchedule), this);
    }
}

void ComponentMixer::timerSchedule(float dt)
{
    _touchTimeAll += dt;
    computeCompleteAndCallback();
}

void ComponentMixer::computeCompleteAndCallback()
{
    if(!componentMixCallback)
        return;
    float percent = 0;
    if(_mixerType == kMixerTypeTimed)
    {
        percent = _touchTimeAll / _mixDoneLength;
    }
    else if(_mixerType == kMixerTypeDistance)
    {
        percent = _moveDistanceAll / _mixDoneLength;
    }
    percent = clampf(percent, 0, 1.0);
    _mixPercent = percent * 100;
    componentMixCallback(_mixPercent);
    //done
    if(_mixPercent > 99.9)
    {
        stop();
    }
}

bool ComponentMixer::isOwnerInEffectArea()
{
    if(getOwner() == nullptr)
        return false;
    Vec2 ptWorld = getOwner()->convertToWorldSpaceAR(_effectLocalPosition);
    return _effectEffectWorld.containsPoint(ptWorld);
}

bool ComponentMixer::isTouchInRecursionChildren(Node* node, cocos2d::Touch * t)
{
    Node* parent = node->getParent();
    if(!parent)
        return false;
    Point ptInParent = parent->convertTouchToNodeSpace(t);
    bool isTouchIn = node->getBoundingBox().containsPoint(ptInParent);
    if(isTouchIn)
    {
        return true;
    }
    for(auto child: node->getChildren())
    {
        if(isTouchInRecursionChildren(child, t))
            return true;
    }
    return false;
}