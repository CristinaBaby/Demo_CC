//
//  IceCreamIceCreamDecorationLayer.h
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/10.
//
//

#ifndef __CCATS1044_BeachFoodParty__IceCreamIceCreamDecorationLayer__
#define __CCATS1044_BeachFoodParty__IceCreamIceCreamDecorationLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "AdapterScreen.h"
#include "BaseLayer.h"
#include "Utils.h"
#include "SceneManager.h"
#include "KDGridView.h"
#include "KSSimpleAdapter.h"
#include "UnitScrollView.h"
#include "Dialog.h"
#include "GameLayer.h"
#include "extensions/cocos-ext.h"
using namespace cocos2d::extension;

USING_NS_CC;
using namespace std;

enum IceCreamDecorationType{
    KSYRUP,
    KCANDY,
    KSPRINKLE,
    KCOOKIE,
    KTOPPER,
    KEXTRAS,

};



class GameLayer;




class IceCreamDecorationLayer:public BaseLayer{
    
public:
    IceCreamDecorationLayer(GameLayer* gameLayer);
    ~IceCreamDecorationLayer();
public:
    static IceCreamDecorationLayer* create(GameLayer* gameLayer);
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
    void hideUnitScroll();
    void showUnitScroll(int index);
    
    bool TouchBegan(Touch *pTouch, Event *pEvent);
    void TouchEnded(Touch *pTouch, Event *pEvent);
    void TouchMoved(Touch *pTouch, Event *pEvent);
    
    void purchaseFinish(Ref*);
private:
    
    void showBanner(Ref* r);
    void hideBanner(Ref* r);
    
    GameLayer* m_pGameLayer;
    Layer* m_pDecLayer;
    
    // Sprite* m_pFruitSprite;
    Vector<Sprite*> m_vTypes;
    KDGridView* m_pGirdView;
    KSSimpleAdapter* m_pAdapter;
    UnitScrollView* m_pUnitScrollView;
    MenuItemSprite* m_pCurrentSprite;
    Sprite* m_pUnitSprite;
    Sprite* m_pUnitScrollbgsprite;
    
    Sprite* m_pCup;
    Sprite* pBgSprite;
    Sprite* m_pCake;
    ui::Button* resetBtn;
    int m_pIndex;
    //判断是否接受点击事件
    bool m_bIsTouch;
    
    //背景是否存在
    bool m_bIsScrollBgShow;
    
//    __String* m_sTouchPath;
//    std::string m_sTouchPath;
    int m_iFrontIndex;
    int m_iBackrontIndex;
};
#endif /* defined(__CCATS1044_BeachFoodParty__IceCreamIceCreamDecorationLayer__) */
