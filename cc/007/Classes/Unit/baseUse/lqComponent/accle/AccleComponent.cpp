//
//  AccleComplent.cpp
//  lqtest
//
//  Created by luotianqiang1 on 15/11/2.
//
//

#include "AccleComponent.h"
IMPLEMENT_COCOS2DX_CLASS(AccleComponent);

AccleComponent::AccleComponent():_magnification(1){
    _preAccel = Vec3::ZERO;
    _changeInterval = Vec3(0.025f,0.0f,0.0f);
}
AccleComponent::~AccleComponent(){
    
}

EventListener* AccleComponent::createLis(){
     auto _accelLis = EventListenerAcceleration::create(std::bind(&AccleComponent::accelEvent,this,std::placeholders::_1,std::placeholders::_2));
     _dispatcher->addEventListenerWithSceneGraphPriority(_accelLis, _lisNode);
    return _accelLis;
}

void  AccleComponent::accelEvent(Acceleration * _acceleration, Event *event){
    auto _currentAccel = Vec3(_acceleration->x,_acceleration->y,_acceleration->z);
    auto _distance = _preAccel-_currentAccel;
    if(abs(_distance.x)>=_changeInterval.x&&abs(_distance.y)>=_changeInterval.y&&abs(_distance.z)>_changeInterval.z){
        _preAccel = _currentAccel;
        dispatcherAccelEvent(_acceleration,event);
    }
}

void AccleComponent::dispatcherAccelEvent(Acceleration *, Event *){
    
}