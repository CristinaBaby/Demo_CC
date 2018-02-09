//
//  GridView.cpp
//  PuzzleTest
//
//  Created by zhangguangzong1 on 15/6/9.
//
//

#include "GridView.h"

GridView::GridView():
m_bdoLayoutDirty(false),m_pTouchListener(nullptr),
m_eDirection(Direction::VERTICAL),m_iRow(1),m_iCol(1),
m_fMarginTop(0),m_fMarginLeft(0),m_fMarginRight(0),
m_fMarginBottom(0),m_fHSpace(0),m_fVSpace(0),m_bAutoScrolling(false),
m_fAdjustDistance(0),m_fMinOffset(0),m_fMaxOffset(0),m_fSpeed(0),m_pCurTouchNode(nullptr),
m_fMove(0),m_fnClickCall(nullptr),m_pAdapter(nullptr),m_bThrowEnable(true),
m_bAdjustEnable(false),m_bScrollOffset(false),m_iUpdateCounter(0),
m_eOffsetDir(OffsetDirection::LeftAndTop)
{
}

GridView::~GridView()
{
    CC_SAFE_RELEASE_NULL(m_pAdapter);
}

GridView* GridView::create(const Size &contentSize)
{
    GridView* grid = new GridView();
    if(grid && grid->initWithSize(contentSize))
    {
        grid->autorelease();
        return grid;
    }
    CC_SAFE_DELETE(grid);
    return nullptr;
}

bool GridView::initWithSize(const Size &contentSize)
{
    if (!Layer::init())
        return false;
    
    m_oViewSize = contentSize;
    this->setContentSize(contentSize);
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    m_pContainer = Layer::create();
    m_pContainer->setContentSize(contentSize);
    m_pContainer->ignoreAnchorPointForPosition(false);
    m_pContainer->setAnchorPoint(Vec2(0,1));
    m_pContainer->setPosition(Vec2(0,m_oViewSize.height));
    this->addChild(m_pContainer,1);
    
    
    this->setTouchEnabled(true);
    
    this->scheduleUpdate();
    
    return true;
}

void GridView::addGridChild(Node* child)
{
    if(m_vItems.find(child) != m_vItems.end())
        assert("child already added. It can't be added again");
    m_vItems.pushBack(child);
    m_pContainer->addChild(child);
    
    m_bdoLayoutDirty = true;
}

void GridView::addGridChildren(const Vector<Node*>& vec)
{
    for (auto it = vec.begin(); it!= vec.end(); it++)
    {
        this->addGridChild(*it);
    }
}

void GridView::removeItem(Node* node)
{
    if (m_vItems.find(node)!=m_vItems.end())
    {
        node->removeFromParent();
        m_vItems.eraseObject(node);
        m_bdoLayoutDirty = true;
    }
}

void GridView::setDirection(Direction dir,int row_or_col)
{
    m_eDirection = dir;
    switch (m_eDirection) {
        case Direction::HORIZONTAL:
            m_iRow = row_or_col;
            m_fAdjustStep = m_oViewSize.width;
            break;
        case Direction::VERTICAL:
            m_iCol = row_or_col;
            m_fAdjustStep = m_oViewSize.height;
            break;
        default:
            m_iCol = row_or_col;
            m_fAdjustStep = m_oViewSize.height;
            break;
    }
}

Node* GridView::getItemNode(long index)
{
    if(index <= 0 || index > m_vItems.size())
        return nullptr;
    return m_vItems.at(index - 1);
}

void GridView::refresh()
{
    m_bdoLayoutDirty = true;
    if (m_pAdapter)
    {
        m_pAdapter->refresh();
    }
}

void GridView::setAdapter(Adapter* adapter)
{
    CC_SAFE_RELEASE_NULL(m_pAdapter);
    m_pAdapter = adapter;
    CC_SAFE_RETAIN(m_pAdapter);
}

void GridView::setContentOffset(float offset)
{
    Vec2 pos(0,m_oViewSize.height);
    switch (m_eDirection) {
        case Direction::HORIZONTAL:
            pos.x = offset;
            break;
        case Direction::VERTICAL:
            pos.y = m_oViewSize.height + offset;
            break;
        default:
            break;
    }
    m_pContainer->setPosition(pos);
}

