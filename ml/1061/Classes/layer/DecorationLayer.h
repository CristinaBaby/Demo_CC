//
//  OreoDecorationLayer.h
//  OreoMaker
//
//  Created by wusonglin1 on 14-11-3.
//
//

#ifndef __OreoMaker__DecorationLayer__
#define __OreoMaker__DecorationLayer__

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
#include "WriteTouchNode.h"

using namespace cocos2d::extension;

USING_NS_CC;
using namespace std;

class GameLayer;

class DecorationLayer:public BaseLayer{

public:
    DecorationLayer();
    ~DecorationLayer();
public:
    static DecorationLayer* create();
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
    
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);

    void startAnimtion();
    void showBread();
    
    void unitScrollViewAction(bool b);
  
    void showHand();
    int checkTopper(int index);
    
    void onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event);
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
//    Sprite* m_pItemBg;
    
    Sprite* m_pPlate;
    
    

    int m_pIndex;
    //判断是否接受点击事件
    bool m_bIsTouch;
    bool m_bIsFirst;
    bool m_bIsShowUnit;
    float m_unitScrollViewX;

};

#endif /* defined(__OreoMaker__OreoDecorationLayer__) */
