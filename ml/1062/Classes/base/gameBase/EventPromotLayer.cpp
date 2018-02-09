//
//  EventPromotLayer.cpp
//  backerychef
//
//  Created by luotianqiang1 on 15/6/2.
//
//

#include "EventPromotLayer.h"
std::vector<promotCallback*>  EventPromotLayer::callBacks;

bool EventPromotLayer::init(){
    EventLayer::init();
    clicke = nullptr;
    initWithColor(Color4B(0,0,0,0));
    mpListerner = nullptr;
    time = 5;
    _curent = nullptr;
    return  true;
}
bool  EventPromotLayer::touchBegine(Touch * pTouch, Event*pEvent){
    unschedule(schedule_selector(EventPromotLayer::schecFun));
     this->schedule(schedule_selector(EventPromotLayer::schecFun),time);
    return true;
}

void EventPromotLayer::touchEnd(Touch*pTouch,Event*pEvent){
    unschedule(schedule_selector(EventPromotLayer::schecFun));
    this->schedule(schedule_selector(EventPromotLayer::schecFun),time);
}
void EventPromotLayer::touchCancle(Touch*pTouch,Event*pEvent){
    unschedule(schedule_selector(EventPromotLayer::schecFun));
    this->schedule(schedule_selector(EventPromotLayer::schecFun),time);
}

void EventPromotLayer::setTime(float time){
    if(time>0)
        this->time = time;
}

void EventPromotLayer::schecFun(float){
    if(callBacks.size()>0&& _curent != nullptr && (&_curent)==(callBacks[callBacks.size()-1]))
            _curent();
}

void  EventPromotLayer::onEnterTransitionDidFinish(){
    EventLayer::onEnterTransitionDidFinish();
    mpListerner->setSwallowTouches(false);
     unschedule(schedule_selector(EventPromotLayer::schecFun));
    this->schedule(schedule_selector(EventPromotLayer::schecFun),time);
   
 
}
void  EventPromotLayer::onExitTransitionDidStart(){
    unschedule(schedule_selector(EventPromotLayer::schecFun));
    auto it = std::find(callBacks.begin(), callBacks.end(), &_curent);
    if(it != callBacks.end())
        callBacks.erase(it);
    EventLayer::onExitTransitionDidStart();
}

void EventPromotLayer::setCallbakc(promotCallback t){
    _curent = t;
    callBacks.push_back(&_curent);
}