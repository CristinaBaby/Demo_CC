//
//  IcePopStepLayer2.h
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/6.
//
//

#ifndef __CCATS1044_BeachFoodParty__IcePopStepLayer2__
#define __CCATS1044_BeachFoodParty__IcePopStepLayer2__

#include <stdio.h>
#include <vector>
#include "cocos2d.h"
#include "StepLayer.h"
#include "GameLayer.h"
#include "KDGridView.h"
#include "KSSimpleAdapter.h"
#include "ToolSprite.h"


USING_NS_CC;

class IcePopStepLayer2: public StepLayer,public ToolSpriteDelegate
{
public:
    IcePopStepLayer2(GameLayer* gameLayer);
    IcePopStepLayer2();
    ~IcePopStepLayer2();
    static IcePopStepLayer2* create(GameLayer* gameLayer);
    
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
    //各个材料进入动画
    void MaterialComing1();
    void MaterialComing2();
    void MaterialComing3();
    void MaterialComing4();
    
    //步骤控制器
    void stepControl(float dt);
    
    void nextStep();
    
    virtual void onEnter();
    virtual void onExit();
    
    //绘制冰棍口味
    void initMoule();
    
    //冰棍口味定时器
    void updateMoule(float f);
    
    //冰棍口味定时器
    void updatePartic(float f);
public:
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index);
public:
    //存放果汁类型的容器
    Vector<Sprite*> m_vTypes;
    
    //存放冰棍的容器
    Vector<Sprite*> m_vTypesTaste;
    
    
    KDGridView* m_pGirdView;
    KSSimpleAdapter* m_pAdapter;
    Sprite* m_pCurrentSprite;
    
    //
    ToolSprite* m_pSwitch;
    
    float m_fOffset;
    
//    Sprite* m_pMoule;
    Sprite* m_pTastType;
//    ProgressTimer* m_pMouleTimer;
    
    //口味的类型
    int m_iTasteType;
    //口味进度条的TAG标记
    int m_iTimeTag;
    //口味进度条的百分数
    float m_fTimePercentage;
    
    ParticleSystemQuad* waterParticle;
    ParticleSystemQuad* chooseParticle;
    bool m_bIsShow;
    
    bool m_bIsFirst;
};


#endif /* defined(__CCATS1044_BeachFoodParty__IcePopStepLayer2__) */