void GridView::setTouchEnabled(bool enabled)
{
    _eventDispatcher->removeEventListener(m_pTouchListener);
    m_pTouchListener = nullptr;
    
    if (enabled)
    {
        m_pTouchListener = EventListenerTouchOneByOne::create();
        m_pTouchListener->setSwallowTouches(true);
        m_pTouchListener->onTouchBegan = CC_CALLBACK_2(GridView::touchBegin, this);
        m_pTouchListener->onTouchMoved = CC_CALLBACK_2(GridView::touchMove, this);
        m_pTouchListener->onTouchEnded = CC_CALLBACK_2(GridView::touchEnd, this);
        m_pTouchListener->onTouchCancelled = CC_CALLBACK_2(GridView::touchCancel, this);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pTouchListener, this);
    }
}

bool GridView::isTouchEnabled() const
{
    return _touchListener != nullptr;
}

void GridView::update(float delta)
{
    if (m_bScrollOffset)
    {
        if (m_iUpdateCounter >= 2)
        {
            m_bScrollOffset = false;
            m_iUpdateCounter = 0;
            this->scrollContentOffset();
        }
        m_iUpdateCounter++;
    }
    
    if (m_bAutoScrolling)
    {
        this->autoScroll(delta);
    }
}

void GridView::moveContainer(const Vec2& move)
{
    m_pContainer->setPosition(m_pContainer->getPosition() + move);
}

void GridView::decelerateScrolling(float dt)
{
    m_oScrollDistance = m_oScrollDistance * SCROLL_RATE;
    this->moveContainer(m_oScrollDistance);
    
    if ((fabs(m_oScrollDistance.x) <= 1.0f && fabs(m_oScrollDistance.y) <= 1.0f)
        || isOutOfOffset())
    {
        this->unschedule(schedule_selector(GridView::decelerateScrolling));
        this->countAdjustDistance();
    }
}

bool GridView::isOutOfOffset()
{
    float offset;
    switch (m_eDirection)
    {
        case Direction::VERTICAL:
            offset = m_pContainer->getPositionY();
            break;
        case Direction::HORIZONTAL:
            offset = m_pContainer->getPositionX();
            break;
        default:
            break;
    }
    if (offset >= m_fMinOffset && offset <= m_fMaxOffset)
    {
        return false;
    }
    return true;
}

float GridView::getMaxOffset()
{
    float maxOffset = 0.0f;
    switch (m_eDirection)
    {
        case Direction::VERTICAL:
            maxOffset = m_fMaxOffset - m_oViewSize.height;
            break;
        case Direction::HORIZONTAL:
            maxOffset = m_fMinOffset;
            break;
        default:
            break;
    }
    
    return maxOffset;
}

void GridView::scrollContentOffset()
{
    switch (m_eOffsetDir) {
        case OffsetDirection::LeftAndTop:
            this->scrollToLeftAndTop();
            break;
        case OffsetDirection::RightAndBottom:
            this->scrollToRightAndBottom();
            break;
        default:
            break;
    }
}

void GridView::scrollToLeftAndTop(float duration,bool isFromRightAndBottom)
{
    m_fScrollDuration = duration;
    m_bFromAnother = isFromRightAndBottom;
    m_eOffsetDir = OffsetDirection::LeftAndTop;
    
    m_bScrollOffset = true;
}

void GridView::scrollToRightAndBottom(float duration,bool isFromLeftAndTop)
{
    m_fScrollDuration = duration;
    m_bFromAnother = isFromLeftAndTop;
    m_eOffsetDir = OffsetDirection::RightAndBottom;
    
    m_bScrollOffset = true;
}

void GridView::scrollToLeftAndTop()
{
    if (m_bFromAnother)
        this->setContentOffset(this->getMaxOffset());
    Vec2 pos = m_pContainer->getPosition();
    switch (m_eDirection)
    {
        case Direction::VERTICAL:
            m_fAdjustDistance = m_oViewSize.height - pos.y;
            if (m_fAdjustDistance > 0)
                m_fAdjustDistance = 0;
            break;
        case Direction::HORIZONTAL:
            m_fAdjustDistance = -pos.x;
            if (m_fAdjustDistance < 0)
                m_fAdjustDistance = 0;
            break;
        default:
            break;
    }
    m_bAutoScrolling = true;
    m_fSpeed = m_fAdjustDistance / m_fScrollDuration ;
}

