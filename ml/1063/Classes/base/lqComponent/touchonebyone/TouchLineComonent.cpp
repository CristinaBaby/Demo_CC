//
//  TouchLineComonent.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/20.
//
//

#include "TouchLineComonent.h"
#include "LQMath.h"

IMPLEMENT_COCOS2DX_CLASS(TouchLineComonent);
TouchLineComonent::TouchLineComonent():
_direction(Vec2::ZERO){
    
}

TouchLineComonent::~TouchLineComonent(){
    
}

void TouchLineComonent::touchMoveLis(Touch*_touch,Event*){
    auto _shade = LQMath::LQgetShadow(_touch->getDelta(), _direction);
    dispatcherEventWithData(ComponentTouchMove,_shade);
}

void TouchLineComonent::touchEndLis(Touch*_touch,Event*_event){
    TouchNodeComponent::touchEndLis(_touch, _event);
}
void TouchLineComonent::touchCanceLis(Touch*_touch,Event*_event){
    TouchNodeComponent::touchCanceLis(_touch, _event);
}


