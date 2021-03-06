//
//  IceCreamStepLayer2.h
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/2.
//
//

#ifndef __CCATS1044_BeachFoodParty__IceCreamStepLayer2__
#define __CCATS1044_BeachFoodParty__IceCreamStepLayer2__

#include <stdio.h>
#include <vector>
#include "cocos2d.h"
#include "StepLayer.h"
#include "GameLayer.h"
#include "KDGridView.h"
#include "KSSimpleAdapter.h"
#include "ToolSprite.h"
#include "ScribbleTouchNode.h"

USING_NS_CC;

class IceCreamStepLayer2: public StepLayer,public ToolSpriteDelegate
{
public:
    IceCreamStepLayer2(GameLayer* gameLayer);
    IceCreamStepLayer2();
    ~IceCreamStepLayer2();
    static IceCreamStepLayer2* create(GameLayer* gameLayer);
    
private:
    virtual bool init();
    bool initUI();
    
    void initData();
public:
    
    void onBtnCallBack(Ref* sender);
    void typeItemClick(int tag,MenuItem* menuItem,int index);
    void freezerAction();
    void chooseAction();
    void handAction();
    void onEnterTransitionDidFinish();
    
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    
    //擦拭层的回调，利用scheduleOnce
    void getScribbleCallback(float ft);
    
    //擦拭完成的回调
    void scribbleOver(Ref*);
    
    void scribbleTouchBegan(Ref*);
    
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
    
    virtual void onEnter();
    virtual void onExit();
    
    void nextStep();
public:
    //存放杯子的容器
    Vector<Sprite*> m_vTypes;
    
    KDGridView* m_pGirdView;
    KSSimpleAdapter* m_pAdapter;
    MenuItemSprite* m_pCurrentSprite;
    
    Sprite* m_pFreezerDoorBottom;
    Sprite* m_pFreezerDoorTop;
    //可擦拭的精灵
    ScribbleTouchNode* m_pScribbleFreezerDoor;
    
    bool m_bIsFirst;
    
};





#endif /* defined(__CCATS1044_BeachFoodParty__IceCreamStepLayer2__) */
