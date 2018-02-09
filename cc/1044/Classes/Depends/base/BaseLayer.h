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
#include "AdLoadingLayerBase.h"
#include "AdsLoadingLayer.h"

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
    virtual void onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event){};
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
    
    virtual void onNegativeClick(void* type){};
    virtual void onPositiveClick(void* type){};
public:
      //展示全屏广告的回调
    virtual void showAdsLaod(Ref*){};
    //关闭全屏广告的回调
    virtual void closeAdsLaod(Ref*){};
    
//    //购买成功的回调
//    virtual void purchaseFinish(Ref*){};
    
    virtual void showBanner(Ref* r){};
    virtual void hideBanner(Ref* r){};
protected:
    Size visibleSize;
    Size winSize;
    Vec2 visibleOrigin;
    
    Layer* m_pBackGroundLayer;
    Layer* m_pContentLayer;
    Layer* m_pUILayer;
    bool isReturn;
    
//    LayerColor* m_pLayerColor;
    
};
#endif /* defined(__OreoMaker__BaseLayer__) */
