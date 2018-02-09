//
//  STGridView.cpp
//  SweetDiscovery
//
//  Created by zhangguangzong1 on 8/21/13.
//
//

#include "KDGridView.h"
//#include "CSVParse.h"

KDGridView::KDGridView():
m_fHSpace(0),m_fVSpace(0),
m_iCol(0),m_iRow(0),
m_fMarginTop(0),m_fMarginLeft(0),
m_fMarginRight(0),m_fMarginBottom(0),
m_fAdjustStep(0),m_ScrollCallback(nullptr),
m_bAutoGetAdjust(true),m_bTouchedMenu(false),
m_bNeedAdjust(true),m_bScroll(true),
m_bScrollNeedAdjust(false),m_bLock(false),
isInTouchLoop(false),m_Adapter(nullptr),
m_fnClickCall(nullptr),m_fnMoveCall(nullptr),
m_fnTouchDownCall(nullptr),m_fnTouchEndCall(nullptr),m_fnTouchMoveCall(nullptr),
m_bIsNeedSwallowEvent(true),listener(nullptr),
m_iIndex(-1)
{}

KDGridView:: ~KDGridView()
{
    CC_SAFE_RELEASE_NULL(m_Adapter);
}

KDGridView* KDGridView::create(const Size &contentSize)
{
    KDGridView* grid = new KDGridView();
    
    if(grid && grid->initWithSize(contentSize))
    {
        grid->autorelease();
        return grid;
    }
    CC_SAFE_DELETE(grid);
    return nullptr;
}

void KDGridView::setScrollEnable(bool enable)
{
    this->m_bScroll = enable;
}

bool KDGridView::initWithSize(const cocos2d::Size &contentSize)
{
    bool bRet = false;
    
    do {
        CC_BREAK_IF(!Layer::init());
        
        this->setContentSize(contentSize);
        
        this->ignoreAnchorPointForPosition(false);
        
        this->setAnchorPoint(Vec2(0.5f, 0.5f));
        
        
        m_pMenu = Menu::create();
        
        m_pMenu->setPosition(Vec2::ZERO);
        
        _eventDispatcher->removeEventListenersForTarget(m_pMenu);
        
        
        
        m_Container = Layer::create();
        
        m_pScrollView = ScrollView::create(contentSize, m_Container);
        
        m_pScrollView->setPosition(Vec2::ZERO);
        
        m_pScrollView->setTouchEnabled(true);
        
        m_pScrollView->setDelegate(this);
        
        m_pScrollView->setDirection(ScrollView::Direction::VERTICAL);
        
        this->addChild(m_pScrollView, 1);
        
        _eventDispatcher->removeEventListenersForTarget(m_pScrollView);
        
        m_pScrollView->setBounceable(true);
        
        
        m_pMask = Layer::create();
        
        m_pMask->setContentSize(contentSize);
        
        m_pMask->setPosition(Vec2::ZERO);
        
        this->addChild(m_pMask, 0);
        
        listener = EventListenerTouchOneByOne::create();
        
        listener->setSwallowTouches(m_bIsNeedSwallowEvent);
        
        listener->onTouchBegan = CC_CALLBACK_2(KDGridView::TouchBegan, this);
        listener->onTouchMoved = CC_CALLBACK_2(KDGridView::TouchMoved, this);
        listener->onTouchEnded = CC_CALLBACK_2(KDGridView::TouchEnded, this);
        listener->onTouchCancelled = CC_CALLBACK_2(KDGridView::TouchCancelled, this);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, m_pMask);
        
        m_Container->addChild(m_pMenu);
        
        bRet = true;
        
    } while (0);
    
    return bRet;
}

void KDGridView::setSwallowEvent(bool isSwallow)
{
    
    m_bIsNeedSwallowEvent = isSwallow;
    listener->setSwallowTouches(m_bIsNeedSwallowEvent);
}

void KDGridView::setScrollCallback(KDGridViewScrollCallback *callback)
{
    if(nullptr == callback)
        return;
    
    this->m_ScrollCallback = callback;
}

