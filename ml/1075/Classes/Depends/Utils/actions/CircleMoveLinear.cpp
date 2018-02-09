//
//  CircleMoveLinearAction.cpp
//  ActionTest-x
//
//  Created by renhong on 7/15/14.
//
//

#include "CircleMoveLinear.h"

CircleMoveLinear *CircleMoveLinear::create(float dt, float radius, Vec2 center,float angle){
    CircleMoveLinear *pAction = new CircleMoveLinear(radius, center,angle);
    pAction->initWithDuration(dt);
    pAction->autorelease();
    
    return pAction;
}

CircleMoveLinear::CircleMoveLinear(float radius, Vec2 center,float angle){
    _a = radius;
    _b = radius;
    _center = center;
    _angle = angle;
    log("======%f,%f",_center.x,_center.y);
}

void CircleMoveLinear::startWithTarget(Node *target)
{
    Ellipse::startWithTarget(target);
    _initPos = target->getPosition();
    Vec2 offset = Vec2(_initPos, _center);
    
    log("======%f,%f",_center.x,_center.y);
    log("======%f,%f",_initPos.x,_initPos.y);
    log("======%f,%f",offset.x,offset.y);
    _angleOrig = atan(fabs(offset.y / offset.x));
    _circleCount = fabsf(_angle/2/M_PI);
}

void CircleMoveLinear::update(float t){
    float x = 0, y = 0;
    float angle = _angleOrig+_angle * t;//椭圆中心与当前运动点连线角度
    float k = tanf(angle);//椭圆中心与当前运动点连线斜率
    //联立直线标准方程（y = kx）与椭圆标轨迹定义求当前坐标点
    int count = fabs(_angle * t/2/M_PI);
//    float offset = t-1.0*count/_circleCount;
    float offset = angle;
    if(offset>0){
        if (offset <= M_PI_2+2*M_PI*count) {//第一象限
            x = _a / sqrt(pow(k, 2) +1);
            y = k * x;
            log("==========1=====");
        }
        else if(offset <= M_PI+2*M_PI*count){//第二象限
            x = -_a / sqrt(pow(k, 2) +1);
            y = k * x;
            log("==========2=====");
        }
        else if(offset <= M_PI*1.5+2*M_PI*count){//第三象限
            x = -_a / sqrt(pow(k, 2) +1);
            y = k * x;
            log("==========3=====");
        }
        else{//第四象限
            x = _a / sqrt(pow(k, 2) +1);
            y = k * x;
            log("==========4=====");
        }
    }else{
        if (offset >= -M_PI_2-2*M_PI*count) {//第一象限
            x = _a / sqrt(pow(k, 2) +1);
            y = k * x;
            log("==========1=====");
        }
        else if(offset >= -M_PI-2*M_PI*count){//第二象限
            x = -_a / sqrt(pow(k, 2) +1);
            y = k * x;
            log("==========2=====");
        }
        else if(offset >= -M_PI*1.5-2*M_PI*count){//第三象限
            x = -_a / sqrt(pow(k, 2) +1);
            y = k * x;
            log("==========3=====");
        }
        else{//第四象限
            x = _a / sqrt(pow(k, 2) +1);
            y = k * x;
            log("==========4=====");
        }
    }
//    if(_angle>0){
//        if (offset <= 0.25) {//第一象限
//            x = _a / sqrt(pow(k, 2) +1);
//            y = k * x;
//            log("==========1=====");
//        }
//        else if(offset <= 0.5){//第二象限
//            x = _a / sqrt(pow(k, 2) +1);
//            y = k * x;
//            log("==========2=====");
//        }
//        else if(offset <= 0.75){//第三象限
//            x = _a / sqrt(pow(k, 2) +1);
//            y = k * x;
//            log("==========3=====");
//        }
//        else{//第四象限
//            x = _a / sqrt(pow(k, 2) +1);
//            y = k * x;
//            log("==========4=====");
//        }
//        
//    }else{
//        if (offset <= 0.25) {//第一象限
//            x = _a / sqrt(pow(k, 2) +1);
//            y = k * x;
//            log("==========1=====");
//        }
//        else if(offset <= 0.5){//第二象限
//            x = _a / sqrt(pow(k, 2) +1);
//            y = k * x;
//            log("==========2=====");
//        }
//        else if(offset <= 0.75){//第三象限
//            x = _a / sqrt(pow(k, 2) +1);
//            y = k * x;
//            log("==========3=====");
//        }
//        else{//第四象限
//            x = _a / sqrt(pow(k, 2) +1);
//            y = k * x;
//            log("==========4=====");
//        }
//    }
//    log("==========time=====%f",t);
    Vec2 position = _center+ Vec2(x, y);
    this->getTarget()->setPosition(position);
}