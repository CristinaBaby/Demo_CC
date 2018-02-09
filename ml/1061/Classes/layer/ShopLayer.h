//
//  ShopLayer.h
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-24.
//
//
#ifndef __OreoMaker__ShopLayer__
#define __OreoMaker__ShopLayer__

#include <iostream>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "KDGridView.h"
#include "KShopAdapter.h"
#include "Dialog.h"
#include "DialogCallback.h"
#include "MySpriteButton.h"


using namespace std;

class ShopLayer:public BaseLayer,public KDGridViewScrollCallback{
    
private:
    ShopLayer();
    ~ShopLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(ShopLayer);
    
public:
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
public:
    void initUI();
    void initData();
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    virtual void onScrolling(float offsetX,float offsetY);
    virtual void onScrollEnd(float offsetX,float offsetY);
    virtual void onScrollDidZoom(cocos2d::extension::ScrollView* view);
    
    
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);
    void onBuyCallback(int,MenuItem*,int);
    void purchaseFinishCallback(bool isPurchase);
    
    
    void AfterRestoreCallBack(bool isRestore,bool isPurchase);
    void shopCloaeAds();
    bool checkOneUnLock();
    bool checkAllUnLock();
    
    void btnClick(Ref* pSender);
    void restoreClick(Ref* pSender);
    void closeClick(Ref* pSender);
    
    void showUI();
    void showFont();
private:
    Vector<Sprite*> m_vTypes;
    Vector<MySpriteButton*> m_vBtn;
//    KDGridView* m_pGirdView;
//    KShopAdapter* m_pAdapter;
    bool isFirstIn;
    
    //判断另一个包
//    ui::Widget *ui_button;
//    ui::Widget *ui_close;
    
    int btnIndex;
};

#endif /* defined(__OreoMaker__ShopLayer__) */
