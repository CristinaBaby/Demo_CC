//
//  CombActions.h
//
//  Created by tanshoumei on 15/5/18.
//
//

#ifndef CombActions_H
#define CombActions_H

#include <stdio.h>
#include "component/actions/CombActionBase.h"
#include "component/actions/ActionHelper.h"

NS_CombAction_BEGIN

class CombActionMoveBase: public CombActionBase
{
public:
    CombActionMoveBase();
    
    bool init(Node* aTargetNode, const Vec2& atTarget, ActionDirection aDirection);
    
protected:
    virtual MoveTo *_getMoveAction();
        
protected:
    Vec2 _targetPosition;
    
    ActionDirection _direction;
};

class CombActionThrowBackIn: public CombActionMoveBase
{
public:
    static CombActionThrowBackIn* create(Node* aTargetNode, const Vec2& atTarget, ActionDirection aDirection);
    
protected:
    virtual ActionInterval *_getAction();
    
    //回弹时间
    CC_SYNTHESIZE(float, _backDuration, BackDuration);
    //回弹距离
    CC_SYNTHESIZE(float, _backDistance, BackDistance);
    
};

class CombActionMoveInBounce: public CombActionMoveBase
{
public:
    static CombActionMoveInBounce* create(Node* aTargetNode, const Vec2& atTarget, ActionDirection aDirection);
    
protected:
    virtual ActionInterval *_getAction();
};

class CombActionMoveInEaseExponential: public CombActionMoveBase
{
public:
    static CombActionMoveInEaseExponential* create(Node* aTargetNode, const Vec2& atTarget, ActionDirection aDirection);
    
protected:
    virtual ActionInterval *_getAction();
};

class CombActionMoveOutEaseExponential: public CombActionMoveBase
{
public:
    static CombActionMoveOutEaseExponential* create(Node* aTargetNode, ActionDirection aDirection);
        
protected:
    virtual ActionInterval *_getAction();
};

class CombActionJumpBy: public CombActionBase
{
public:
    static CombActionJumpBy* create(Node* aTargetNode, float duration, const Vec2& jumpPosition, float height, int jumps);
    
    bool init(Node* aTargetNode, float duration, const Vec2& position, float height, int jumps);
    
protected:
    virtual ActionInterval *_getAction();
    
protected:
    Vec2 _jumpPosition;
        
    CC_SYNTHESIZE(float, _duration, Duration);
    
    CC_SYNTHESIZE(int, _jumpTimes, JumpTimes);
    
    CC_SYNTHESIZE(float, _height, Height);
};

class CombActionScaleBounce: public CombActionBase
{
public:
    static CombActionScaleBounce* create(Node *aTargetNode, unsigned int times = kRepeatForever);
    bool init(Node *aTargetNode, unsigned int times);
protected:
    virtual ActionInterval *_getAction();
    
protected:
    CC_SYNTHESIZE(float, _bigFactor, BigFactor);
    
    CC_SYNTHESIZE(float, _smallFactor, smallFactor);
    
    unsigned int _repeatTimes;
};

NS_CombAction_END

#endif