void GridView::scrollToRightAndBottom()
{
    if (m_bFromAnother)
        this->setContentOffset(0);
    
    Vec2 pos = m_pContainer->getPosition();
    Size size = m_pContainer->getContentSize();
    switch (m_eDirection)
    {
        case Direction::VERTICAL:
            m_fAdjustDistance = size.height - pos.y;
            if (m_fAdjustDistance < 0)
                m_fAdjustDistance = 0;
            break;
        case Direction::HORIZONTAL:
            m_fAdjustDistance = (m_oViewSize.width - size.width) - pos.x;
            if (m_fAdjustDistance > 0)
                m_fAdjustDistance = 0;
            break;
        default:
            break;
    }
    
    m_bAutoScrolling = true;
    m_fSpeed = m_fAdjustDistance / m_fScrollDuration ;
}

void GridView::countAdjustDistance()
{
    float offset;
    switch (m_eDirection)
    {
        case Direction::VERTICAL:
            offset = m_pContainer->getPositionY();
            break;
        case Direction::HORIZONTAL:
            offset = m_pContainer->getPositionX();
            break;
        default:
            break;
    }
    if (offset >= m_fMinOffset && offset <= m_fMaxOffset)
    {
        if (m_bAdjustEnable)
        {
            /*默认按页调整*/
            m_bAutoScrolling = true;
            m_fAdjustDistance = getAdjustDistance();
            m_fSpeed = m_fAdjustDistance / 0.3f ;
        }
    }else
    {
        m_bAutoScrolling = true;
        
        if(offset < m_fMinOffset)
        {
            m_fAdjustDistance = m_fMinOffset - offset;
        }else
        {
            m_fAdjustDistance = m_fMaxOffset - offset;
        }
        m_fSpeed = m_fAdjustDistance / 0.3f ;
    }
}

float GridView::getAdjustDistance()
{
    Vec2 curPos = m_pContainer->getPosition();
    Vec2 deltaPos = curPos - m_oBeginTouch;
    int deltaOffset = 0;
    switch (m_eDirection)
    {
        case Direction::VERTICAL:
            deltaOffset = deltaPos.y;
            break;
        case Direction::HORIZONTAL:
            deltaOffset = deltaPos.x;
            break;
        default:
            break;
    }
    
    int i = abs(deltaOffset) % m_fAdjustStep;
    if (!i)
    {
        return 0;
    }
    
    if (i < m_fAdjustStep / 2)
    {
        return -i * (abs(deltaOffset)/deltaOffset);
    }else
    {
        return (m_fAdjustStep - i) * (abs(deltaOffset)/deltaOffset);
    }
}

void GridView::autoScroll(float f)
{
    float move = 0;
    float step = m_fSpeed * f;
    m_fAdjustDistance-=step;
    move = step;
    if (step > 0)
    {
        if (m_fAdjustDistance <= 0)
        {
            m_bAutoScrolling = false;
            move = step + m_fAdjustDistance;
        }
    }else
    {
        if (m_fAdjustDistance >= 0)
        {
            m_bAutoScrolling = false;
            move = step + m_fAdjustDistance;
        }
    }
    switch (m_eDirection)
    {
        case Direction::VERTICAL:
            this->moveContainer(Vec2(0,move));
            break;
        case Direction::HORIZONTAL:
            this->moveContainer(Vec2(move,0));
            break;
        default:
            break;
    }
}

bool GridView::touchBegin(Touch* touch,Event* event)
{
    if (!this->isVisible())
    {
        return false;
    }
    
    Rect frame = getViewRect();
    if (frame.containsPoint(touch->getLocation()))
    {
        m_bAutoScrolling = false;
        m_pCurTouchNode = getTouchNode(touch);
        m_fMove = 0.0f;
        m_oScrollDistance = Vec2::ZERO;
        m_oBeginTouch = m_pContainer->getPosition();
        if(m_pAdapter && m_pCurTouchNode)
            m_pAdapter->itemDown(m_pCurTouchNode);
        return true;
    }
    
    return false;
}

