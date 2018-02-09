//
//  TweenScrollView.h
//  Make
//
//  Created by QAMAC01 on 15-3-4.
//
//

#ifndef __Make__TweenScrollView__
#define __Make__TweenScrollView__

#include <stdio.h>
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

class TweenScrollViewDelegate
{
public:
    virtual void scrollBegan() = 0;
    virtual void scrollEndWithIdx(int idx) = 0;
    
};

class TweenScrollView : public Node,public REScrollViewDelegate
{
public:
    static TweenScrollView* createScrollViewWithVec(Vector<__String*> vec);
    bool initScrollViewWithVec(Vector<__String*> vec);
    void setDelegate(TweenScrollViewDelegate* delegate){delegate_ = delegate;};
    
private:
    ////delegate
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view);
    virtual void scrollViewDidZoom(REScrollView* view);
    virtual void scrollViewTouchEnd(Vec2 worldPoint);
    
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    void initScroll();
    void showDeleteBtn();
    float checkOffset(Sprite* sp);
    
private:
    REScrollView* scroll_;
    Vector<Sprite*> btn_Vec;
    Vector<__String*> sp_Vec;
    int curIdx_;
    Layer* scroll_layer;
    TweenScrollViewDelegate* delegate_;
};

#endif /* defined(__Make__TweenScrollView__) */
