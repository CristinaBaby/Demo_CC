//
//  ActionHelper.h
//  PizzaMaker
//
//  Created by luotianqiang1 on 3/9/15.
//
//

#ifndef __PizzaMaker__ActionHelper__
#define __PizzaMaker__ActionHelper__

#include<iostream>
#include "cocos2d.h"
USING_NS_CC;


class ActionHelper {
public:
    enum class  ShowDirection {
        show_from_top,
        show_from_bottom,
        show_from_left,
        show_from_right
    };
public:
   static void setStartPostion(Node* node, Vec2 endPosiont,ShowDirection type);
    static void showBackInOut(Node* node, Vec2 endPosiont,ShowDirection type, std::function<void()> actionEnd = nullptr,float time=0.75);
   static void showBouce(Node* node, Vec2 endPosiont,ShowDirection type, std::function<void()> actionEnd = nullptr,float time=1);
   static void show(Node* node, Vec2 endPosiont,ShowDirection type, std::function<void()> actionEnd = nullptr,float time = 1,float percent = 0.8f);
    static void showRote(Node* node, Vec2 endPosiont,ShowDirection type, std::function<void()> actionEnd = nullptr,float time = 1);
    
   static void hide(Node* node,ShowDirection type, std::function<void()> actionEnd = nullptr,bool _isRemove = true,float _time = 0.6);
   static void changeFade(Sprite* preSprite, Sprite* curentSprite);
   static void changeBouce(Sprite* preSprite, Sprite* curentSprite);
   static ActionInterval* createShakeAction(float distance = 5,float time = 0.05f);
   static ActionInterval* createRotePrompt();
   static ActionInterval* createRoteAction(float rote = 10 ,float time = 0.18);
   static ActionInterval* createMovePrompt();
   static ActionInterval* createUpDownPrompt();
  static ActionInterval* createScaleAction();
    static ActionInterval* createJumpForever();
     static ActionInterval* createJumpScale();
   static void runMoveHideAction(Node* node, Vec2 startPos,Vec2 endPo,float time = 0.8f);
    static void runBezierMoveHideAction(Node* node, Vec2 startPos,Vec2 endPo,float time = 1.3);
    static void runRoteHideAction(Node* node, float _endRote = -45.0f,float time = 0.8f);
    static void runMoveHideAction2(Node* node);
    static   ActionInterval *getJellyAction();
    static ActionInterval *createRollJelly();
    static void delayFunc(float _time,Node*,std::function<void()>);
    static void showTitle(Node*);
    static int titleActionTag;
    static void showScaleArrow(Node*);
    
};

#endif /* defined(__PizzaMaker__ActionHelper__) */