void GridView::touchMove(Touch* touch,Event* event)
{
    Vec2 moveDistance = touch->getDelta();
    switch (m_eDirection)
    {
        case Direction::VERTICAL:
            m_oScrollDistance = Vec2(0.0f, moveDistance.y);
            break;
        case Direction::HORIZONTAL:
            m_oScrollDistance = Vec2(moveDistance.x, 0.0f);
            break;
        default:
            break;
    }
    Rect frame = getViewRect();
    if (frame.containsPoint(touch->getLocation()))
    {
        handleMoveLogic(touch);
    }else
    {
        handleReleaseLogic(touch);
    }
    if (m_fMove <= CLICK_THRESHOLD)
    {
        Vec2 delta = touch->getDelta();
        m_fMove+=delta.length();
        if (m_fMove > CLICK_THRESHOLD)
        {
            if(m_pAdapter && m_pCurTouchNode)
                m_pAdapter->itemCancel(m_pCurTouchNode);
            m_pCurTouchNode = nullptr;
        }
    }
}

void GridView::touchEnd(Touch* touch,Event* event)
{
    handleReleaseLogic(touch);
    m_fMove = 0.0f;
    if (m_pCurTouchNode)
    {
        if(m_pAdapter)
            m_pAdapter->itemUp(m_pCurTouchNode);
        if (m_fnClickCall)
        {
            long index = m_vItems.getIndex(m_pCurTouchNode);
            m_fnClickCall(this,index + 1);
        }
    }
    
    if (m_bThrowEnable)
    {
        this->schedule(schedule_selector(GridView::decelerateScrolling));
    }
}

void GridView::touchCancel(Touch* touch,Event* event)
{
    handleReleaseLogic(touch);
    if(m_pAdapter && m_pCurTouchNode)
        m_pAdapter->itemCancel(m_pCurTouchNode);
    m_pCurTouchNode = nullptr;
}

void GridView::handleMoveLogic(Touch *touch)
{
    Vec2 delta = touch->getDelta();
    switch (m_eDirection) {
        case Direction::VERTICAL:
            delta.x = 0;
            break;
        case Direction::HORIZONTAL:
            delta.y = 0;
            break;
        default:
            break;
    }
    moveContainer(delta);
}

void GridView::handleReleaseLogic(Touch *touch)
{
    this->countAdjustDistance();
}

Node* GridView::getTouchNode(Touch* touch)
{
    Node* node = nullptr;
    Vec2 touchPos = touch->getLocation();
    for (auto it = m_vItems.begin(); it != m_vItems.end(); it++)
    {
        Rect rect;
        rect.origin = (*it)->convertToWorldSpace(Vec2::ZERO);
        rect.size = (*it)->getContentSize();
        if (rect.containsPoint(touchPos))
        {
            node = (*it);
            break;
        }
    }
    return node;
}

/*==============显示区域控制==============*/
void GridView::beforeDraw()
{
    m_oBeforeDrawCommand.init(_globalZOrder);
    m_oBeforeDrawCommand.func = CC_CALLBACK_0(GridView::onBeforeDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&m_oBeforeDrawCommand);
}

void GridView::onBeforeDraw()
{
    m_bSsissorRestored = false;
    Rect frame = getViewRect();
    auto glview = Director::getInstance()->getOpenGLView();
    
    if (glview->isScissorEnabled()) {
        m_bSsissorRestored = true;
        m_oParentScissorRect = glview->getScissorRect();
        //set the intersection of m_oParentScissorRect and frame as the new scissor rect
        if (frame.intersectsRect(m_oParentScissorRect)) {
            float x = MAX(frame.origin.x, m_oParentScissorRect.origin.x);
            float y = MAX(frame.origin.y, m_oParentScissorRect.origin.y);
            float xx = MIN(frame.origin.x+frame.size.width, m_oParentScissorRect.origin.x+m_oParentScissorRect.size.width);
            float yy = MIN(frame.origin.y+frame.size.height, m_oParentScissorRect.origin.y+m_oParentScissorRect.size.height);
            glview->setScissorInPoints(x, y, xx-x, yy-y);
        }
    }
    else {
        glEnable(GL_SCISSOR_TEST);
        glview->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
    }
}

void GridView::afterDraw()
{
    m_oAfterDrawCommand.init(_globalZOrder);
    m_oAfterDrawCommand.func = CC_CALLBACK_0(GridView::onAfterDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&m_oAfterDrawCommand);
}

void GridView::onAfterDraw()
{
    if (m_bSsissorRestored)
    {
        //restore the parent's scissor rect
        auto glview = Director::getInstance()->getOpenGLView();
        
        glview->setScissorInPoints(m_oParentScissorRect.origin.x, m_oParentScissorRect.origin.y, m_oParentScissorRect.size.width, m_oParentScissorRect.size.height);
    }
    else {
        glDisable(GL_SCISSOR_TEST);
    }
}

