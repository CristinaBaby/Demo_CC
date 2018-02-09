//
//  HomeLayer.h
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-7-31.
//
//

#ifndef __MakeSnowMan__HomeLayer__
#define __MakeSnowMan__HomeLayer__

#include <iostream>
#include "cocos2d.h"
#include "./extensions/cocos-ext.h"
#include "ui/cocosGUI.h"
#include "Dialog.h"
#include "DialogCallback.h"
#include "BaseLayer.h"
#include "ShopSelView.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;


class HomeLayer :public Layer,public DialogCallback

{
public:
    static Scene* scene();
    CREATE_FUNC(HomeLayer);
    bool init();
    
private:
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    void initWithUI();

    void initButton();
    void initBackground();
    void initAnimation();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

private:

    ShopSelView* _shopSelView;
    
};


#endif /* defined(__MakeSnowMan__HomeLayer__) */