KDGridViewScrollCallback* KDGridView::getScrollCallback()
{
    return m_ScrollCallback;
}

void KDGridView::setBackground(Node* bg)
{
    bg->ignoreAnchorPointForPosition(false);
    
    bg->setAnchorPoint(Vec2(0.5f,0.5f));
    
    bg->setPosition(Vec2(this->getContentSize().width/2,
                        this->getContentSize().height/2));
    
    this->addChild(bg,-1);
}

void KDGridView::setAdapter(KDAdapter* adapter)
{
    if(NULL == adapter)
        return;
    
    if (m_Adapter)
    {
        m_Adapter->release();
        m_Adapter = nullptr;
    }
    
    this->m_Adapter = adapter;
    
    this->m_Adapter->retain();
    
    this->layoutView();
}

KDAdapter* KDGridView::getAdapter()
{
    return m_Adapter;
}

void KDGridView::setAdjustStep(float f)
{
    if(f>=0)
    {
        this->m_fAdjustStep = f;
        this->m_bAutoGetAdjust = false;
    }else
    {
        this->m_bAutoGetAdjust = true;
    }
}

void KDGridView::layoutView()
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
        
        if(m_pScrollView->getDirection() == ScrollView::Direction::HORIZONTAL)
        {
            int col = totalCount % m_iRow == 0 ? totalCount / m_iRow : totalCount / m_iRow + 1;
            log("col:%d",col);
            
            //计算宽度
            float containerWidth = m_fMarginLeft + itemWidth * col + m_fHSpace * (col -1) + m_fMarginRight;
            
            if(m_bAutoGetAdjust)
                m_fAdjustStep = itemWidth + m_fHSpace;
            
            m_Container->setContentSize(Size(containerWidth, this->getContentSize().height));
            
            //待修改
            m_fLowLevelOffsetX = this->getContentSize().width - containerWidth;
            
            m_bScrollNeedAdjust = m_fLowLevelOffsetX >= 0 ? false : true ;
            
            m_pScrollView->setContentOffset(Vec2(0, 0));
            
            m_pScrollView->setContentSize(m_Container->getContentSize());
        }else
        {
            //计算行数
            int row = totalCount%m_iCol == 0 ? totalCount/m_iCol : totalCount/m_iCol + 1;
            /////////////////////////////////////////////////////////////
            //计算高度
//            float containerHeight = m_fMarginTop + itemHeight * row + m_fVSpace*(row -1) + m_fMarginBottom;
            float containerHeight = m_fMarginTop + m_fMarginBottom  + m_fVSpace*(row -1) + m_Adapter->getTotalHeight(m_Adapter->getCount());;

            /////////////////////////////////////////////////////////////
            if(m_bAutoGetAdjust)
                m_fAdjustStep = itemHeight + m_fVSpace;
            m_Container->setContentSize(Size(this->getContentSize().width, containerHeight));
            m_fLowLevelOffsetY = this->getContentSize().height - containerHeight;
            m_bScrollNeedAdjust = m_fLowLevelOffsetY>=0 ? false : true ;
            m_pScrollView->setContentOffset(Vec2(0,m_fLowLevelOffsetY));
            m_pScrollView->setContentSize(m_Container->getContentSize());
        }
        float startX = m_fMarginLeft;
        float startY = m_Container->getContentSize().height - m_fMarginTop;
        log("startY = %f",startY);
        
        for (int i = 0; i < totalCount; i++) {
            
            MenuItem* obj = m_Adapter->getView(i, nullptr, m_pMenu);

            m_pMenuItemsVector.pushBack(obj);
            
            if (obj)
            {

                obj->setCallback(CC_CALLBACK_1(KDGridView::menuCallback,this));
                
                obj->setUserObject(__String::createWithFormat("%d",i));
                
                obj->ignoreAnchorPointForPosition(false);
                obj->setAnchorPoint(Vec2(0.5, 0.5));
                
                int curRow = this->calculateRow(i);
                int curCol = this->calculateColumn(i);
                
                Rect rect = obj->boundingBox();
                
                float nodeX = 0;
                float nodeY = 0;
                if (m_iCol == 1) {
                    nodeX = startX + this->getContentSize().width / 2;
                    nodeY = startY - m_Adapter->getTotalHeight(curRow) - m_fVSpace*curRow - rect.size.height * obj->getAnchorPoint().y;
                }else{
                    nodeX = startX + curCol*rect.size.width + m_fHSpace*curCol + rect.size.width * obj->getAnchorPoint().x;
                    nodeY = startY - curRow*rect.size.height - m_fVSpace*curRow - rect.size.height * obj->getAnchorPoint().y;
                }
                
                obj->setPosition(Vec2(nodeX, nodeY));
                
                log("nodeX = %f, nodeY = %f",nodeX,nodeY);
                m_pMenu->addChild(obj);
            }
        }
    }
    if (m_ScrollCallback)
    {
        m_ScrollCallback->onScrollEnd(m_pScrollView->getContentOffset().x, m_pScrollView->getContentOffset().y);
    }
}

