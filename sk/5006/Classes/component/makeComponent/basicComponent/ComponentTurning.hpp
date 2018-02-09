//
//  ComponentTurning.hpp
//  BHUG1062
//
//  Created by liji on 16/4/20.
//
//

#ifndef ComponentTurning_hpp
#define ComponentTurning_hpp

#include <stdio.h>
#include "component/makeComponent/basicComponent/ComponentBase.h"

typedef enum
{
    kComponentTurningDirectionClockwise,
    kComponentTurningDirectionAnticlockwise,
}
kComponentTurningDirection;



class ComponentTurning: public ComponentBase
{
public:
    /**
     *  限制方向转动
     */
    CREATE_FUNC(ComponentTurning);
    
    ComponentTurning();
    ~ComponentTurning(){};
    
    void onAdd();
//    void start() override;
    void stop() override;
//    void pause() override;
//    void resume() override;
    
    std::function<void(float totalAngle)> _turningCallback = nullptr;
    std::function<void()> _turningStop = nullptr;

    
private:
    bool onTouchBegan(Touch* touch, Event* event);
    
    void onTouchMoved(Touch* touch, Event* event);
    
    void onTouchEnded(Touch* touch, Event* event);
    
    void onTouchCancelled(Touch* touch, Event* event);

    
protected:
    float _orignalRotate = 0.f;
    
    CC_SYNTHESIZE(kComponentTurningDirection, _direction, Direction);
    

};

#endif /* ComponentTurning_hpp */
