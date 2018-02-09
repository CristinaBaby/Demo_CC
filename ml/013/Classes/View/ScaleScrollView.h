//
//  ScaleScrollView.h
//  CSnowman
//
//  Created by QAMAC01 on 14-11-14.
//
//

#ifndef __CSnowman__ScaleScrollView__
#define __CSnowman__ScaleScrollView__

#include <iostream>
#include "cocos2d.h"
#include "./extensions/cocos-ext.h"
#include "ui/cocosGUI.h"
#include "REScrollView.h"
#include "Dialog.h"
#include "DialogCallback.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;


class ScaleScrollView : public Node,public REScrollViewDelegate,public DialogCallback
{
public:
    static Scene* scene();
    CREATE_FUNC(ScaleScrollView);
    bool init();

private:
////delegate
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view);
    virtual void scrollViewDidZoom(REScrollView* view);
	virtual void scrollViewTouchEnd(Vec2 worldPoint);

    
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    void initScroll();
    void showDeleteBtn();
    float checkOffset(Sprite* sp);
    
private:
    __Array* favArray_;
    REScrollView* scroll_;
    Vector<Sprite*> sp_Vec;
    int curIdx_;
    Layer* scroll_layer;
    ui::Button* deleteBtn_;
};

#endif /* defined(__CSnowman__ScaleScrollView__) */
