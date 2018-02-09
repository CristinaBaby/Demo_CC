//
//  KDIYScrollView.cpp
//  CookiesMaker
//
//  Created by wangbosheng1 on 14-12-1.
//
//

#include "KDIYScrollView.h"

KDIYScrollView::KDIYScrollView():
m_bTouchedMenu(false),m_bScroll(true),
m_bScrollNeedAdjust(false),m_bLock(false),
m_Adapter(NULL),m_fnClickCall(nullptr)
{}

KDIYScrollView:: ~KDIYScrollView()
{
//    CC_SAFE_RELEASE_NULL(m_Adapter);
}

KDIYScrollView* KDIYScrollView::create(const Size &contentSize)
{
    KDIYScrollView* grid = new KDIYScrollView();
    if(grid && grid->initWithSize(contentSize))
    {
        grid->autorelease();
        return grid;
    }
    CC_SAFE_DELETE(grid);
    return nullptr;
}

void KDIYScrollView::setScrollEnable(bool enable)
{
    this->m_bScroll = enable;
}

bool KDIYScrollView::initWithSize(const cocos2d::Size &contentSize)
{
    bool bRet = false;
    
    do {
        CC_BREAK_IF(!Layer::init());
        
        this->setContentSize(contentSize);
        this->ignoreAnchorPointForPosition(false);
        this->setAnchorPoint(Vec2(0.5f,0.5f));
        
        m_pMenu = Menu::create();
        m_pMenu->setPosition(Vec2::ZERO);
        _eventDispatcher->removeEventListenersForTarget(m_pMenu);
        
        m_Container = Layer::create();
        
        m_pScrollView = ScrollView::create(contentSize,m_Container);
        m_pScrollView->setPosition(Vec2::ZERO);
        m_pScrollView->setTouchEnabled(true);
        m_pScrollView->setDelegate(this);
        m_pScrollView->setDirection(ScrollView::Direction::VERTICAL);
        this->addChild(m_pScrollView,1);
        _eventDispatcher->removeEventListenersForTarget(m_pScrollView);
        m_pScrollView->setBounceable(true);
        
        m_pMask = Layer::create();
        m_pMask->setContentSize(contentSize);
        m_pMask->setPosition(Vec2::ZERO);
        this->addChild(m_pMask,0);
        
        EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        
        listener->onTouchBegan = CC_CALLBACK_2(KDIYScrollView::TouchBegan, this);
        listener->onTouchMoved = CC_CALLBACK_2(KDIYScrollView::TouchMoved, this);
        listener->onTouchEnded = CC_CALLBACK_2(KDIYScrollView::TouchEnded, this);
        listener->onTouchCancelled = CC_CALLBACK_2(KDIYScrollView::TouchCancelled, this);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, m_pMask);
        
        m_Container->addChild(m_pMenu);
        bRet = true;
    } while (0);
    
    return bRet;
}

void KDIYScrollView::setAdapter(KDAdapter* adapter)
{
    if(NULL == adapter)
        return;
    if (m_Adapter)
    {
        m_Adapter->release();
        m_Adapter = NULL;
    }
    this->m_Adapter = adapter;
    this->m_Adapter->retain();
    this->layoutView();
}

KDAdapter* KDIYScrollView::getAdapter()
{
    return m_Adapter;
}

void KDIYScrollView::layoutView()
{
    m_pMenu->removeAllChildrenWithCleanup(true);
    m_pMenuItemsVector.clear();
    if(!m_Container)
        return;
    int totalCount = m_Adapter->getCount();
    if (totalCount <= 0)
        return;
    if (m_pScrollView)
    {
        float itemWidth = m_Adapter->getItemWidth();
        float itemHeight = m_Adapter->getItemHeight();
        float containerHeight = 10 + m_Adapter->getView(0,NULL,nullptr)->getContentSize().height+
        m_Adapter->getView(1,NULL,nullptr)->getContentSize().height*4;
        m_Container->setContentSize(Size(this->getContentSize().width, containerHeight));
        m_pScrollView->setContentSize(m_Container->getContentSize());
        float startX = 10;
        float startY = m_Container->getContentSize().height - 10;
        for (int i = 0; i < totalCount; i++) {
            MenuItem* obj = m_Adapter->getView(i,NULL,m_pMenu);
            m_pMenuItemsVector.pushBack(obj);
            if (obj)
            {
                //                obj->setTarget(this, menu_selector(KDGridView::menuCallback));
                obj->setCallback(CC_CALLBACK_1(KDIYScrollView::menuCallback,this));
                obj->setUserObject(__String::createWithFormat("%d",i));
                obj->setAnchorPoint(Vec2(0.5, 0.5));

                Rect rect = obj->boundingBox();
                float nodeX ;
                float nodeY ;
                if(i%2 == 0)
                {
                    nodeX = 10+obj->getContentSize().width/2;
                }
                else
                {
                    nodeX = m_Container->getContentSize().width-10-obj->getContentSize().width/2;
                }
                
                if(0 == i)
                nodeY = startY-obj->getContentSize().height/2;
                else
                {
                    nodeY =startY-m_Adapter->getView(0,NULL,m_pMenu)->getContentSize().height/2-i*obj->getContentSize().height;
                }

                obj->setPosition(Vec2(nodeX, nodeY));
                m_pMenu->addChild(obj);
            }
        }

    }
    }


