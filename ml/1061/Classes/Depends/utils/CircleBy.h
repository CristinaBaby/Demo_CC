//
//  CircleBy.h
//  CrazyCalculate
//
//  Created by tangbowen on 14-12-26.
//
//

#ifndef __CrazyCalculate__CircleBy__
#define __CrazyCalculate__CircleBy__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class CircleBy : public ActionInterval {
    
public:
    CircleBy();
    virtual ~CircleBy();
    
public:
    //tm代表圆周运动的时间，circleCenter代表圆心，半径是根据精灵当前的位置和圆心的距离计算出来的，不需要作为参数传递进去，第三个参数代表角度数，来确定当前节点在圆周运动中的位置
    static CircleBy * create(float tm, Vec2 circleCenter, float initRandians, float targetRandians);
    //初始化参数
    bool init(float tm, Vec2 circleCenter, float initRandians, float targetRandians);
    //从父类继承需要覆写的方法
    virtual void update(float);
    //确定执行动作的节点以后调用的方法
    virtual void startWithTarget(Node *target);
    //动作翻转
    virtual CircleBy * reverse() const;
    //克隆一个相同的动作
    virtual CircleBy * clone() const;
    //设置旋转多少度，默认做的圆周运动是一个整圆
    void setRadians(float radians);
    
private:
    //圆心
    Vec2    _circleCenter;
    //半径
    double   _radius;
    //每帧需要转过的弧度数
    double   _radians;
    //初始的角度数
    double   _radiansValue;
    
    double   _targetRandians;
    //旋转角度，默认是360，也就是做一个完整的圆周运动
    double   _maxRadians;
    //刷新次数
    int     _times;


};

#endif /* defined(__CrazyCalculate__CircleBy__) */
