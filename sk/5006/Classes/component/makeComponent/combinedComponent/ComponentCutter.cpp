//
//  ComponentCutter.cpp
//
//
//  Created by maxiang on 3/17/16.
//
//

#include "ComponentCutter.h"

ComponentCutter::ComponentCutter():
_cutMode(CutMode::kCutModeAuto),
_compMoveToTarget(nullptr),
_eventListenerTouchOneByOne(nullptr),
_isFinish(false)
{
    
}

ComponentCutter::~ComponentCutter()
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(ComponentCutter::dragCut), this);
}

bool ComponentCutter::init()
{
    
    return true;
}

#pragma mark- Cut

void ComponentCutter::start()
{
    if (!_owner)
        return;
    
    _isFinish = false;
    if (CutMode::kCutModeAuto == _cutMode)
    {
        doAutoCut();
    }
    else if (CutMode::kCutModeDrag == _cutMode)
    {
        doDragCut();
    }
}

void ComponentCutter::doAutoCut()
{
    if (!_owner)
        return;
    
    auto doneCall = [this]()
    {
        if (cutFinishCallBack)
            cutFinishCallBack();
    };
    
    
    auto moveUpAction = MoveBy::create(_autoCutConfig.cutterMoveDuration, Vec2(0.0, _autoCutConfig.cutterMoveDistance));
    auto moveDownAction = MoveBy::create(_autoCutConfig.cutterMoveDuration, Vec2(0.0, -_autoCutConfig.cutterMoveDistance));
    
    Vector<FiniteTimeAction*> cutActionsVector;
    int times = _autoCutConfig.cutTimes;
    bool flag = true;
    while (times > 0) {
        
        if (flag)
            cutActionsVector.pushBack(moveDownAction);
        else
            cutActionsVector.pushBack(moveUpAction);
        flag = !flag;
        --times;
    }
    
//    auto test = MotionStreak::create(_autoCutConfig.cutterMoveDuration/2, 20, 60, Color3B::GREEN, "res/1cut_fruit/light.png");
//    _owner->addChild(test,-1);
//    test->setPosition(_owner->getContentSize());
//    test->runAction(MoveBy::create(_autoCutConfig.cutterMoveDuration, Vec2(0.0, -_autoCutConfig.cutterMoveDistance)));
    
    _owner->runAction(Sequence::create(Sequence::create(cutActionsVector), CallFunc::create(doneCall), NULL));
}

void ComponentCutter::doDragCut()
{
    if (!_owner)
        return;

    Director::getInstance()->getScheduler()->schedule(schedule_selector(ComponentCutter::dragCut), this, 1.0/60.0, false);

    _cutterOriPosition = _owner->getPosition();
    _cutterPosition = _owner->getPosition();

    addTouchEventListener();
}

void ComponentCutter::dragCut(float time)
{
    static float distance = 0;
    static float x = -_dragCutConfig.cutterMoveSpeed;
    static float angel = 90.0 - _dragCutConfig.cutterAngel;

    if (_owner->getPosition().y > _cutterPosition.y)
    {
        float y = fabsf(x) * tan(CC_DEGREES_TO_RADIANS(angel));
        
        if (x > 0)
            y = -y;
        
        _owner->setPosition(_owner->getPosition() + Vec2(x, y));
        
        if (_cutterOriPosition.y - _owner->getPosition().y >= _dragCutConfig.cutSomethingsHeight)
        {
            if (_isFinish)
                return;
            
            _isFinish = true;
            Director::getInstance()->getScheduler()->unschedule(schedule_selector(ComponentCutter::dragCut), this);
            
            removeTouchEventListener();
            
            if (cutFinishCallBack)
                cutFinishCallBack();
            
            return;
        }

        if (_dragCutting)
        {
            _dragCutting();
        }
        
        distance += fabsf(x);
        if (distance >= _dragCutConfig.cutterMoveDistance)
        {
            x = x * -1;
            distance = 0;
            
            if (x > 0)
            {
                angel = 90.0 - _dragCutConfig.cutterAngel + _dragCutConfig.cutterFrameAngel;
            }
            else
            {
                angel = 90.0 - _dragCutConfig.cutterAngel;
            }
        }
    }
    else
    {
        if(_dragCuttingStop)
            _dragCuttingStop();
    }
}

void ComponentCutter::addTouchEventListener()
{
    if (_eventListenerTouchOneByOne || !getOwner())
        return;
    
    _eventListenerTouchOneByOne = EventListenerTouchOneByOne::create();
    _eventListenerTouchOneByOne->onTouchBegan = CC_CALLBACK_2(ComponentCutter::onTouchBegan, this);
    _eventListenerTouchOneByOne->onTouchMoved = CC_CALLBACK_2(ComponentCutter::onTouchMoved, this);
    _eventListenerTouchOneByOne->onTouchEnded = CC_CALLBACK_2(ComponentCutter::onTouchEnded, this);
    _eventListenerTouchOneByOne->onTouchCancelled = CC_CALLBACK_2(ComponentCutter::onTouchCancelled, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_eventListenerTouchOneByOne, getOwner());
    _eventListenerTouchOneByOne->setSwallowTouches(true);
}

void ComponentCutter::removeTouchEventListener()
{
    if (_eventListenerTouchOneByOne)
    {
        Director::getInstance()->getEventDispatcher()->removeEventListener(_eventListenerTouchOneByOne);
        _eventListenerTouchOneByOne = nullptr;
    }
}

bool ComponentCutter::onTouchBegan(Touch* touch, Event* event)
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

void ComponentCutter::onTouchMoved(Touch* touch, Event* event)
{
    _cutterPosition = _cutterPosition + touch->getDelta();
}

void ComponentCutter::onTouchEnded(Touch* touch, Event* event)
{
    
}

void ComponentCutter::onTouchCancelled(Touch* touch, Event* event)
{
    
}

#pragma mark- Functions

void ComponentCutter::setTargetWorldPoint(const Vec2& worldPoint,
                                          const float distance)
{
    if (!_owner)
        return;
    
    if (_compMoveToTarget)
    {
        _owner->removeComponent(_compMoveToTarget);
        _compMoveToTarget = nullptr;
    }
    
    _compMoveToTarget = ComponentTouchMoveToTarget::create(worldPoint);
    _compMoveToTarget->setName(typeid(_compMoveToTarget).name());
    _compMoveToTarget->setMaxDistanceForMoveToTarget(distance);
    _owner->addComponent(_compMoveToTarget);

    _compMoveToTarget->start();
    _compMoveToTarget->componentTouchMoveToTargetDoneCall = [this]()
    {
        if (onTargetWorldPointCallBack)
            onTargetWorldPointCallBack();
        
        start();
    };
}






