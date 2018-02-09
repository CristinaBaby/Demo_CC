//
//  SelectLayer.h
//  MLABS1043_BrownieMaker
//
//  Created by wusonglin1 on 15-1-5.
//
//

#ifndef __MLABS1043_BrownieMaker__SelectLayer__
#define __MLABS1043_BrownieMaker__SelectLayer__

#include <iostream>
#include "cocos2d.h"
#include "../Depends/base/BaseLayer.h"
#include "../Depends/widget/KDGridView.h"
#include "../Depends/widget/KSSimpleAdapter.h"
#include "extensions/cocos-ext.h"


enum{
    kPIZZA,
    kPASTA,
    krISOTTO,
};

class SelectLayer : public BaseLayer {
    
public:
    SelectLayer();
    ~SelectLayer();
    
public:
    CREATE_FUNC(SelectLayer);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    void initUI();
    void initData();
    void onBtnCallBack(Ref* sender);
    void typeItemClick(int tag,MenuItem* menuItem,int index);
    void titleAction();
    void fontAction();
    void chooseAction();
    void onEnterTransitionDidFinish();
public:
    
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    
    // 监听Android返回键事件
    virtual void onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event);
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);
    
    bool checkFix(int i);
    bool checkSeveral(int i);
    
    void setChooseData(int index);
    
    void showTap();
    void hideTap();
public:
//    Sprite* m_pTitle;
   
    
    Vector<Sprite*> m_vTypes;
    KDGridView* m_pGirdView;
    KSSimpleAdapter* m_pAdapter;
    MenuItemSprite* m_pCurrentSprite;
    
    ui::Widget * nextBtn;
    ui::TextAtlas* ui_number;
    int m_pIndex;

    int m_iTouchIndex;
    ParticleSystemQuad* chooseParticle;
    int m_iFontNum;
    bool m_bIsFromShop;
    
    Sprite* m_pTitle;
    float nextBtnY;
};











#endif /* defined(__MLABS1043_BrownieMaker__SelectLayer__) */
