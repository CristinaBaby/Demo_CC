//
//  ComponentTouchMoveToTarget.hpp
//
//  Created by tanshoumei on 1/25/16.
//
//

#ifndef ComponentTouchMoveToTarget_hpp
#define ComponentTouchMoveToTarget_hpp

#include <stdio.h>
#include "component/makeComponent/basicComponent/ComponentBase.h"
#include "component/makeComponent/basicComponent/ComponentDistanceToTarget.h"
#include "component/makeComponent/basicComponent/ComponentTouchMove.h"

//move to target的时机类型
enum {
    //1 即使还没放手，只要距离够了就move to target
    kMoveToTargetWhenTouchMove,
    //2 只有在放手的时候，距离满足才move to target(Default)
    kMoveToTargetWhenTouchEnded
};

/// 触摸移动目标到指定点。
/// 可设置容差距离
class ComponentTouchMoveToTarget: public ComponentBase
{
public:
    static ComponentTouchMoveToTarget* create(const Vec2& targetPointWorld);
    
    bool init(const Vec2& targetPointWorld);
    
    virtual void start();
    
    virtual void pause();
    
    virtual void resume();
    
    virtual ~ComponentTouchMoveToTarget();
    
public:
#pragma mark --call back for touch move
    //触摸相关回调
    std::function<void(Touch* touch)> touchBeganCall = nullptr;
    
    std::function<void(Touch* touch)> touchMovedCall = nullptr;
    
    std::function<void(Touch* touch)> touchEndedCall = nullptr;
 
#pragma mark --call back new
    //移动到目标点
    std::function<void()> componentTouchMoveToTargetDoneCall = nullptr;
    
    //回到指定起点
    std::function<void()> componentTouchMoveBackDoneCall = nullptr;
    
    //move to target的时机类型
    CC_SYNTHESIZE(int, _whenMoveToTarget, WhenMoveToTarget);
    
    //当小于此值时，就可以move to target了
    CC_SYNTHESIZE(float, _maxDistanceForMoveToTarget, MaxDistanceForMoveToTarget);
    
    //松手后如未接近到目标点，回到的点。local坐标系.
    //默认值为一个很远的无意义点，如果设置此值，就代表开始此功能
    CC_SYNTHESIZE(Vec2, _backToPointLocal, BackToPointLocal);
    
    CC_SYNTHESIZE(Vec2, _targetPointWorld, TargetPointWorld);
    
    void setEnableMove(bool enable);
protected:
    virtual void doMoveToTarget();
    
    virtual void moveToTargetDone();
    
    virtual void doBackToAction();
    
    virtual void backToDone();
    
protected:
    ComponentTouchMove* _componentTouchMove = nullptr;
    ComponentDistanceToTarget *_compPosition = nullptr;
    bool _isAnimation = false;
};

#endif /* ComponentTouchMoveToTarget_hpp */
