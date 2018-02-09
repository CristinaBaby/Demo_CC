//
//  MyPageView.cpp
//  LunchFoodMaker
//
//  Created by huxixiang on 15/9/28.
//
//

#include "MyPageView.h"


MyPageView::MyPageView():m_pDelegate(nullptr),m_bIsFlag(false)
{
    
}

MyPageView::~MyPageView()
{
    
}

bool MyPageView::init()
{
    bool isInit = false;
    do
    {
        CC_BREAK_IF(!PageView::init());
        isInit = true;
    }while(0);
    
    return isInit;
}

bool MyPageView::onTouchBegan(Touch *touch, Event *unusedEvent)
{
    if (m_pDelegate) {
        m_pDelegate->isCancelTouch(false);
        
        Rect limit = Rect(0, 0, this->getContentSize().width, this->getContentSize().height);
        if(limit.containsPoint(this->convertToNodeSpace(touch->getLocation())))
        {
            if (!m_bIsFlag) {
                m_bIsFlag = true;
                m_pDelegate->touchBegin();
            }
        }
    }
    return PageView::onTouchBegan(touch, unusedEvent);
}

void MyPageView::onTouchMoved(Touch *touch, Event *unusedEvent)
{
    Vec2 delta = touch->getDelta();
    
    log("delta.x========%f", delta.x);
    if (fabs(delta.x) != 0 || fabs(delta.y) != 0) {
        
        if (m_pDelegate) {
            m_pDelegate->isCancelTouch(true);
        }
    }else{
        if (m_pDelegate) {
            m_pDelegate->isCancelTouch(false);
        }
    }
    
    PageView::onTouchMoved(touch, unusedEvent);
}

void MyPageView::onTouchEnded(Touch *touch, Event *unusedEvent)
{
    if (m_pDelegate) {
        if (m_bIsFlag) {
            m_bIsFlag = false;
            m_pDelegate->touchEnd();
        }
    }
    PageView::onTouchEnded(touch, unusedEvent);
}

void MyPageView::onTouchCancelled(Touch *touch, Event *unusedEvent)
{
    this->onTouchEnded(touch, unusedEvent);
}

void MyPageView::setMyPageViewDelegate(MyPageViewDelegate* delegate)
{
    m_pDelegate = delegate;
}

void MyPageView::onEnter()
{
    PageView::onEnter();
}

void MyPageView::onEnterTransitionDidFinish()
{
    PageView::onEnterTransitionDidFinish();
}