//
// XCPageView.h
// ANIM3203
//
// Created by dengcheng on 15/9/16.
//
//

#ifndef __ANIM3203__XCPageView__
#define __ANIM3203__XCPageView__

#include "cocos2d.h"
#include <ui/CocosGUI.h>
#include <cocostudio/CocoStudio.h>

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class XCPageView : public PageView
{
public:
    XCPageView();
    ~XCPageView();

    static XCPageView* create();
    virtual bool init();

    void pauseScroll() {
        m_bPauseScroll = true;
    }
    
    void resumeScroll() {
        m_bPauseScroll = false;
    }
    
protected://method

	void onEnter() override;

	void onExit() override;

private://Variables
    bool m_bPauseScroll;
private://method
    virtual bool onTouchBegan(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchMoved(Touch *touch, Event *unusedEvent) override;
};

#endif /* defined(__ANIM3203__XCPageView__) */