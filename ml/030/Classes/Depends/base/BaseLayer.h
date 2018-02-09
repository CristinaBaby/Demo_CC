//
//  BaseLayer.h
//  OreoMaker
//
//  Created by wangbosheng1 on 14-10-22.
//
//

#ifndef __OreoMaker__BaseLayer__
#define __OreoMaker__BaseLayer__

#include <iostream>
#include "cocos2d.h"
#include "AdapterScreen.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "../utils/DialogCallback.h"
#include "../utils/Dialog.h"
#include "../managers/SceneManager.h"
#include "AdsManager.h"
#include "Configure.h"
#include "AdsManager.h"


using namespace cocostudio;
using namespace ui;
USING_NS_CC;

class BaseLayer : public Layer,public DialogCallback{
    
public:
    BaseLayer();
    virtual ~BaseLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(BaseLayer);
    
public:
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    // 监听Android返回键事件
    //在子类实现
    virtual void onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event){};
    virtual void onNegativeClick(void* type){};
    virtual void onPositiveClick(void* type){};
public:
    // initNecessary
    bool initNecessary();
    // initUI
    void initUI();
    // regist event
    void registEvent();
    // add child to background layer
    void addToBackGroundLayer(Node *child, int localZOrder);
    // add child to content layer
    void addToContentLayer(Node *child, int localZOrder);
    // add child to ui layer
    void addToUILayer(Node *child, int localZOrder);
public:
    virtual void onHomeCallback(Node* sender){};
    virtual void onShopCallback(Node* sender){};
    virtual void onMoreCallback(Node* sender){};
    virtual void onNextCallback(Node* sender){};
    virtual void onBackCallback(Node* sender){};
    virtual void onResetCallback(Node* sender){};
    
public:
      //展示全屏广告的回调
    virtual void showAdsLaod(EventCustom*){};
    //关闭全屏广告的回调
    virtual void closeAdsLaod(EventCustom*){};

    bool m_bIsShowReturn;
    bool m_bIsQuitGame;
protected:
    Size visibleSize;
    Size winSize;
    Vec2 visibleOrigin;
    
    Layer* m_pBackGroundLayer;
    Layer* m_pContentLayer;
    Layer* m_pUILayer;
    
};
#endif /* defined(__OreoMaker__BaseLayer__) */
