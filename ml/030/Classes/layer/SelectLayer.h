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
#include "ToolSprite.h"

enum{
    kPIZZA,
    kPASTA,
    krISOTTO,
};

class SelectLayer : public BaseLayer,public ToolSpriteDelegate {
    
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
    
    void carAction();
    void dialogAction();
    void itemAction();
    void itemHide();
public:
    
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    
    // 监听Android返回键事件
    virtual void onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event);
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);
    
   void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
   void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
    
    void checkTouch(float f);
public:
    Sprite* m_pCar;
    Sprite* m_pDialog;
    Vector<ToolSprite*> m_vTool;
    
    //设施几秒没有操作  增加提示提示
    bool m_bCheckTouch;
    
    ui::Button* btn_yes;
    ui::Button* btn_no;
    
    Vec2 item_pos;
    
    bool m_bIsFirst;
};











#endif /* defined(__MLABS1043_BrownieMaker__SelectLayer__) */
