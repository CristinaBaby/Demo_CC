//
//  ActionUtils.h
//  MLABS1040_ItaliaFood
//
//  Created by wusonglin1 on 15-1-5.
//
//

#ifndef __MLABS1040_ItaliaFood__ActionUtils__
#define __MLABS1040_ItaliaFood__ActionUtils__

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

class ActionUtils : public Ref {
    
public:
    
    
    //圆形按钮滚动效果，锚点设置为中点
    static void roll(Node* node,Vec2,float pTime = KAppearTime,int circle = 1,CallFunc* func = nullptr);
    //精灵掉落弹性效果
    static void dropOut(Node* node,Vec2 vec2,float pTime = KAppearTime,CallFunc* func = nullptr);
    
    static void moveBy(Node* node,Vec2 vec2,float pTime = KAppearTime,CallFunc* func = nullptr);
    
    static void moveTo(Node* node,Vec2 vec2,float pTime = KAppearTime,CallFunc* func = nullptr);
    
    static void scaleTo(Node* node,float f,float pTime = KAppearTime,CallFunc* func = nullptr);
    
    static void moveByEaseBack(Node* node,Vec2 vec2,float pTime = KAppearTime,CallFunc* func = nullptr);
    
    static void moveByElasticOut(Node* node,Vec2 vec2,float pTime = KAppearTime,CallFunc* func = nullptr);
    
     static void moveByElasticIn(Node* node,Vec2 vec2,float pTime = KAppearTime,CallFunc* func = nullptr);
    
    static void moveToElasticOut(Node* node,Vec2 vec2,float pTime = KAppearTime,CallFunc* func = nullptr);
    
    static void moveToElasticIn(Node* node,Vec2 vec2,float pTime = KAppearTime,CallFunc* func = nullptr);
    
    static void fadeOut(Node* node,float pTime = KAppearTime,CallFunc* func = nullptr);
    
    static void fadeIn(Node* node,float pTime = KAppearTime,CallFunc* func = nullptr);
    //Node的延时
    static void delayTimeAction(Node* node,float pTime = KAppearTime,CallFunc* func = nullptr);
    
    static void shakeNode(Node *pNode, float pScale, int lRepeatCount,CallFunc* func = nullptr);
    
    static void shakeCup(Node *pNode, float pScale, int lRepeatCount,CallFunc* func = nullptr);
    
    static void flashAction(Node *pNode,int lRepeatCount,CallFunc* func = nullptr);
    
    static void zoomAction(Node *pNode, int lRepeatCount,CallFunc* func = nullptr);
    
    /*
     *根据HSV的值，返回纹理对象
     *
     */
    static Texture2D* getShapeScribbleTexture(__String* filename,int h, int s, int v);

    static void jumpAction(Node *pNode, int lRepeatCount,CallFunc* func = nullptr,float delayTime = 0.0);
    
   
};
#endif /* defined(__MLABS1040_ItaliaFood__ActionUtils__) */
