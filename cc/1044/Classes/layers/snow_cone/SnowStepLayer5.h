//
//  SnowStepLayer5.h
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/8.
//
//

#ifndef __CCATS1044_BeachFoodParty__SnowStepLayer5__
#define __CCATS1044_BeachFoodParty__SnowStepLayer5__

#include <stdio.h>
#include <vector>
#include "cocos2d.h"
#include "StepLayer.h"
#include "GameLayer.h"
#include "KDGridView.h"
#include "KSSimpleAdapter.h"
#include "ToolSprite.h"
#include "MyPaintSprite.h"

USING_NS_CC;

class SnowStepLayer5: public StepLayer,public ToolSpriteDelegate
{
public:
    SnowStepLayer5(GameLayer* gameLayer);
    SnowStepLayer5();
    ~SnowStepLayer5();
    static SnowStepLayer5* create(GameLayer* gameLayer);
    
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
    
    void titleAction();
    void knifeAction();
    void clickAction();
    
    virtual void onEnter();
    virtual void onExit();
    
public:
    //存放杯子的容器
    Vector<Sprite*> m_vTypes;
    
    KDGridView* m_pGirdView;
    KSSimpleAdapter* m_pAdapter;
    MenuItemSprite* m_pCurrentSprite;
    
//    Vector<<#class T#>>
   
    Sprite* m_pCup;
//    Sprite* m_pIceCup;
    ToolSprite* m_pSyrup;
    
    vector<Scribble*> m_vSyrupPaint;
    
    RenderTexture* _rtx;
    
    //防止连续点击，节省资源
    bool m_bClick;
    bool m_bNext;
//    Scribble* paint;
};

#endif /* defined(__CCATS1044_BeachFoodParty__SnowStepLayer5__) */
