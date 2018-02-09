//
//  FavLayer.h
//  CSnowman
//
//  Created by QAMAC01 on 14-11-14.
//
//

#ifndef __CSnowman__FavLayer__
#define __CSnowman__FavLayer__

#include <iostream>
#include "cocos2d.h"
#include "./extensions/cocos-ext.h"
#include "ui/cocosGUI.h"
#include "Dialog.h"
#include "DialogCallback.h"
#include "ScaleScrollView.h"
#include "BaseLayer.h"
#include "UIUnitScrollView.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class FavLayer : public Layer,public DialogCallback
{
public:
    static Scene* scene();
    CREATE_FUNC(FavLayer);
    bool init();
private:
    void initWithUI();
    
private:
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    
    
    virtual void onEnter();
    virtual void onExit();

    
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    
    //    virtual void onExit();
    //    virtual void onEnter();
    
    
    Vector<Node*> initFavBtn();
    Vector<Node*> vec_btn;
    void initScrollView();
    void showDeleteBtn(bool show);
private:
    UIUnitScrollView* _scrollView;
    bool _showBtn = false;
};

#endif /* defined(__CSnowman__FavLayer__) */
