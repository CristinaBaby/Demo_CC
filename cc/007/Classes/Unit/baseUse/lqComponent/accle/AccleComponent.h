//
//  AccleComplent.hpp
//  lqtest
//
//  Created by luotianqiang1 on 15/11/2.
//
//

#ifndef AccleComponent_h
#define AccleComponent_h

/**
 *重力感应的Component
 *
 */
#include "LQComponent.h"
class AccleComponent: public LQComponent{
    DECLARE_COMPONENT_DYNCLASS(AccleComponent);
    CC_SYNTHESIZE(float, _magnification, Magnification);
    CC_SYNTHESIZE(Vec3, _changeInterval, ChangeInterval);//调用频率
public:
    AccleComponent();
    virtual ~AccleComponent();
protected:
    virtual EventListener* createLis();
    virtual void accelEvent(Acceleration *, Event *);
    virtual void dispatcherAccelEvent(Acceleration *, Event *);
private:
    Vec3 _preAccel;
};

#endif /* AccleComplent_h */
