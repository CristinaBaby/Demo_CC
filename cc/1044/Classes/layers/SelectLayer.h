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
#include "STAds.h"

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
    void chooseAction();
    void onEnterTransitionDidFinish();
    
    void iceCreamAction();
    void icePopAction();
    void checkGame();
    
    void changeColor(int i);
public:
    
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    
    // 监听Android返回键事件
    virtual void onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event);
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);
public:
    Sprite* m_pTitle;
    
    Vector<Sprite*> m_vTypes;
    KDGridView* m_pGirdView;
    KSSimpleAdapter* m_pAdapter;
    MenuItemSprite* m_pCurrentSprite;
    
    ui::Button* nextBtn;
    int m_pIndex;
    
    //展示全屏广告的回调
    virtual void showAdsLaod(Ref*);
    //关闭全屏广告的回调
    virtual void closeAdsLaod(Ref*);
    
    int m_iTouchIndex;
    ParticleSystemQuad* chooseParticle;
};











#endif /* defined(__MLABS1043_BrownieMaker__SelectLayer__) */
