//
//  ParabolaTo.cpp
//  COOK0032_GumballCandymaker
//
//  Created by wusonglin1 on 15/10/8.
//
//

#include "ParabolaTo.h"

BezierTo* ParabolaTo::create(float t, Vec2 startPoint, Vec2 endPoint, float height /* = 0 */, float angle /* = 60 */){
    
    // 把角度转换为弧度
    float radian = angle*3.14159/180.0;
    // 第一个控制点为抛物线左半弧的中点
    float q1x = startPoint.x+(endPoint.x - startPoint.x)/4.0;
    Vec2 q1 = Vec2(q1x, height + startPoint.y+cos(radian)*q1x);
    // 第二个控制点为整个抛物线的中点
    float q2x = startPoint.x + (endPoint.x - startPoint.x)/2.0;
    Vec2 q2 = Vec2(q2x, height + startPoint.y+cos(radian)*q2x);
    
    //曲线配置
    ccBezierConfig cfg;
    cfg.controlPoint_1 = q1;
    cfg.controlPoint_2 = q2;
    cfg.endPosition = endPoint;
    //使用CCEaseInOut让曲线运动有一个由慢到快的变化，显得更自然
//    return CCEaseInOut::create(,0.5);
    return BezierTo::create(t,cfg);
}