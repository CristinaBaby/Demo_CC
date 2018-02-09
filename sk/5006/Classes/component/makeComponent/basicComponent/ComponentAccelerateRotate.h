//
//  ComponentAcceleteRotate.hpp
//
//  Created by tanshoumei on 1/26/16.
//
//

#pragma once

#include <stdio.h>
#include "component/makeComponent/basicComponent/ComponentBase.h"

/// 根据重力感应旋转
class ComponentAccelerateRotate: public ComponentBase
{
public:
    static ComponentAccelerateRotate* create(float endRotation);
    
    bool init(float endRotation);
    
    ComponentAccelerateRotate();
    
    virtual void start() override;
    
    virtual void pause() override;
    
    virtual void resume() override;
    
    virtual void stop() override;
    
    /**
     *  call back
     */
    std::function<void()> componentAccelerateRotateDoneCall = nullptr;
    std::function<void(cocos2d::Acceleration *acceleration, cocos2d::Event * e)> componentAccCall = nullptr;
    
    CC_SYNTHESIZE(float, _startRotation, StartRotation);
    
    CC_SYNTHESIZE(float, _endRotation, EndRotation);
    
    /**
     *  允许的误差角度。比如目标是90度，误差为10度，那转到80也算完成，然后会自动调整到目标角度。
     *  默认值为0
     */
    CC_SYNTHESIZE(float, _deviationRotation, DeviationRotation);
    
protected:
    
    virtual void onAdd() override;
    
    virtual void onRemove() override;
    
    void _acclerateCall(Acceleration *, Event *);
    
protected:
    
    EventListenerAcceleration* _listenerAccleration = nullptr;
    
private:
    float _minRotationForRotate;
    float _maxRotationForRotate;
};

