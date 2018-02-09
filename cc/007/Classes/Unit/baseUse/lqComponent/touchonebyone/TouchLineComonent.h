//
//  TouchLineComonent.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/20.
//
//

#ifndef TouchLineComonent_hpp
#define TouchLineComonent_hpp

#include "TouchNodeComponent.h"
/**
 只在一条线上响应事件
 */
class TouchLineComonent: public TouchNodeComponent {
    DECLARE_COMPONENT_DYNCLASS(TouchLineComonent);
    CC_SYNTHESIZE(Vec2, _direction, Direction);
public:
    virtual ~TouchLineComonent();
    TouchLineComonent();
protected:
   virtual void touchMoveLis(Touch*_touch,Event*);
    virtual void touchEndLis(Touch*,Event*);
    virtual void touchCanceLis(Touch*,Event*);
};
#endif /* TouchLineComonent_hpp */
