//
//  CircleMoveLinearAction.h
//  ActionTest-x
//
//  Created by renhong on 7/15/14.
//
//

#ifndef __ActionTest_x__CircleMoveLinearAction__
#define __ActionTest_x__CircleMoveLinearAction__

#include "Ellipse.h"
/**
 *  沿着椭圆轨迹绕椭圆中心做匀速（线速度）运动
 */
class CircleMoveLinear : public Ellipse {
public:
    /**
     *  a: 椭圆长轴 b: 椭圆短轴 center: 椭圆中心点
     */
    static CircleMoveLinear *create(float dt, float radius, Vec2 center,float angle);
    
protected:
    CircleMoveLinear(float radius, Vec2 center,float angle);
    virtual void update(float t);
    void startWithTarget(Node *target);
    float _angle;
    float _orginPos;
    float _angleOrig;
    cocos2d::Vec2 _initPos;
    float _circleCount;
};

#endif /* defined(__ActionTest_x__CircleMoveLinearAction__) */