Rect GridView::getViewRect()
{
    Vec2 screenPos = this->convertToWorldSpace(Vec2::ZERO);
    
    float scaleX = this->getScaleX();
    float scaleY = this->getScaleY();
    
    for (Node *p = _parent; p != nullptr; p = p->getParent()) {
        scaleX *= p->getScaleX();
        scaleY *= p->getScaleY();
    }

    if(scaleX<0.f)
    {
        screenPos.x += m_oViewSize.width*scaleX;
        scaleX = -scaleX;
    }
    
    if(scaleY<0.f)
    {
        screenPos.y += m_oViewSize.height*scaleY;
        scaleY = -scaleY;
    }
    
    return Rect(screenPos.x, screenPos.y, m_oViewSize.width*scaleX, m_oViewSize.height*scaleY);
}

void GridView::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
    // quick return if not visible
    if (!isVisible())
    {
        return;
    }
    
    doLayout();
    
    uint32_t flags = processParentFlags(parentTransform, parentFlags);
    
    Director* director = Director::getInstance();
    CCASSERT(nullptr != director, "Director is null when seting matrix stack");
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);
    
    this->beforeDraw();
    
    if (!_children.empty())
    {
        int i=0;
        
        // draw children zOrder < 0
        for( ; i < _children.size(); i++ )
        {
            Node *child = _children.at(i);
            if ( child->getLocalZOrder() < 0 )
            {
                child->visit(renderer, _modelViewTransform, flags);
            }
            else
            {
                break;
            }
        }
        
        // this draw
        this->draw(renderer, _modelViewTransform, flags);
        
        // draw children zOrder >= 0
        for( ; i < _children.size(); i++ )
        {
            Node *child = _children.at(i);
            child->visit(renderer, _modelViewTransform, flags);
        }
        
    }
    else
    {
        this->draw(renderer, _modelViewTransform, flags);
    }
    
    this->afterDraw();
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}


/*==============布局控制==============*/
void GridView::doLayout()
{
    if (!m_bdoLayoutDirty)
        return;
    
    updateAllItemsPosition();
    
    m_bdoLayoutDirty = false;
}

void GridView::updateAllItemsPosition()
{
    this->countRowAndCol();//计算行列
    /*计算container的长宽*/
    float width = this->getTotalWidth();
    float height = this->getTotalHeight();
    width = MAX(width, m_oViewSize.width);
    height = MAX(height, m_oViewSize.height);
    
    //重新计算container的尺寸
    m_pContainer->setContentSize(Size(width,height));
    //重新计算最大偏移量
    
    
    switch (m_eDirection)
    {
        case Direction::VERTICAL:
            m_fMaxOffset = height;
            m_fMinOffset = m_oViewSize.height;
            this->verticalLayout();
            break;
        case Direction::HORIZONTAL:
            m_fMaxOffset = 0;
            m_fMinOffset = m_oViewSize.width - width;
            this->horizontalLayout();
            break;
        default:
            break;
    }
}

void GridView::horizontalLayout()
{
    float x = 0 + m_fMarginLeft;
    float y = m_pContainer->getContentSize().height;
    /*临时变量标记当前行最大宽度的值,通常情况下所有Item一样大,该值是个定值*/
    float tempX;
    for (long col = 0; col < m_iCol; col++)
    {
        tempX = 0;
        y = m_pContainer->getContentSize().height - m_fMarginTop;
        for (long row = 0; row < m_iRow; row++)
        {
            long index = col * m_iRow + row;
            if(index >= 0 && index < m_vItems.size())
            {
                Node* item = m_vItems.at(index);
                item->ignoreAnchorPointForPosition(false);
                item->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                Size itemSize = item->getContentSize();
                y -= itemSize.height / 2;
                item->setPosition(x + itemSize.width / 2,y);
                log("x = %f,y = %f",x + itemSize.width / 2,y);
                y -= itemSize.height / 2 + m_fVSpace;
                
                tempX = MAX(tempX, itemSize.width);
            }
        }
        x += (tempX + m_fHSpace);
    }
}

