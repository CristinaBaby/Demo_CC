//
//  ComponentDistanceToTarget.hpp
//
//  Created by tanshoumei on 1/25/16.
//
//

#pragma once

#include <stdio.h>
#include "component/makeComponent/basicComponent/ComponentBase.h"

enum{
    kTargetTypeNone = 0,
    kTargetTypePosition = 1,
    kTargetTypeRect = 1 << 1
};

/// 用于判断到目标点(区域)距离的组件
class ComponentDistanceToTarget: public ComponentBase
{
public:
    static ComponentDistanceToTarget* create(const Vec2& targetPointWorld);
    
    static ComponentDistanceToTarget* create(const Rect& targetRectWorld);
    
public:
    /**
     *  回调到target的距离
     *  如果目标是一个点，则为到目标点的实际距离
     *  如果目标是一个区域，距离为0表示在区域内，不为0表示在区域外
     */
    std::function<void(float)> ComponentDistanceToTargetCallback = nullptr;
    
    /**
     *  除了通过回调(ComponentDistanceToTargetCallback)来获取到目标的距离外，也可主动调用get获取
     */
    CC_SYNTHESIZE_READONLY(float, _currentDis, CurrentDis);
   
public:
    
    bool init(const Vec2& targetPointWorld);
    
    bool init(const Rect& targetRectWorld);
    
    ComponentDistanceToTarget();
    
    ~ComponentDistanceToTarget();
    
    virtual void start() override;
    
    virtual void stop() override;
    
    virtual void pause() override;
    
    virtual void resume() override;
    
protected:
    virtual void onAdd() override;
    
    virtual void onRemove() override;
    
    void _checkTargetSchedule(float dt);
    
protected:
    Vec2 _targetPoint;
    
    Rect _targetRect;
    
    int _targetType = kTargetTypeNone;
};

