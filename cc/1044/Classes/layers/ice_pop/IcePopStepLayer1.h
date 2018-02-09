//
//  IcePopStepLayer1.h
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/3.
//
//

#ifndef __CCATS1044_BeachFoodParty__IcePopStepLayer1__
#define __CCATS1044_BeachFoodParty__IcePopStepLayer1__

#include <stdio.h>
#include <vector>
#include "cocos2d.h"
#include "StepLayer.h"
#include "GameLayer.h"
#include "KDGridView.h"
#include "KSSimpleAdapter.h"
#include "ToolSprite.h"


USING_NS_CC;

class IcePopStepLayer1: public StepLayer,public ToolSpriteDelegate
{
public:
    IcePopStepLayer1(GameLayer* gameLayer);
    IcePopStepLayer1();
    ~IcePopStepLayer1();
    static IcePopStepLayer1* create(GameLayer* gameLayer);
    
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
    
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
public:
    //各个材料进入动画
    void MaterialComing1();
    void MaterialComing2();
    void MaterialComing3();
    void MaterialComing4();
    
    //步骤控制器
    void stepControl(float dt);
    
    virtual void onEnter();
    virtual void onExit();
    
public:
    //存放杯子的容器
    Vector<Sprite*> m_vTypes;
    
    KDGridView* m_pGirdView;
    KSSimpleAdapter* m_pAdapter;
    MenuItemSprite* m_pCurrentSprite;
    
    ToolSprite* m_pIce;
    bool m_bNext;
    int m_iTouchIndex;
    ParticleSystemQuad* chooseParticle;

};


#endif /* defined(__CCATS1044_BeachFoodParty__IcePopStepLayer1__) */
