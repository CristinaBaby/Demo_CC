//
//  LQCircle.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/19.
//
//

#include "LQCircle.h"


LQCircle::LQCircle():
_R(0.0f)
,_center(Vec2::ZERO){
    
}
 LQCircle* LQCircle::create(Vec2 v,float r){
     auto _r = new LQCircle;
     if(_r != nullptr&& _r->initData(v,r)){
         _r->autorelease();
         return _r;
     }
     CC_SAFE_RELEASE_NULL(_r);
     return nullptr;
}

bool LQCircle::containsPoint(Vec2 _point){
    float tempx = _point.x-_center.x;
    float tempy = _point.y-_center.y;
    return (tempx*tempx+tempy*tempy)<=_R*_R;
}

bool LQCircle::initData(Vec2 _v,float _r){
    this->_center = _v;
    this->_R = _r;
    return true;
}

float LQCircle::distanceToPoint(Vec2 _pos){
    return _pos.distance(_center) - +_R;
}