void GridView::verticalLayout()
{
    float x;
    float y = m_pContainer->getContentSize().height - m_fMarginTop;
    /*临时变量标记当前行最大高度的值,通常情况下所有Item一样大,该值是个定值*/
    float tempY;
    for (long row = 0; row < m_iRow; row++)
    {
        tempY = 0;
        x = m_fMarginLeft;
        for (long col = 0; col < m_iCol; col++)
        {
            long index = row * m_iCol + col;
            if(index >= 0 && index < m_vItems.size())
            {
                Node* item = m_vItems.at(index);
                item->ignoreAnchorPointForPosition(false);
                item->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                Size itemSize = item->getContentSize();
                x += itemSize.width / 2;
                item->setPosition(x,y - itemSize.height / 2);
                x += itemSize.width / 2 + m_fHSpace;
                
                tempY = MAX(tempY, itemSize.height);
            }
        }
        y -= (tempY + m_fVSpace);
    }
}

void GridView::countRowAndCol()
{
    long totalCount = m_vItems.size();
    switch (m_eDirection)
    {
        case Direction::VERTICAL:
            m_iRow = totalCount % m_iCol ? totalCount / m_iCol + 1 : totalCount / m_iCol ;
            break;
        case Direction::HORIZONTAL:
            m_iCol = totalCount % m_iRow ? totalCount / m_iRow + 1 : totalCount / m_iRow ;
            break;
        default:
            break;
    }
}

float GridView::getTotalWidth()
{
    float totalWidth = m_fMarginLeft + m_fMarginRight;
    float tempWidth;
    for (long col = 0; col < m_iCol; col++)
    {
        tempWidth = 0;
        for (long row = 0; row < m_iRow; row++)
        {
            long index = row * m_iRow + col;
            if(index >= 0 && index < m_vItems.size())
            {
                Node* item = m_vItems.at(index);
                tempWidth = MAX(tempWidth, item->getContentSize().width);
            }
        }
        if (col == m_iCol - 1)
        {
            totalWidth+= tempWidth ;
        }else
        {
            totalWidth+= (tempWidth + m_fHSpace);
        }
    }
    return totalWidth;
}

float GridView::getTotalHeight()
{
    float totalHeight = m_fMarginTop + m_fMarginBottom;
    float tempHeight;
    for (long row = 0; row < m_iRow; row++)
    {
        tempHeight = 0;
        for (long col = 0; col < m_iCol; col++)
        {
            long index = row * m_iCol + col;
            if(index >= 0 && index < m_vItems.size())
            {
                Node* item = m_vItems.at(index);
                tempHeight = MAX(tempHeight, item->getContentSize().height);
            }
        }
        if (row == m_iRow - 1)
        {
            totalHeight+= tempHeight ;
        }else
        {
            totalHeight+= (tempHeight + m_fVSpace);
        }
    }
    return totalHeight;
}

/*==================Adapter====================*/
void Adapter::setDatas(const Vector<Node*>& datas)
{
    //移除之前的数据
    for (auto it = m_vData.begin(); it != m_vData.end(); it++)
    {
        m_pGrid->removeItem(*it);
    }
    m_vData.clear();
    m_vData.pushBack(datas);
    for (long i = 0,size = m_vData.size(); i < size; i++)
    {
        itemView(m_vData.at(i),i+1);
        m_pGrid->addGridChild(m_vData.at(i));
    }
}

void Adapter::addDatas(const Vector<Node*>& datas)
{
    m_vData.pushBack(datas);
    for (long i = 0,size = datas.size(); i < size; i++)
    {
        long index = m_vData.getIndex(datas.at(i));
        itemView(datas.at(i),index+1);
        m_pGrid->addGridChild(datas.at(i));
    }
}

void Adapter::addData(Node* data)
{
    m_vData.pushBack(data);

    long index = m_vData.getIndex(data);
    itemView(data,index+1);
    m_pGrid->addGridChild(data);
}

void Adapter::refresh()
{
    for (long i = 0,size = m_vData.size(); i < size; i++)
    {
        itemRefresh(m_vData.at(i), i + 1);
    }
}

long Adapter::getCount()
{
    return m_vData.size();
}

bool Adapter::isEmpty()
{
    return m_vData.size() == 0;
}

void Adapter::itemDown(Node* node)
{
    node->setOpacity(150);
}

void Adapter::itemUp(Node* node)
{
    node->setOpacity(255);
}

void Adapter::itemCancel(Node* node)
{
    node->setOpacity(255);
}

void Adapter::itemView(Node* item,long index){}
void Adapter::itemRefresh(Node* item,long index){}