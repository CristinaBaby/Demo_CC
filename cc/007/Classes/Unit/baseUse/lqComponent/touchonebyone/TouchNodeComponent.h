//
//  TouchNodeComponent.hpp
//  lqtest
//
//  Created by luotianqiang1 on 15/11/9.
//
//

#ifndef TouchNodeComponent_hpp
#define TouchNodeComponent_hpp

#include "TouchComponent.h"
class TouchNodeComponent: public TouchComponent{
    DECLARE_COMPONENT_DYNCLASS(TouchNodeComponent);
public:
    TouchNodeComponent();
protected:
    virtual bool touchBeganLis(Touch*,Event*);
    virtual void touchMoveLis(Touch*,Event*);
    virtual void touchEndLis(Touch*,Event*);
    virtual void touchCanceLis(Touch*,Event*);
};

#endif /* TouchNodeComponent_hpp */
