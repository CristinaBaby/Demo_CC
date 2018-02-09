/*----------------------------------------------------------------
 // Copyright (C) 公司名称 2014年 __MyCompanyName__. All rights reserved.
 //
 // 文件名：BaseScene
 // 文件功能描述：
 //
 //
 // 创建标识：dengcheng on 30/3/15.
 //
 // 修改标识：
 // 修改描述：
 //
 // 修改标识：
 // 修改描述：
 //---------------------------------------------------------------
 */

#ifndef __PizzaHouseStory__BaseScene__
#define __PizzaHouseStory__BaseScene__

#include "cocos2d.h"
#include <ui/CocosGUI.h>
#include "XCVisibleRect.h"
#include "UserDataManager.h"
#include "GameHeader.h"
#include <cocostudio/CocoStudio.h>

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class BaseScene : public Scene
{
public:
    
    BaseScene();
    ~BaseScene();
    
protected://Variables
    
    Layer *m_pUILayer;
    
    Layer *m_pGameLayer;
    
    Layer *m_pPopLayer;
    
    bool m_bBackKeyEvent;
    
    bool m_bTouchEvent;
    
    typedef enum {
        kNone = 0,
        kNextScene,
        kPreviousScene,
        kShop,
        kOtherTypeOne,
        kOtherTypeTwo,
        kOtherTypeThree,
        kOtherTypeFour
    }ButtonType;
    
protected://method
    
    virtual bool init(const std::string &aFilePath);
    
    /**
     *  打开返回键事件
     *
     *  @param std::function<void 回调
     */
    void openBackKeyEvent(std::function<void()> aFunc);
    
    /**
     *  打开触摸事件
     */
    void openTouchEvent();
    
    virtual bool onTouchBegan(Touch*, Event*){return true;}
    virtual void onTouchMoved(Touch*, Event*){}
    virtual void onTouchEnded(Touch*, Event*){}
    
    virtual void onExit() override;
    
    virtual void onButtonCallback(Ref *aButton, Widget::TouchEventType aType){}
    
    virtual void onVolumSwitch(Ref* aButton,CheckBox::EventType aType){}
    
private://Variables
    
    std::function<void()> m_fBackKeyCallback;
    
private://method
    
    /**
     *  设置背景图
     *
     *  @param aFile 文件路径
     */
    void setBackgroundImage(std::string aFile);
    
    
    void onKeyCallback(EventKeyboard::KeyCode code, Event* pEvent);
    
};

#endif