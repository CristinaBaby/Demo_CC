//
//  IceCreamStepLayer3.h
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/3.
//
//

#ifndef __CCATS1044_BeachFoodParty__IceCreamStepLayer3__
#define __CCATS1044_BeachFoodParty__IceCreamStepLayer3__

#include <stdio.h>
#include <vector>
#include "cocos2d.h"
#include "StepLayer.h"
#include "GameLayer.h"
#include "KDGridView.h"
#include "KSSimpleAdapter.h"
#include "ToolSprite.h"
#include "ScribbleTouchNode.h"

enum ToolSpriteType{
    KSpoonNull,
    KSpoonEd,
    KBack,
    KNext,
    KICON,
};


USING_NS_CC;

class IceCreamStepLayer3: public StepLayer,public ToolSpriteDelegate,public KDGridViewScrollCallback
{
public:
    IceCreamStepLayer3(GameLayer* gameLayer);
    IceCreamStepLayer3();
    ~IceCreamStepLayer3();
    static IceCreamStepLayer3* create(GameLayer* gameLayer);
    
private:
    virtual bool init();
    bool initUI();
    
    void initData();
public:
    
    void onBtnCallBack(Ref* sender);
    void typeItemClick(int tag,MenuItem* menuItem,int index);
    void typeIceItemClick(int tag,MenuItem* menuItem,int index);
    
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
    
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchMove(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index);

    
    virtual void onScrollEnd(float offsetX,float offsetY);
    virtual void onScrolling(float offsetX,float offsetY);
    void startAction();
    //选择框显示动画
    void girdViewShowAction();
    void girdViewHideAction();
public:
    //存放杯子的容器
    Vector<Sprite*> m_vTypes;
    Vector<Sprite*> m_vIceTypes;
    
    KDGridView* m_pGirdView;
    KSSimpleAdapter* m_pAdapter;
    MenuItemSprite* m_pCurrentSprite;
    
    KDGridView* m_pIceGirdView;
    KSSimpleAdapter* m_pIceAdapter;
  
    Sprite* m_pGridViewBg;
    Sprite* m_pFreezerDoor;
    Sprite* m_pFrame;
//    Sprite* m_pCup;
    
    ToolSprite* m_pSpoon;
    ToolSprite* m_pBack;
    ToolSprite* m_pNext;
    ToolSprite* m_pIcon;
    Sprite* m_pGarbage;
    
    int m_iSpoonType;
    int m_iIceType;
    
    Rect m_rIceMoveRect;
    Rect m_rIceCupRect;
    
    Vec2 _positionSpoon;
    
    LayerColor* m_pLayerColor;
    bool m_bNext;
    bool m_bCupShow;
    Sprite* m_pCup1;
};
#endif /* defined(__CCATS1044_BeachFoodParty__IceCreamStepLayer3__) */
