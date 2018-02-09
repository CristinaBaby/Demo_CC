//
//  CombActionBase.cpp
//
//  Created by tanshoumei on 15/5/15.
//
//

#include "component/actions/CombActionBase.h"

NS_CombAction_BEGIN

CombActionBase::CombActionBase():
_targetNode(nullptr),
_combActionDelegate(nullptr),
_triggerType(kCombActionTriggerTypeAtOnce),
_combActionState(kCombActionStateNone),
_actionDuration(1.0)
{
}

CombActionBase::~CombActionBase()
{
    if(_triggerType == kCombActionTriggerTypeEvent && _triggerParam.length() > 0)
    {
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(_triggerParam);
    }
    else if(_triggerType == kCombActionTriggerTypeDuration)
    {
        auto duration = Value(_triggerParam).asFloat();
        if(duration > 0)
        {
            Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
        }
    }
}


void CombActionBase::start()
{
    if(_triggerType == kCombActionTriggerTypeEvent)
    {
        auto callback = [this](EventCustom *){
            _actionRealDo();
        };
        if(_triggerParam.length() > 0)
        {
            Director::getInstance()->getEventDispatcher()->addCustomEventListener(_triggerParam, callback);
        }
    }
    else if(_triggerType == kCombActionTriggerTypeDuration)
    {
        if(_triggerParam.length() > 0)
        {
            auto duration = Value(_triggerParam).asFloat();
            if(duration > 0)
            {
                Director::getInstance()->getScheduler()->schedule(schedule_selector(CombActionBase::_scheduleCallback), this, 0, 0, duration, false);
            }
        }
    }
    else if(_triggerType == kCombActionTriggerTypeAtOnce)
    {
        _actionRealDo();
    }
    
}

void CombActionBase::stop()
{
    if(_currentAction)
    {
        _targetNode->stopAction(_currentAction);
    }
}

void CombActionBase::_scheduleCallback(float dt)
{
    _actionRealDo();
    if(actionStartCallBack)
    {
        actionStartCallBack();
    }
}

void CombActionBase::_actionRealDo()
{
    _combActionState = kCombActionStateBegin;
    
    if(_actionName.length() > 0)
    {
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(_actionName + "_begin");
    }
    
    if(_targetNode)
    {
        _currentAction = _getAction();
        _targetNode->runAction(_currentAction);
    }
    if(_combActionDelegate)
    {
        _combActionDelegate->actionStateChanged(this);
    }
}

void CombActionBase::_actionDone()
{
    _combActionState = kCombActionStateEnd;
    
    _currentAction = nullptr;
    
    if(_actionName.length() > 0)
    {
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(_actionName + "_end");
    }
    
    if(_combActionDelegate)
    {
        _combActionDelegate->actionStateChanged(this);
    }
    if(actionEndCallBack)
    {
        actionEndCallBack();
    }
}

NS_CombAction_END