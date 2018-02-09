//
//  LQEllipse.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/20.
//
//

#include "LQEllipse.h"

LQEllipse* LQEllipse::create(Vec2 _center,float _a,float _b){
    auto _e = new LQEllipse();
    if(nullptr != _e && _e->initData(_center, _a, _b)){
        _e->autorelease();
        return _e;
    }
    CC_SAFE_RELEASE_NULL(_e);
    return nullptr;
}

bool LQEllipse::initData(Vec2 _center,float _a,float _b){
    this->_center = _center;
    this->_a = _a;
    this->_b = _b;
    return true;
}

bool LQEllipse::containsPoint(Vec2 _point){
    float tempx = _point.x-_center.x;
    float tempy = _point.y-_center.y;
    return tempx*tempx/(_a*_a) + tempy*tempy/(_b*_b)<=1;
}