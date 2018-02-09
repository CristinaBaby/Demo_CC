//
//  ParabolaTo.h
//  COOK0032_GumballCandymaker
//
//  Created by wusonglin1 on 15/10/8.
//
//

#ifndef __COOK0032_GumballCandymaker__ParabolaTo__
#define __COOK0032_GumballCandymaker__ParabolaTo__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

//抛物线运动
class ParabolaTo
{
public:
    /*创建一个抛物线动作
     参数：
     t                   时间
     startPoint      开始点
     endPoint        结束点
     height          高度（影响抛物线的高度）
     angle           角度（贝塞尔曲线两个控制点与y轴的夹角，直接影响精灵的抛出角度）
     */
    static BezierTo* create(float t, Vec2 startPoint, Vec2 endPoint, float height = 0, float angle = 60);
};
#endif /* defined(__COOK0032_GumballCandymaker__ParabolaTo__) */