void KDGridView::loadLayoutConfig(const char* csv,int row)
{

}

void KDGridView::refresh()
{
    this->layoutView();
}

bool KDGridView::isNearToLeftTopSide()
{
    if(m_pScrollView->getDirection() == ScrollView::Direction::HORIZONTAL)
    {
        return m_pScrollView->getContentOffset().x>=0;
    }else
    {
        return m_pScrollView->getContentOffset().y>=0;
    }
}

bool KDGridView::isNearToRightBottomSide()
{
    if(m_pScrollView->getDirection() == ScrollView::Direction::HORIZONTAL)
    {
        return m_pScrollView->getContentOffset().x<=m_fLowLevelOffsetX;
    }else
    {
        return m_pScrollView->getContentOffset().y<=m_fLowLevelOffsetY;
    }
}

void KDGridView::moveNextStep()
{
    if (!m_bLock)
    {
        m_bLock = true;
        Point adjustPos;
        if(m_pScrollView->getDirection() == ScrollView::Direction::HORIZONTAL)
        {
            if(m_pScrollView->getContentOffset().x<=m_fLowLevelOffsetX)
            {
                m_bLock = false;
                return;
            }
            adjustPos = Vec2(m_pScrollView->getContentOffset().x - m_fAdjustStep, 0);
        }else
        {
            if(m_pScrollView->getContentOffset().y<=m_fLowLevelOffsetY)
            {
                m_bLock = false;
                return;
            }
            adjustPos = Vec2(0,m_pScrollView->getContentOffset().y - m_fAdjustStep);
        }
        m_pScrollView->setContentOffsetInDuration(adjustPos, 0.5f);
        
        FiniteTimeAction*  action = Sequence::create(DelayTime::create(0.5f), CallFunc::create(CC_CALLBACK_0(KDGridView::unlock,this)), NULL);
        runAction(action);
        
    }
}

void KDGridView::movePreStep()
{
    if(!m_bLock)
    {
        m_bLock = true;
        Point adjustPos;
        if(m_pScrollView->getDirection() == ScrollView::Direction::HORIZONTAL)
        {
            if(m_pScrollView->getContentOffset().x>=0)
            {
                m_bLock = false;
                return;
            }
            adjustPos = Vec2(m_pScrollView->getContentOffset().x + m_fAdjustStep, 0);
        }else
        {
            if(m_pScrollView->getContentOffset().y>=0)
            {
                m_bLock = false;
                return;
            }
            adjustPos = Vec2(0,m_pScrollView->getContentOffset().y + m_fAdjustStep);
        }
        m_pScrollView->setContentOffsetInDuration(adjustPos, 0.5f);
        FiniteTimeAction*  action = Sequence::create(DelayTime::create(0.5f), CallFunc::create(CC_CALLBACK_0(KDGridView::unlock,this)), NULL);
        runAction(action);
    }
}

