//
//  InertiaComponent.cpp
//  candyfactory
//
//  Created by luotianqiang1 on 16/4/7.
//
//

#include "InertiaComponent.h"

const int InertiaComponent::cacheCount = 16;

IMPLEMENT_COCOS2DX_CLASS(InertiaComponent);
InertiaComponent::InertiaComponent():
_invokeAction(nullptr)
,isInvoking(false)
,currentSpeed(0)
,time(0)
,currentIndex(0)
,updateFunc(nullptr)
,moveCount(0)
,moveDistance(0)
{
    caches = vector<float>(cacheCount,0);
}
InertiaComponent::~InertiaComponent(){
    CC_SAFE_RELEASE_NULL(_invokeAction);
}

void InertiaComponent::onAdd(){
    LQComponent::onAdd();
    if(!isSchedule(schedule_selector(InertiaComponent::updateS)))
        schedule(schedule_selector(InertiaComponent::updateS), 0.0);
}

 void InertiaComponent::onRemove(){
     if(isSchedule(schedule_selector(InertiaComponent::updateS)))
         unSchedule(schedule_selector(InertiaComponent::updateS));
     LQComponent::onRemove();
}

void InertiaComponent::Invoke(float percent){
    if(isEnabled()&& _invokeAction != nullptr){
        isInvoking = true;
        updateFunc = CC_CALLBACK_1(InertiaComponent::checkSpeed, this);
      
        if(time != 0){
            currentIndex++;
            currentIndex =  currentIndex%cacheCount;
            caches[currentIndex] = percent/time;
            auto sum = std::accumulate(caches.begin(), caches.end(), 0.0);
            auto ttt = sum/caches.size();
            auto temps = caches;
            std::sort(temps.begin(), temps.end(), [ttt](float a,float b){
                return abs(ttt-a)<abs(ttt-b);
            });
            auto sum2 = std::accumulate(temps.begin(), temps.begin() +6, 0.0);
            currentSpeed = sum2/6;
            this->dispatcherEventWithData(InertiaInvoking, currentSpeed);
          //  moveDistance+= currentSpeed;
            CCLOG("currentSpeed%f",currentSpeed);
        }
        _invokeAction->startWithTarget(_owner);
        _invokeAction->update(currentSpeed*time);
        time = 0;
    }
}

void InertiaComponent::stopInvoke(){
    isInvoking = false;
    time = 0;
    currentIndex = 0;
    caches = vector<float>(cacheCount,0);
    updateFunc = CC_CALLBACK_1(InertiaComponent::inertia, this);
    moveDistance = 0;
    moveCount = 0;
}

void InertiaComponent::inertia(float time){
    if(!isInvoking&&abs(currentSpeed*time)>0.01){
        currentSpeed = currentSpeed*0.98;
         _invokeAction->startWithTarget(_owner);
        _invokeAction->update(currentSpeed*time);
        this->dispatcherEventWithData(Inertiaing, currentSpeed*time);
    }else {
        if(!isInvoking){
             updateFunc = nullptr;
            this->dispatcherEvent(InertiaStop);
        }
    }
}

void InertiaComponent::updateS(float t){
    if(isEnabled()&& updateFunc != nullptr){
        updateFunc(t);
    }
}

void InertiaComponent::checkSpeed(float time){
        this->time+=time;
}
