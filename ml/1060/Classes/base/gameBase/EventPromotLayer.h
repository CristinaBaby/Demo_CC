//
//  EventPromotLayer.h
//  backerychef
//
//  Created by luotianqiang1 on 15/6/2.
//
//

#ifndef __backerychef__EventPromotLayer__
#define __backerychef__EventPromotLayer__

#include "EventLayer.h"
typedef std::function<void()>  promotCallback;

class EventPromotLayer : public EventLayer {
public:
    CREATE_FUNC(EventPromotLayer);
    bool init();
    bool touchBegine(Touch * pTouch, Event*pEvent);
    virtual void touchEnd(Touch*pTouch,Event*pEvent);
    virtual void touchCancle(Touch*pTouch,Event*pEvent);
    void setTime(float time);
protected:
    virtual void onEnterTransitionDidFinish();
    virtual void onExitTransitionDidStart();
    void schecFun(float);
    float time;
    static   std::vector<promotCallback*> callBacks;
    promotCallback _curent;
public:
    void setCallbakc(promotCallback);
};

#endif /* defined(__backerychef__EventPromotLayer__) */
