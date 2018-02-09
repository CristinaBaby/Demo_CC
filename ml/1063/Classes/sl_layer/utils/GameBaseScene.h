//
//  GameBaseScene.h
//  MLABS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/29.
//
//

#ifndef __MLABS1044_BeachFoodParty__GameBaseScene__
#define __MLABS1044_BeachFoodParty__GameBaseScene__

#include <stdio.h>
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Configure.h"
#include "DialogCallback.h"
#include "Dialog.h"
#include "Global.h"
#include "SceneManager.h"
#include "AdapterScreen.h"
#include "ActionUtils.h"
#include "ToolSprite.h"
//#include "Shake.h"
#include "AudioHelp.h"
#include "AdsManager.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace std;


//用于make项目中间的步骤Scene
class GameBaseScene : public Layer,public DialogCallback
{
public:
    typedef enum
    {
        kZorderBackground           =           0,
        kZorderContent              =           50,
        kZorderUI                   =           100
    }Zorder;
public:
    GameBaseScene();
    virtual ~GameBaseScene();
    
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
private:
    // 监听Android返回键事件
    EventListenerKeyboard* m_pKeyboardListener;
    void onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyBackDown();
    void onNegativeClick(void* type);
    void onPositiveClick(void* type);
    
    //按钮
    void touchEvent(Ref *obj , cocos2d::ui::Widget::TouchEventType type);
public:
    
    //展示全屏广告的回调
    virtual void showAdsLaod(EventCustom*);
    //关闭全屏广告的回调
    virtual void closeAdsLaod(EventCustom*);
    
    //下一步
    virtual void nextStep(){};
    
    void addToBGLayer(Node* node,int zorder = 0);
    void addToContentLayer(Node* node,int zorder = 0);
    void addToUILayer(Node* node,int zorder = 0);
public:
    void initUI();
    void hideNextButton();
    void showNextButton(bool isAction);
public:
    Layer* m_pBackGroundLayer;
    Layer* m_pContentLayer;
    Layer* m_pUILayer;

    //显示返回框
    bool m_bIsShowReturn;
    //按钮是否有动画
    bool m_bNextAction;
    ui::Button* m_pNextButton;
    
    EventListenerCustom* showListener;
    EventListenerCustom* closeListener;
};



#endif /* defined(__MLABS1044_BeachFoodParty__GameBaseScene__) */
