//
//  AccleMoveComponent.hpp
//  lqtest
//
//  Created by luotianqiang1 on 15/11/3.
//
//

#ifndef AccleMoveComponent_hpp
#define AccleMoveComponent_hpp

#include "AccleComponent.h"
class AccleMoveComponent: public AccleComponent{
    DECLARE_COMPONENT_DYNCLASS(AccleMoveComponent);
    CC_SYNTHESIZE(Vec2, _originalPos, OriginalPos);
public:
    AccleMoveComponent();
protected:
    virtual void  dispatcherAccelEvent(Acceleration * _acceleration, Event *);
};

#endif /* AccleMoveComponent_hpp */
