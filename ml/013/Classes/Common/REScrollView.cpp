//
//  REScrollView.cpp
//  CSnowman
//
//  Created by QAMAC01 on 14-11-17.
//
//

#include "REScrollView.h"


REScrollView* REScrollView::create(Size size)
{
    REScrollView* pRet = new REScrollView();
    if (pRet && pRet->initWithViewSize(size,nullptr))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}


bool REScrollView::onTouchBegan(Touch *touch, Event *event)
{
    bool ret = ScrollView::onTouchBegan(touch, event);
    return ret;
}
void REScrollView::onTouchMoved(Touch *touch, Event *event)
{

    ScrollView::onTouchMoved(touch, event);
    if (m_pREScrollViewDelegate)
    {
        m_pREScrollViewDelegate -> scrollViewDidScroll(this);
    }

}



void REScrollView::onTouchEnded(Touch* touch, Event* event)
{
    ScrollView::onTouchEnded(touch, event);
    if (m_pREScrollViewDelegate)
    {
        m_pREScrollViewDelegate -> scrollViewTouchEnd(touch -> getLocation());
    }

}

void REScrollView::onTouchCancelled(Touch* touch, Event* event)
{
    if (!this->isVisible())
    {
        return;
    }
    
    auto touchIter = std::find(_touches.begin(), _touches.end(), touch);
    _touches.erase(touchIter);
    
    if (_touches.size() == 0)
    {
        _dragging = false;
        _touchMoved = false;
    }
    
    if (m_pREScrollViewDelegate)
    {
        m_pREScrollViewDelegate -> scrollViewTouchEnd(touch -> getLocation());
        
    }

}