//
//  TouchMoveRote.hpp
//  lqtest
//
//  Created by luotianqiang1 on 15/11/5.
//
//

#ifndef TouchMoveRote_hpp
#define TouchMoveRote_hpp

#include "TouchMoveComponent.h"


class TouchMoveRote : public TouchMoveComponent{
    DECLARE_COMPONENT_DYNCLASS(TouchMoveRote);
public:
    TouchMoveRote();
    virtual ~TouchMoveRote();
protected:
    enum  EventType {
        TYPEMOVE,
        TYPEROTE,
        TYPENULL
    };
protected:
    virtual bool touchBeganLis(Touch*,Event*);
    virtual void touchMoveLis(Touch*,Event*);
protected:
    EventType _eventType;
    vector<Vec2> points;
};

#endif /* TouchMoveRote_hpp */