int KDGridView::calculateRow(int index) {
    int arg = 0;
    switch (m_pScrollView->getDirection()) {
		case ScrollView::Direction::VERTICAL:
			arg = (int) index / m_iCol;
			break;
		case ScrollView::Direction::HORIZONTAL:
			arg = (int) index % m_iRow;
			break;
        default:
			arg = (int) index / m_iCol;
            break;
    }
    return arg;
}

int KDGridView::calculateColumn(int index) {
    int arg = 0;
    switch (m_pScrollView->getDirection()) {
		case ScrollView::Direction::VERTICAL:
			arg = index % m_iCol;
			break;
		case ScrollView::Direction::HORIZONTAL:
			arg = index / m_iRow;
			break;
        default:
            arg = index % m_iCol;
            break;
    }
    return arg;
}

void KDGridView::menuCallback(Ref* pSender)
{
    MenuItem* obj = dynamic_cast<MenuItem*>(pSender);
    if(obj && m_fnClickCall)
    {
        __String* indexStr = (__String*)obj->getUserObject();
        if(index >=0 && 0 < m_Adapter->getCount())
        {
            m_fnClickCall(this->getTag(),obj,indexStr->intValue());
        }
    }
}

void KDGridView::setDirection(ScrollView::Direction eDirection)
{
    if(m_pScrollView)
    {
        m_pScrollView->setDirection(eDirection);
    }
}

//scrollview滚动的时候会调用
void KDGridView::scrollViewDidScroll(ScrollView* view)
{
    if (m_ScrollCallback)
    {
        m_ScrollCallback->onScrolling(view->getContentOffset().x, view->getContentOffset().y);
    }
}

void KDGridView::onEnter()
{
    Layer::onEnter();
}

void KDGridView::onExit()
{
    Layer::onExit();
}

bool KDGridView::TouchBegan(Touch *pTouch, Event *pEvent)
{
    if (!this->isVisible())
    {
        return false;
    }
    
    if(m_bLock)
    {
        return false;
    }
    
	Vec2 touchPoint = pTouch->getLocation();
    
	Vec2 realPoint = m_pMask->convertToNodeSpace(touchPoint);
    
	Rect rect = Rect(0, 0, m_pMask->getContentSize().width, m_pMask->getContentSize().height);
    log("realPoint.y============%f", realPoint.y - m_pScrollView->getContentOffset().y);
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
            if(this->m_bScroll)
            {
                m_iIndex = (int)(realPoint.y - m_pScrollView->getContentOffset().y - m_fMarginBottom) / (m_Adapter->getItemHeight() + m_fVSpace);
//                log("index========%d", m_iIndex);
 
                if (m_fnTouchDownCall) {
                    log("touchPoint.x=========%f, touchPoint.y===========%f", touchPoint.x, touchPoint.y);
                    m_fnTouchDownCall(m_iIndex, pTouch, pEvent);
                }
                m_bTouchedMenu = m_pMenu->onTouchBegan(pTouch,pEvent);
                m_pScrollView->onTouchBegan(pTouch, pEvent);
            }
            else
            {
                m_bTouchedMenu = m_pMenu->onTouchBegan(pTouch,pEvent);
            }
        }
		return true;
	}
    return false;
}

void KDGridView::TouchMoved(Touch *pTouch, Event *pEvent)
{
    if (m_fnTouchMoveCall) {
        m_fnTouchMoveCall(m_iIndex, pTouch, pEvent);
    }
    
    Vec2 delta = pTouch->getDelta();
//
//    Vec2 realPoint = m_pMask->convertToNodeSpace(pTouch->getLocation());
//    Rect rect = Rect(0, 0, m_pMask->getContentSize().width, m_pMask->getContentSize().height);
//    if (!rect.containsPoint(realPoint)) {
//        return;
//    }
    
    if (fabs(delta.x) < 3 && fabs(delta.y) < 3) {
        return;
    }
    
    float degress = CC_RADIANS_TO_DEGREES(atan2(delta.y, - delta.x));
    if ((degress > 135.0f && degress <= 180.0f) || (degress > -180.0f && degress < -135.0f)) {
        if (m_fnMoveCall) {
            m_fnMoveCall(m_iIndex, pTouch, pEvent);
            return;
        }
    }
    
    if(!m_bLock)
    {
        if (this->m_bScroll)
        {
            m_pScrollView->onTouchMoved(pTouch, pEvent);
            if (m_bTouchedMenu)
            {
                m_pMenu->onTouchCancelled(pTouch,pEvent);
                m_bTouchedMenu = false;
            }
//        }else
//        {
//            m_pMenu->onTouchMoved(pTouch, pEvent);
        }
    }
}

