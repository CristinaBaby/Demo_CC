//
//  OreoSL_DecorationLayer.h
//  OreoMaker
//
//  Created by wusonglin1 on 14-11-3.
//
//

#ifndef __OreoMaker__SL_DecorationLayer__
#define __OreoMaker__SL_DecorationLayer__

#include <iostream>
#include "cocos2d.h"
#include "AdapterScreen.h"
#include "BaseLayer.h"
#include "Utils.h"
#include "SceneManager.h"
#include "KDGridView.h"
#include "KSDecorationAdapter.h"
#include "UnitScrollView.h"
#include "Dialog.h"

#include "extensions/cocos-ext.h"
#include "ActionUtils.h"
//#include "WriteTouchNode.h"

using namespace cocos2d::extension;

USING_NS_CC;
using namespace std;

class GameLayer;

class SL_DecorationLayer:public BaseLayer{

public:
    SL_DecorationLayer();
    ~SL_DecorationLayer();
public:
    static SL_DecorationLayer* create();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    bool initUI();
    bool  initData();
public:
    void onTypeCallback(Ref* sender, Control::EventType controlEvent);
    void onTypePageCallback(Ref* sender, Control::EventType controlEvent);
    void _onTypePageCallback(Ref* sender, Control::EventType controlEvent);
    void onBtnCallBack(Ref* sender);
    
    virtual void scrollViewTouchBegin(Vec2 worldPoint);
	virtual void scrollViewWillScroll();
    void typeItemClick(int tag,MenuItem* menuItem,int index);
    void onUnitCallback(int tag,MenuItem* menuItem,int index);
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    // 监听Android返回键事件
    virtual void onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event);
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);

    void startAnimtion();
    void showBread();
    
    void unitScrollViewAction(bool b);
  
    void showHand();
    
    void showIconBg();
    void hideIconBg();
    
private:
    
    Layer* m_pDecLayer;

   // Sprite* m_pFruitSprite;
    Vector<Sprite*> m_vTypes;
    KDGridView* m_pGirdView;
    KSDecorationAdapter* m_pAdapter;
    UnitScrollView* m_pUnitScrollView;
    MenuItemSprite* m_pCurrentSprite;
    Sprite* m_pUnitScrollbgsprite;

    Sprite* m_pCup;
    Sprite* pBgSprite;
    Sprite* m_pCake;
    
    ui::Button* resetBtn;
//    ui::Button* upBtn;
//    ui::Button* downBtn;
    ui::Button* nextBtn;
    Sprite* m_pItemBg;
    
    Sprite* m_pPlate;
    
    Sprite* m_pTapTitle;

    int m_pIndex;
    //判断是否接受点击事件
    bool m_bIsTouch;
    bool m_bIsFirst;
    bool m_bIsShowUnit;
    float m_unitScrollViewX;
    
//    WriteTouchNode* m_pWriteTouchNode;
//    ToolSprite* m_pSause;
    Vec2 posIconBg;
    
};

#endif /* defined(__OreoMaker__OreoSL_DecorationLayer__) */
