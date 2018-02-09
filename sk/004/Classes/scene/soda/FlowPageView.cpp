
#include "FlowPageView.h"
FlowPageView::FlowPageView()
{
    m_fScrollSpeed = 0;
    m_fMaxScale = 1;
    m_fMinScale = 1;
    m_PageSize = Size::ZERO;
    startPointOffset = Vec2::ZERO;
    _autoScrollSpeed = 0;
    scrollEnd = nullptr;
    m_bIsScrollByPage = true;
}

FlowPageView::~FlowPageView()
{
    
}

FlowPageView* FlowPageView::create()
{
    FlowPageView* widget = new FlowPageView();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

bool FlowPageView::init()
{
    if ( !PageView::init() )
    {
        return false;
    }
    
    return true;
}

void FlowPageView::doLayout()
{
    if (!_doLayoutDirty)
    {
        return;
    }
    if (m_PageSize.equals(Size::ZERO)) {
        m_PageSize = getContentSize();
    }
    updateAllPagesPosition();
    updateAllPagesSize();
    updateBoundaryPages();
    
    refresh();
    _doLayoutDirty = false;
}

void FlowPageView::updateAllPagesSize()
{
    Size selfSize = m_PageSize;
    for (auto& page : _pages)
    {
        page->setContentSize(selfSize);
    }
    
}
void FlowPageView::updateAllPagesPosition()
{
    ssize_t pageCount = this->getPageCount();
    
    if (pageCount <= 0)
    {
        _curPageIdx = 0;
        return;
    }
    
    if (_curPageIdx >= pageCount)
    {
        _curPageIdx = pageCount-1;
    }
    
    float pageWidth = m_PageSize.width;
    for (int i=0; i<pageCount; i++)
    {
        Layout* page = _pages.at(i);
        page->setPosition(Vec2((i-_curPageIdx) * pageWidth+(getContentSize().width-m_PageSize.width)/2+startPointOffset.x, startPointOffset.y));
        
    }
    
}

bool FlowPageView::onTouchBegan(Touch *touch, Event *unusedEvent)
{
    m_bIsScrolling = false;
    return PageView::onTouchBegan(touch, unusedEvent);
}


bool FlowPageView::scrollPages(float touchOffset)
{
//    PageView::scrollPages(touchOffset);
    if (this->getPageCount() <= 0)
    {
        return false;
    }
    
    if (!_leftBoundaryChild || !_rightBoundaryChild)
    {
        return false;
    }
    if (fabs(touchOffset)>3.0f) {
        m_bIsScrolling = true;
    }
    float realOffset = touchOffset;
    
    switch (_touchMoveDirection)
    {
        case TouchDirection::LEFT: // left
            
            if (_rightBoundaryChild->getRightBoundary()+(getContentSize().width-m_PageSize.width)/2 + touchOffset <= _rightBoundary)
            {
                realOffset = _rightBoundary - _rightBoundaryChild->getRightBoundary()-(getContentSize().width-m_PageSize.width)/2;
                movePages(Vec2(realOffset,0));
                return false;
            }
            break;
            
        case TouchDirection::RIGHT: // right
            
            if (_leftBoundaryChild->getLeftBoundary()-(getContentSize().width-m_PageSize.width)/2 + touchOffset >= _leftBoundary)
            {
                realOffset = _leftBoundary - _leftBoundaryChild->getLeftBoundary()+(getContentSize().width-m_PageSize.width)/2;
                movePages(Vec2(realOffset,0));
                return false;
            }
            break;
        default:
            break;
    }
    
    movePages(Vec2(realOffset,0));
    refresh();
    return true;
}

void FlowPageView::refresh()
{
    int index =0;
    float delt = (getContentSize().width-m_PageSize.width)/2;
    for (auto& page : this->getPages())
    {
        float offset = page->getPositionX()-delt;
        if (page->getChildren().size()>0) {
            if (fabs(offset)>m_PageSize.width) {
                page->getChildren().at(0)->setScale(m_fMinScale);
            }else{
                page->getChildren().at(0)->setScale(m_fMaxScale-fabs(offset)/m_PageSize.width*(m_fMaxScale-m_fMinScale));
            }
//            log("===%f",page->getChildren().at(0)->getScale());
        }
        index++;
    }
    
}

void FlowPageView::scrollToPageAnimation(int idx,float duration)
{
    if (idx < 0 || idx >= this->getPageCount())
    {
        return;
    }
    _curPageIdx = idx;
    Layout* curPage = _pages.at(idx);
    _autoScrollDistance = -(curPage->getPosition().x)+(getContentSize().width-m_PageSize.width)/2;
    if (_autoScrollSpeed==0) {
        _autoScrollSpeed = fabs(_autoScrollDistance)/0.2f;
    }
    _autoScrollSpeed = fabs(_autoScrollDistance)/duration;
    _autoScrollDirection = _autoScrollDistance > 0 ? AutoScrollDirection::RIGHT : AutoScrollDirection::LEFT;
    _isAutoScrolling = true;
    if (scrollEnd) {
        scrollEnd(_curPageIdx);
    }
    
}
void FlowPageView::scrollToPage(ssize_t idx)
{
    if (idx < 0 || idx >= this->getPageCount())
    {
        return;
    }
    _curPageIdx = idx;
    Layout* curPage = _pages.at(idx);
    _autoScrollDistance = -(curPage->getPosition().x)+(getContentSize().width-m_PageSize.width)/2;
//    if (_autoScrollSpeed==0) {
//        _autoScrollSpeed = fabs(_autoScrollDistance)/0.2f;
//    }
    _autoScrollSpeed = fabs(_autoScrollDistance)/0.2f;
    _autoScrollDirection = _autoScrollDistance > 0 ? AutoScrollDirection::RIGHT : AutoScrollDirection::LEFT;
    _isAutoScrolling = true;
    if (scrollEnd) {
        scrollEnd(_curPageIdx);
    }
}

void FlowPageView::update(float dt)
{
    if (_isAutoScrolling)
    {
        this->autoScroll(dt);
    }
}

void FlowPageView::autoScroll(float dt)
{
    switch (_autoScrollDirection)
    {
        case AutoScrollDirection::LEFT:
        {
            float step = _autoScrollSpeed*dt;
            if (_autoScrollDistance + step >= 0.0f)
            {
                step = -_autoScrollDistance;
                _autoScrollDistance = 0.0f;
                _isAutoScrolling = false;
            }
            else
            {
                _autoScrollDistance += step;
            }
            scrollPages(-step);
            if (!_isAutoScrolling)
            {
                pageTurningEvent();
            }
            break;
        }
            break;
        case AutoScrollDirection::RIGHT:
        {
            float step = _autoScrollSpeed*dt;
            if (_autoScrollDistance - step <= 0.0f)
            {
                step = _autoScrollDistance;
                _autoScrollDistance = 0.0f;
                _isAutoScrolling = false;
            }
            else
            {
                _autoScrollDistance -= step;
            }
            scrollPages(step);
            
            if (!_isAutoScrolling)
            {
                pageTurningEvent();
            }
            
            break;
        }
        default:
            break;
    }
    
}

void FlowPageView::handleReleaseLogic(Touch *touch)
{
//    PageView::handleReleaseLogic(touch);
    if (this->getPageCount() <= 0)
    {
        return;
    }
    Widget* curPage = dynamic_cast<Widget*>(this->getPages().at(_curPageIdx));
    if (curPage)
    {
        Vec2 curPagePos = curPage->getPosition();
        ssize_t pageCount = this->getPageCount();
        float curPageLocation = curPagePos.x;
        float pageWidth = m_PageSize.width;
        float boundary = pageWidth/2.0f;
        float scale = 10;
        if (!m_bIsScrollByPage) {
            
            Vec2 pos = Vec2((getContentSize().width-m_PageSize.width)/2+startPointOffset.x, startPointOffset.y);
            
            float delta = curPageLocation-pos.x;
            if (delta>scale) {
    //            向右滑动  _curPageIdx减小
                int pageOffset = (delta+pageWidth/2)/pageWidth;
                if (pageOffset<1) {
                    pageOffset = 1;
                }
                if (_curPageIdx <= 0){
                    scrollToPage(0);
                }else if (_curPageIdx-pageOffset <= 0){
                    scrollToPage(0);
                }else{
                    scrollToPage(_curPageIdx-pageOffset);
                }
                
            }else if (delta<-scale){
    //            向左滑动  _curPageIdx增大
                int pageOffset = fabs(delta-pageWidth/2)/pageWidth;
                if (pageOffset<1) {
                    pageOffset = 1;
                }
                if (_curPageIdx >= pageCount-1){
                    scrollToPage(_curPageIdx);
                }else if (_curPageIdx+pageOffset >= pageCount-1){
                    scrollToPage(pageCount-1);
                }else{
                    scrollToPage(_curPageIdx+pageOffset);
                }
            }else{
                scrollToPage(_curPageIdx);
            }
            return;
        }

        if (curPageLocation <= -boundary-20)
        {
            if (_curPageIdx >= pageCount-1)
            {
//                scrollPages(-curPageLocation);
                scrollToPage(_curPageIdx);
            }
            else
            {
                scrollToPage(_curPageIdx+1);
            }
        }
        else if (curPageLocation <= boundary*0.5)
        {
            if (_curPageIdx >= pageCount-1)
            {
                //                scrollPages(-curPageLocation);
                scrollToPage(0);
            }
            else
            {
                scrollToPage(_curPageIdx+1);
            }
        }
        else if (curPageLocation >= boundary*1.5)
        {
            if (_curPageIdx <= 0)
            {
//                scrollPages(-curPageLocation);
                scrollToPage(0);
            }
            else
            {
                scrollToPage(_curPageIdx-1);
            }
        }
        else
        {
            scrollToPage(_curPageIdx);
        }
    }
}

