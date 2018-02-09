//
//  IcePopStepLayer3.h
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/6.
//
//

#ifndef __CCATS1044_BeachFoodParty__IcePopStepLayer3__
#define __CCATS1044_BeachFoodParty__IcePopStepLayer3__

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

class IcePopStepLayer3: public StepLayer,public ToolSpriteDelegate
{
public:
    IcePopStepLayer3(GameLayer* gameLayer);
    IcePopStepLayer3();
    ~IcePopStepLayer3();
    static IcePopStepLayer3* create(GameLayer* gameLayer);
    
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
  
    //擦拭层的回调，利用scheduleOnce
    void getScribbleCallback(float ft);
    
    
    
    //步骤控制器
    void stepControl(float dt);
    
    void closeDoor();
    void timeAction();
    void startTimeAction(float f);
    void handAction();
    void handAction1();
    void frostAction();
    
    //擦拭完成的回调
    void scribbleOver(Ref*);
    
    void scribbleTouchBegan(Ref*);
    
    virtual void onEnter();
    virtual void onExit();
 
    void nextStep();
   
public:
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index);
public:
    
    //可拖动的冰糕
    ToolSprite* m_pMoule;
    Sprite* m_pDoor;
    Sprite* m_pRefrigerator;
    Sprite* m_pShadow;
    Sprite* m_pIce;
    Sprite* m_pFrost;
    
    Sprite* m_pTimeBg;
    Sprite* m_pTimeNum;
    int m_iTimeNum;
    //可擦拭的精灵
    ScribbleTouchNode* m_pScribbleFreezerDoor;
    
};
#endif /* defined(__CCATS1044_BeachFoodParty__IcePopStepLayer3__) */
