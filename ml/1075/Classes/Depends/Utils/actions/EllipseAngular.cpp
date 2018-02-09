//
//  EllipseAngularAction.cpp
//  ActionTest-x
//
//  Created by renhong on 7/15/14.
//
//

#include "EllipseAngular.h"


EllipseAngular *EllipseAngular::create(float dt, float a, float b, CCPoint center){
    EllipseAngular *pAction = new EllipseAngular(a, b, center);
    pAction->initWithDuration(dt);
    pAction->autorelease();
    
    return pAction;
}

EllipseAngular::EllipseAngular(float a, float b, CCPoint center){
    _a = a;
    _b = b;
    _center = center;
}

void EllipseAngular::update(float t){
    float x = 0, y = 0;
    float angle = M_PI * 2 * t;//椭圆中心与当前运动点连线角度
    float k = tanf(angle);//椭圆中心与当前运动点连线斜率
    //联立直线标准方程（y = kx）与椭圆标准方程（x^2 / a^2 + y^2 / b^2 = 1）求当前坐标点
    if (t <= 0.25) {//第一象限
        x = (_a * _b) / sqrt(pow(k, 2) * pow(_a, 2) + pow(_b, 2));
        y = k * x;
    }
    else if(t <= 0.5){//第二象限
        x = -(_a * _b) / sqrt(pow(k, 2) * pow(_a, 2) + pow(_b, 2));
        y = k * x;
    }
    else if(t <= 0.75){//第三象限
        x = -(_a * _b) / sqrt(pow(k, 2) * pow(_a, 2) + pow(_b, 2));
        y = k * x;
    }
    else{//第四象限
        x = (_a * _b) / sqrt(pow(k, 2) * pow(_a, 2) + pow(_b, 2));
        y = k * x;
    }
    CCPoint position = ccpAdd(_center, ccp(x, y));
    this->getTarget()->setPosition(position);
}