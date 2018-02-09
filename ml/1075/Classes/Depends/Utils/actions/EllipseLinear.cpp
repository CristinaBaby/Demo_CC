//
//  EllipseLinearAction.cpp
//  ActionTest-x
//
//  Created by renhong on 7/15/14.
//
//

#include "EllipseLinear.h"

EllipseLinear *EllipseLinear::create(float dt, float a, float b, CCPoint center){
    EllipseLinear *pAction = new EllipseLinear(a, b, center);
    pAction->initWithDuration(dt);
    pAction->autorelease();
    
    return pAction;
}

EllipseLinear::EllipseLinear(float a, float b, CCPoint center){
    _a = a;
    _b = b;
    _center = center;
}

void EllipseLinear::update(float t){
    float x = 0, y = 0;
    float angle = -M_PI * 2 * t;//椭圆中心与当前运动点连线角度
    float k = tanf(angle);//椭圆中心与当前运动点连线斜率
    //联立直线标准方程（y = kx）与椭圆标轨迹定义求当前坐标点
    if (t <= 0.25) {//第一象限
        x = _a / sqrt(1 + k * k);
        y = k * _b / sqrt(1 + k * k);
    }
    else if(t <= 0.5){//第二象限
        x = -_a / sqrt(1 + k * k);
        y = -k * _b / sqrt(1 + k * k);
    }
    else if(t <= 0.75){//第三象限
        x = -_a / sqrt(1 + k * k);
        y = -k * _b / sqrt(1 + k * k);
    }
    else{//第四象限
        x = _a / sqrt(1 + k * k);
        y = k * _b / sqrt(1 + k * k);
    }
    CCPoint position = ccpAdd(_center, ccp(x, y));
    this->getTarget()->setPosition(position);
}