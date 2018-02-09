//
//  BaseLayer.h
//  FastFood
//
//  Created by huxixiang on 15/8/3.
//
//

#ifndef __FastFood__BaseLayer__
#define __FastFood__BaseLayer__

#include <iostream>
#include "cocos2d.h"
#include "AdapterScreen.h"
#include "Dialog.h"
#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "SoundPlayer.h"
#include "GameData.h"
#include "Global.h"
#include "LockScreenLayer.h"

USING_NS_CC;
using namespace ui;

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

public:
    // initNecessary
    bool initNecessary();
    
    // initUI
    void initUI();
    
    // regist event
    void registEvent();
    
    void addLockScreenLayer(int zOrder = INT16_MAX - 10);
    void removeLockScreenLayer();
    bool isLockScreen();
    
    void onButtontouchCallback(Ref *pSender, Widget::TouchEventType type);
    
    Button* getNextButton();
    
    bool getNextBtnVisible();
    void setNextBtnIsVisible(bool isVisible, bool isNeedAction = true);
    
    bool getNextBtnEnabled();
    void setNextBtnIsEnabled(bool isEnabled);
    
    virtual void onNextCallback(){};
    
    void setKeyEnabled(bool enable);
    
    LockScreenLayer* getLockScreenLayer();
    
public:
    // 监听Android返回键事件
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event);
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);
    
public:
    Size visibleSize;
    Size winSize;
    Vec2 visibleOrigin;
    bool isReturn;
    
    LockScreenLayer* m_pLockLayer;
private:
    Button* m_pNextBtn;
    
    bool m_bKeyEnabled;
    
    bool m_bNextBtnEnabled;
    
    bool m_bIsLockScreen;
};
#endif /* defined(__FastFood__BaseLayer__) */
