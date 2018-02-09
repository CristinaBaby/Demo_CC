//
//  SnowStepLayer3.h
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/8.
//
//

#ifndef __CCATS1044_BeachFoodParty__SnowStepLayer3__
#define __CCATS1044_BeachFoodParty__SnowStepLayer3__

#include <stdio.h>
#include <vector>
#include "cocos2d.h"
#include "StepLayer.h"
#include "GameLayer.h"
#include "KDGridView.h"
#include "KSSimpleAdapter.h"
#include "ToolSprite.h"


USING_NS_CC;

class SnowStepLayer3: public StepLayer,public ToolSpriteDelegate
{
public:
    SnowStepLayer3(GameLayer* gameLayer);
    SnowStepLayer3();
    ~SnowStepLayer3();
    static SnowStepLayer3* create(GameLayer* gameLayer);
    
private:
    virtual bool init();
    bool initUI();
    
    void initData();
public:
    
    void onBtnCallBack(Ref* sender);
    void typeItemClick(int tag,MenuItem* menuItem,int index);
    
    void onEnterTransitionDidFinish();
    
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    
    // 监听Android返回键事件
    virtual void onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event);
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);
public:
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchMove(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index);
    
    //步骤控制器
    void stepControl(float dt);
    
    void spoonAction();
    void chooseAction();
    void spoonMiss();
    
    void nextStep();
    
    virtual void onEnter();
    virtual void onExit();
    
public:
    //存放杯子的容器
    Vector<Sprite*> m_vTypes;
    
    Sprite* m_pMachine;
    ToolSprite* m_pBox;
    Sprite* m_pIceMachine;
    Sprite* m_pIceCup;
    Sprite* m_pCup;
    Sprite* m_pArrow;
    ToolSprite* m_pSpoon;
};

#endif /* defined(__CCATS1044_BeachFoodParty__SnowStepLayer3__) */
