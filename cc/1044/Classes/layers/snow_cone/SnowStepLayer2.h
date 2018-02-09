//
//  SnowStepLayer2.h
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/8.
//
//

#ifndef __CCATS1044_BeachFoodParty__SnowStepLayer2__
#define __CCATS1044_BeachFoodParty__SnowStepLayer2__

#include <stdio.h>
#include <vector>
#include "cocos2d.h"
#include "StepLayer.h"
#include "GameLayer.h"
#include "KDGridView.h"
#include "KSSimpleAdapter.h"
#include "ToolSprite.h"


USING_NS_CC;

class SnowStepLayer2: public StepLayer,public ToolSpriteDelegate
{
public:
    SnowStepLayer2(GameLayer* gameLayer);
    SnowStepLayer2();
    ~SnowStepLayer2();
    static SnowStepLayer2* create(GameLayer* gameLayer);
    
private:
    virtual bool init();
    bool initUI();
    
    void initData();
public:
    
    void onBtnCallBack(Ref* sender);
    void typeItemClick(int tag,MenuItem* menuItem,int index);
    void titleAction();
    void chooseAction();
    void onEnterTransitionDidFinish();
    
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    
    // 监听Android返回键事件
    virtual void onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event);
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);
public:
  
    
    //步骤控制器
    void stepControl(float dt);
    
    virtual void onEnter();
    virtual void onExit();
    
    void afterPurchase(EventCustom* e);
    
public:
    //存放杯子的容器
    Vector<Sprite*> m_vTypes;
    
    KDGridView* m_pGirdView;
    KSSimpleAdapter* m_pAdapter;
    MenuItemSprite* m_pCurrentSprite;
    
    Sprite* m_pCup;
    Sprite* m_pGirdViewBg;
    
    Vec2 m_pPos;
    
    bool m_bNext;
};
#endif /* defined(__CCATS1044_BeachFoodParty__SnowStepLayer2__) */
