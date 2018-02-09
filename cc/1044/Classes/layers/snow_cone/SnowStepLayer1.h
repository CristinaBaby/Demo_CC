//
//  SnowStepLayer1.h
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/7.
//
//

#ifndef __CCATS1044_BeachFoodParty__SnowStepLayer1__
#define __CCATS1044_BeachFoodParty__SnowStepLayer1__

#include <stdio.h>
#include <vector>
#include "cocos2d.h"
#include "StepLayer.h"
#include "GameLayer.h"
#include "KDGridView.h"
#include "KSSimpleAdapter.h"
#include "ToolSprite.h"


USING_NS_CC;

enum ToolType{
    KMachineCoverUp,
    KMachineCoverDown,
    KIceBag,
    KSwitchBtn,

};


class SnowStepLayer1: public StepLayer,public ToolSpriteDelegate
{
public:
    SnowStepLayer1(GameLayer* gameLayer);
    SnowStepLayer1();
    ~SnowStepLayer1();
    static SnowStepLayer1* create(GameLayer* gameLayer);
    
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
    //步骤控制器
    void stepControl(float dt);
    
    
    void titleAction();
    void showIceBag();
    void iceBagClicked();
    void showHand();
    void startMakeIce();
    void iceBagAction();
    void iceAction();
    
    virtual void onEnter();
    virtual void onExit();
    
    void nextStep();
    void nextStep1();
    
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index);
     virtual void onTouchMove(ToolSprite* toolSprite,Touch *pTouch);
    void handAction();
    
public:
    //存放杯子的容器
    Vector<Sprite*> m_vTypes;
    
//    KDGridView* m_pGirdView;
//    KSSimpleAdapter* m_pAdapter;
//    MenuItemSprite* m_pCurrentSprite;
    
    
    Sprite* m_pMachine;
    Sprite* m_pBox;
    Sprite* m_pHint;
    Sprite* m_pHand;
    Sprite* m_pIce;
    ToolSprite* m_pMachineCover;
    ToolSprite* m_pIceBag;
    ToolSprite* m_pSwitchBtn;
    
};







#endif /* defined(__CCATS1044_BeachFoodParty__SnowStepLayer1__) */
