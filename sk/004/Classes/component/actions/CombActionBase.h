//
//  CombActionBase.h
//
//  Created by tanshoumei on 15/5/15.
//
//

#ifndef CombActionBase_H
#define CombActionBase_H

#include <stdio.h>
#include "component/actions/ActionMacro.h"
#include "cocos2d.h"
USING_NS_CC;
NS_CombAction_BEGIN

enum CombActionTriggerType
{
    kCombActionTriggerTypeAtOnce,
    kCombActionTriggerTypeEvent,
    kCombActionTriggerTypeDuration
};

enum CombActionState
{
    kCombActionStateNone,
    kCombActionStateBegin,
    kCombActionStateEnd
};

class CombActionBase;

class CombActionDelegate
{
public:
    virtual void actionStateChanged(CombActionBase* apAction);
};

class CombActionBase: public Ref {
public:
    CombActionBase();
    
    ~CombActionBase();
    
    virtual void start();
    
    virtual void stop();
    
    union TriggerParam{
        std::string name;
        float duration;
    };
        
protected:
    virtual void _scheduleCallback(float dt);
    
    virtual ActionInterval *_getAction() = 0;
    
    virtual void _actionRealDo();
    
    virtual void _actionDone();
    
public:
    std::function<void()> actionStartCallBack = nullptr;
    
    std::function<void()> actionEndCallBack = nullptr;
    
protected:
    CC_SYNTHESIZE(CombActionTriggerType, _triggerType, TriggerType);
    
    CC_SYNTHESIZE(std::string, _actionName, ActionName);
   
    CC_SYNTHESIZE(std::string, _triggerParam, TriggerParam);
    
    CC_SYNTHESIZE(Node*, _targetNode, TargetNode);
    
    CC_SYNTHESIZE(CombActionDelegate*, _combActionDelegate, CombActionDelegate);
    
    CC_SYNTHESIZE(CombActionState, _combActionState, CombActionState);
    
    CC_SYNTHESIZE(float, _actionDuration, ActionDuration);
    
    Action* _currentAction = nullptr;
};

NS_CombAction_END

#endif