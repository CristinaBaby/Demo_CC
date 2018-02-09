//
// XCPageView.cpp
// ANIM3203
//
// Created by dengcheng on 15/9/16.
//
//

#include "XCPageView.h"

XCPageView::XCPageView()
:m_bPauseScroll(false)
{
    
}

void XCPageView::onEnter()
{
    PageView::onEnter();
    
}

XCPageView* XCPageView::create()
{
    auto pRet = new XCPageView();
    if(pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    pRet = nullptr;
    return nullptr;
}

bool XCPageView::init()
{
    if ( !PageView::init() )
    {
        return false;
    }
    
    
    
    return true;
}

bool XCPageView::onTouchBegan(Touch *touch, Event *unusedEvent)
{
    if (m_bPauseScroll) {
        return false;
    }
    
    return PageView::onTouchBegan(touch, unusedEvent);
}
void XCPageView::onTouchMoved(Touch *touch, Event *unusedEvent)
{
    if (m_bPauseScroll) {
        return;
    }
    
    PageView::onTouchMoved(touch, unusedEvent);
}

void XCPageView::onExit()
{

    PageView::onExit();
}
XCPageView::~XCPageView()
{
    
}