void KDIYScrollView::setDirection(ScrollView::Direction eDirection)
{
    if(m_pScrollView)
    {
        m_pScrollView->setDirection(eDirection);
    }
}

void KDIYScrollView::refresh()
{
    this->layoutView();
}

void KDIYScrollView::onEnter()
{
    Layer::onEnter();
}

void KDIYScrollView::onExit()
{
    Layer::onExit();
}

bool KDIYScrollView::TouchBegan(Touch *pTouch, Event *pEvent)
{
    if (!this->isVisible())
    {
        return false;
    }
	Vec2 touchPoint = pTouch->getLocation();
	Vec2 realPoint = m_pMask->convertToNodeSpace(touchPoint);
	Rect rect = Rect(0,0,m_pMask->getContentSize().width,m_pMask->getContentSize().height);
    
	if(rect.containsPoint(realPoint))
	{
        m_touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
        switch (m_pScrollView->getDirection())
        {
            case ScrollView::Direction::VERTICAL:
                m_fBeginOffset = m_pScrollView->getContentOffset().y;
                break;
            case ScrollView::Direction::HORIZONTAL:
                m_fBeginOffset= m_pScrollView->getContentOffset().x;
                break;
            default:
                m_fBeginOffset = 0;
                break;
        }
        if(!m_bLock)
        {
            m_bTouchedMenu = m_pMenu->onTouchBegan(pTouch,pEvent);
            if(this->m_bScroll)
                m_pScrollView->onTouchBegan(pTouch, pEvent);
            
        }
		return true;
	}
    return false;
}

void KDIYScrollView::TouchMoved(Touch *pTouch, Event *pEvent)
{
//    if(!m_bLock)
//    {
        if (this->m_bScroll)
        {
            m_pScrollView->onTouchMoved(pTouch, pEvent);
            if (m_bTouchedMenu)
            {
                m_pMenu->onTouchCancelled(pTouch,pEvent);
                m_bTouchedMenu = false;
            }
        }else
        {
            m_pMenu->onTouchMoved(pTouch, pEvent);
        }
//    }
}

void KDIYScrollView::TouchEnded(Touch *pTouch, Event *pEvent)
{
//    if(!m_bLock)
//    {
        if (this->m_bScroll)
            m_pScrollView->onTouchEnded(pTouch, pEvent);
        Vec2 endPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
        float distance = 0;
        if (m_pScrollView->getDirection() == ScrollView::Direction::HORIZONTAL)
        {
            distance = endPoint.x - m_touchPoint.x;
        }else
        {
            distance = endPoint.y - m_touchPoint.y;
        }
        
        log("distance:%f",distance);
        
        if (this->m_bScroll)
        {
            if(fabs(distance) < 10 )
            {
                if(m_bTouchedMenu)
                {
                    m_pMenu->onTouchEnded(pTouch,pEvent);
                    m_bTouchedMenu = false;
                    log("excute menu click!");
                }
            }else
            {
                if (m_bTouchedMenu)
                {
                    m_pMenu->onTouchCancelled(pTouch,pEvent);
                    m_bTouchedMenu = false;
                    log("cancel menu click!");
                }
            }
        }else
        {
            if (m_bTouchedMenu)
            {
                m_pMenu->onTouchEnded(pTouch,pEvent);
                m_bTouchedMenu = false;
                log("excute menu click!");
            }
        }
//    }
}

void KDIYScrollView::TouchCancelled(Touch *pTouch, Event *pEvent)
{
    if (m_bScroll)
    {
        m_pScrollView->onTouchCancelled(pTouch, pEvent);
    }
    if (m_bTouchedMenu)
    {
        m_pMenu->onTouchCancelled(pTouch, pEvent);
        m_bTouchedMenu = false;
    }
}

void KDIYScrollView::menuCallback(Ref* pSender)
{

}


