//
//  ActionManager.h
//  CCATS007
//
//  Created by liqiang on 15/9/22.
//
//

#ifndef __CCATS007__ActionManager__
#define __CCATS007__ActionManager__

#include <stdio.h>

//动作管理
#include <iostream>
#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

#define KAppearTime 1.0

/*
 EaseBounceOut    作用：让目标动作赋予反弹力，且以目标动作结束位子开始反弹
 EaseBounceInOut  作用：让目标动作赋予反弹力，且以目标动作起始与结束位子开始反弹
 EaseBackIn       作用：让目标动作赋予回力  ， 且以目标动作起点位置作为回力点
 EaseBackOut      作用：让目标动作赋予回力  ， 且以目标动作终点位置作为回力点
 EaseBackInOut    作用：让目标动作赋予回力  ， 且以目标动作起点和终点位置作为回力点
 EaseElasticIn    作用：让目标动作赋予弹性  ， 且以目标动作起点位子赋予弹性
 EaseElasticOut   作用：让目标动作赋予弹性  ， 且以目标动作终点位子赋予弹性
 EaseElasticInOut 作用：让目标动作赋予弹性  ，且以目标动作起点和终点位子赋予弹性
 EaseExponentialIn     让目标动作缓慢开始
 EaseSineIn       作用：动作由慢到快
 EaseSineOut      作用：动作由快到慢
 EaseSineInOut    作用：动作由慢到快再快到慢
 */

class AppearManager : public Ref
{
public:
    static void LeftAppear(Node* n,float time= KAppearTime,CallFunc* func = nullptr);
    
    static void RightAppear(Node* n,float time= KAppearTime,CallFunc* func = nullptr);
    
    static void TopAppear(Node* n,float time= KAppearTime,CallFunc* func = nullptr);
    
    static void BottomAppear(Node* n,float time= KAppearTime,CallFunc* func = nullptr);
    
    
    
    static void LeftDisAppear(Node* n,float time= KAppearTime,CallFunc* func = nullptr);
    
    static void RightDisAppear(Node* n,float time= KAppearTime,CallFunc* func = nullptr);
    
    static void TopDisAppear(Node* n,float time= KAppearTime,CallFunc* func = nullptr);
    
    static void BottomDisAppear(Node* n,float time= KAppearTime,CallFunc* func = nullptr);
    
};

#endif /* defined(__CCATS007__ActionManager__) */