void KDGridView::TouchEnded(Touch *pTouch, Event *pEvent)
{
    if(!m_bLock)
    {
        if (m_bNeedAdjust) {
            //m_bLock = true;
        }
        
        if (this->m_bScroll)
            m_pScrollView->onTouchEnded(pTouch, pEvent);
        
        Vec2 endPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
        
        float distance = 0;
        
        if (m_pScrollView->getDirection() == ScrollView::Direction::HORIZONTAL)
        {
            distance = endPoint.x - m_touchPoint.x;
        }
        else
        {
            distance = endPoint.y - m_touchPoint.y;
        }
        
        log("distance:%f",distance);
        
        if (this->m_bScroll)
        {
            if(fabs(distance) < 16 )
            {
                if(m_bTouchedMenu)
                {
                    m_pMenu->onTouchEnded(pTouch,pEvent);
                    
                    m_bTouchedMenu = false;
                    log("excute menu click!");
                    log("offsetX:%f",m_pScrollView->getContentOffset().x);
                }
            }else
            {
                if (m_bTouchedMenu)
                {
                    m_pMenu->onTouchCancelled(pTouch,pEvent);
                    
                    m_bTouchedMenu = false;
                    log("cancel menu click!");
                }
                
                float m_EndOffset = 0;
                
                switch (m_pScrollView->getDirection())
                {
                    case ScrollView::Direction::VERTICAL:
                        
                        m_EndOffset = m_pScrollView->getContentOffset().y;
                        
                        if (m_bScrollNeedAdjust && m_EndOffset >= m_fLowLevelOffsetY && m_EndOffset<=0)
                        {
                            adjustScrollView(m_EndOffset - m_fBeginOffset);
                        }
                        
                        break;
                    case ScrollView::Direction::HORIZONTAL:
                        
                        m_EndOffset= m_pScrollView->getContentOffset().x;
                        
                        if (m_bScrollNeedAdjust && m_EndOffset <= 0 && m_EndOffset >= m_fLowLevelOffsetX)
                        {
                            adjustScrollView(m_EndOffset - m_fBeginOffset);
                        }
                        
                        break;
                    default:
                        break;
                }
                log("m_EndOffset:%f",m_EndOffset);
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
    }
    
    if (m_fnTouchEndCall) {
        m_fnTouchEndCall(m_iIndex, pTouch, pEvent);
    }
}

void KDGridView::TouchCancelled(Touch *pTouch, Event *pEvent)
{
    if (m_bScroll)
    {
        m_pScrollView->onTouchCancelled(pTouch, pEvent);
        
        float m_EndOffset = 0;
        
        switch (m_pScrollView->getDirection())
        {
            case ScrollView::Direction::VERTICAL:
                
                m_EndOffset = m_pScrollView->getContentOffset().y;
                
                if (m_bScrollNeedAdjust && m_EndOffset >= m_fLowLevelOffsetY && m_EndOffset <= 0)
                {
                    adjustScrollView(m_EndOffset - m_fBeginOffset);
                }
                else
                {
                    float adjustY = 0;
                    
                    if (m_EndOffset<0)
                    {
                        adjustY = m_fLowLevelOffsetY;
                    }
                    m_pScrollView->setContentOffset(Vec2(m_pScrollView->getContentOffset().x, adjustY));
                }
                break;
            case ScrollView::Direction::HORIZONTAL:
                
                m_EndOffset= m_pScrollView->getContentOffset().x;
                
                if (m_bScrollNeedAdjust && m_EndOffset <= 0 && m_EndOffset>=m_fLowLevelOffsetX)
                {
                    adjustScrollView(m_EndOffset - m_fBeginOffset);
                }
                else
                {
                    float adjustX = 0;
                    
                    if (m_EndOffset<0)
                    {
                        adjustX = m_fLowLevelOffsetX;
                    }
                    m_pScrollView->setContentOffset(Vec2(adjustX, m_pScrollView->getContentOffset().y));
                }
                break;
            default:
                break;
        }

    }
    if (m_bTouchedMenu)
    {
        m_pMenu->onTouchCancelled(pTouch, pEvent);
        
        m_bTouchedMenu = false;
    }
}

//根据手势滑动的距离和方向滚动图层
void KDGridView::adjustScrollView(float offset)
{
    if(!m_bNeedAdjust)
        return;
    
    if (!m_fAdjustStep)
        return;
    
    m_bLock = true;
    
    float m_Offset = 0;
    
    Point adjustPos;
    
    float adjustAnimDelay = 0.3f;
    
    float x = offset - (int)(offset / m_fAdjustStep) * m_fAdjustStep;
    
    switch (m_pScrollView->getDirection())
    {
		case ScrollView::Direction::VERTICAL:
            
            m_Offset = m_pScrollView->getContentOffset().y;
			break;
            
		case ScrollView::Direction::HORIZONTAL:
            
            m_Offset= m_pScrollView->getContentOffset().x;
			break;
            
        default:
            break;
    }
    
    if((int)((fabs(x) / m_fAdjustStep) + 0.5))
    {
        //over half
        x = x > 0 ? m_fAdjustStep - x : -(m_fAdjustStep - fabs(x));
        m_Offset = m_Offset + x;
    }
    else
    {
        m_Offset = m_Offset - x;
    }
    adjustAnimDelay = fabs(x / m_fAdjustStep);
    
    switch (m_pScrollView->getDirection())
    {
		case ScrollView::Direction::VERTICAL:
            
            adjustPos = Vec2(0,m_Offset);
            break;
            
		case ScrollView::Direction::HORIZONTAL:
            
            adjustPos = Vec2(m_Offset,0);
			break;
            
        default:
            adjustPos = Vec2(0,m_Offset);
            break;
    }
    
    m_pScrollView->unscheduleAllSelectors();
    
    m_pScrollView->setContentOffsetInDuration(adjustPos, adjustAnimDelay);
    
    FiniteTimeAction*  action = Sequence::create(DelayTime::create(adjustAnimDelay + 0.2f), CallFunc::create(CC_CALLBACK_0(KDGridView::unlock, this)), nullptr);
    
    runAction(action);
}

void KDGridView::unlock()
{
    m_bLock = false;
    
    if (m_ScrollCallback)
    {
        m_ScrollCallback->onScrollEnd(m_pScrollView->getContentOffset().x, m_pScrollView->getContentOffset().y);
    }
}

void KDGridView::setLock(bool isLock)
{
    m_bLock = isLock;
}

Vector<MenuItem*> KDGridView::getMenuItemsVector()
{
    return m_pMenuItemsVector;
}

float KDGridView::getAdjustToCenterOfSceen(int index)
{
    if (m_pScrollView->getDirection() == ScrollView::Direction::HORIZONTAL)
    {
        return -((m_pMenuItemsVector.at(0)->getContentSize().width + m_fHSpace) * index) + Director::getInstance()->getVisibleSize().width / 2 - m_fMarginLeft - m_pMenuItemsVector.at(0)->getContentSize().width / 2.0f;
    }
    else if (m_pScrollView->getDirection() == ScrollView::Direction::VERTICAL)
    {
        return -((m_pMenuItemsVector.at(0)->getContentSize().height + m_fVSpace) * index) + Director::getInstance()->getVisibleSize().height / 2 - m_fMarginTop - m_pMenuItemsVector.at(0)->getContentSize().height / 2.0f;;
    }
    return 0;
}
