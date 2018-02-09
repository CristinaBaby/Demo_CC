//
//  CircularScrollView.cpp
//  HHZ001_TEST
//
//  Created by huanghanzhi on 24/3/16.
//
//

#include "CircularScrollView.h"

// 抛投衰减速率
const float SCROLL_RATE     = 0.9f;

// 点击事件阈值，当move超过该值，取消点击
const float CLICK_THRESHOLD = 10;

#pragma mark 初始化
CircularScrollView* CircularScrollView::create(const Size &viewSize)
{
    CircularScrollView* view = new CircularScrollView();
    view->initWithSize(viewSize);
    view->autorelease();
    return view;
}

bool CircularScrollView::initWithSize(const Size &viewSize)
{
    if (!LayerColor::initWithColor(Color4B(150, 0, 0, 0)))
        return false;
    
    // 设置循环滚动层锚点为中点
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    // 设置ViewSize
    setViewSize(viewSize);
    setContentSize(viewSize);
    setTouchEnable(true);
    
    _directClip = false;
    return true;
}

#pragma mark - 添加元素
void CircularScrollView::addItem(Node* child)
{
    if (_vItems.find(child) != _vItems.end())
        assert("child already added. It can't be added again.");
    this->addChild(child);
    _vItems.pushBack(child);
    
    _doLayoutDirty = true;
}

void CircularScrollView::addItem(Vector<Node*> vChildren)
{
    for (const auto &child : vChildren)
    {
        this->addItem(child);
    }
}

#pragma mark 移除元素
void CircularScrollView::removeItem(Node *node)
{
    if (_vItems.find(node) != _vItems.end())
    {
        node->removeFromParent();
        _vItems.eraseObject(node);
        _doLayoutDirty = true;
    }
}

#pragma mark 设置Adapter
void CircularScrollView::setAdapter(CircularAdapter *adapter)
{
    CC_SAFE_RELEASE_NULL(_adapter);
    _adapter = adapter;
    CC_SAFE_RETAIN(_adapter);
}

#pragma mark - 布局控制
void CircularScrollView::doLayout()
{
    if (!_doLayoutDirty)
    {
        return;
    }
    
    updateAllItemsPosition();
    
    _doLayoutDirty = false;
}

void CircularScrollView::updateAllItemsPosition()
{
    // 设置单元格大小
    for (const auto &item : _vItems)
    {
        _cellSize.width = MAX(item->getContentSize().width, _cellSize.width);
        _cellSize.height = MAX(item->getContentSize().height, _cellSize.height);
    }
//    _cellSize.width += _space;
    _cellSize.height += _space;
    
    // 容器总宽度
//    _totalWidth = _cellSize.width * _vItems.size();
    _totalHeight = _cellSize.height * _vItems.size();
    
    // 设置容器可视区域大小
    countNewViewSize();
    
    // 排列容器中的item
//    horizontalLayout();
    verticalLayout();
    
    // 计算左右两个边界
    countBounds();
}

void CircularScrollView::countNewViewSize()
{
    // 可视区域大小需要满足以下公式，否则可视区域大小将为一个单元格大小
    // 可视区域大小 <= (所有单元格大小之和 - 2个单元格大小之和)
    if (_viewSize.height <= (_cellSize.height - _space) * (_vItems.size() - 2))
    {
        _viewSize.width = MAX(_cellSize.width, _viewSize.width);
        _viewSize.height = MAX(_cellSize.height, _viewSize.height);
    }
    else
    {
        _viewSize = _cellSize;
    }
    this->setContentSize(_viewSize);
}

void CircularScrollView::verticalLayout(){
    for (decltype(_vItems.size()) i = 0; i != _vItems.size(); ++i)
    {
        Node* item = _vItems.at(i);
        if (i == _vItems.size() - 1)
        {
            item->setPosition(Vec2(getContentSize().width * 0.5, -_cellSize.height* 0.5));
        }
        else
        {
            item->setPosition(Vec2(getContentSize().width * 0.5, _cellSize.height * 0.5 + _cellSize.height * i));
        }
        float dis = pow(abs(item->getPositionY() - item->getParent()->getContentSize().height/2)/item->getParent()->getContentSize().height/2 * 50 + 1, 2) * 0.45f;
        item->setPositionX(item->getPositionX() + (_directClip?1:-1)*dis);
        log("oldX = %f", item->getParent()->convertToWorldSpace(item->getPosition()).x);
    }
}

void CircularScrollView::horizontalLayout()
{
    for (decltype(_vItems.size()) i = 0; i != _vItems.size(); ++i)
    {
        Node* item = _vItems.at(i);
        if (i == _vItems.size() - 1)
        {
            item->setPosition(Vec2(-_cellSize.width* 0.5, getContentSize().height * 0.5));
        }
        else
        {
            item->setPosition(Vec2(_cellSize.width * 0.5 + _cellSize.width * i, getContentSize().height * 0.5));
        }
    }
}

void CircularScrollView::countBounds()
{
    if (_vItems.size() != 0)
    {
        _boundLeftOrUp = _vItems.at(_vItems.size() - 1)->getPosition().y - _cellSize.height;
    //    TestUtil::addTestPointInNode(Vec2(_boundLeftOrUp, _cellSize.height * 0.5), this);
        
        _boundRightOrBottom = _vItems.at(_vItems.size() - 2)->getPosition().y + _cellSize.height;
    //    TestUtil::addTestPointInNode(Vec2(_boundRightOrBottom, _cellSize.height * 0.5), this);
    }
}

#pragma mark - 滚动控制
void CircularScrollView::moveItems(const Vec2 &delta)
{
    for (const auto &item : _vItems)
    {
        item->setPosition(item->getPosition() + delta);
        float dis = pow(abs(item->getPositionY() - item->getParent()->getContentSize().height/2)/item->getParent()->getContentSize().height/2 * 50 + 1, 2) * 0.45f;
        item->setPositionX(getContentSize().width * 0.5 + (_directClip?1:-1)*dis);
        log("newX = %f", item->getParent()->convertToWorldSpace(item->getPosition()).x);
        checkItemCirculation(item);
    }
    
    if (_fnScrollCall)
    {
        _fnScrollCall();
    }
}

#pragma mark 减速滚动
void CircularScrollView::decelerateScrolling(float dt)
{
    _scrollDistance = _scrollDistance * SCROLL_RATE;
    moveItems(_scrollDistance);
    
    if ((fabs(_scrollDistance.x) <= 1.0f && fabs(_scrollDistance.y) <= 1.0f))
    {
        this->unschedule(schedule_selector(CircularScrollView::decelerateScrolling));
        if (_adjustEnable) // 自动校正
        {
            this->schedule(schedule_selector(CircularScrollView::autoAdjustPosition));
        }
    }
}

#pragma mark 自动校正
void CircularScrollView::autoAdjustPosition(float dt)
{
    _adjustDelta = countAdjustDelta();
    if (fabs(_adjustDelta.y) <= 3.0f)
    {
        // 校正完毕
        moveItems(_adjustDelta);
        this->unschedule(schedule_selector(CircularScrollView::autoAdjustPosition));
        _isLocked = false;
        
        Node* pNode = nullptr;
        for (const auto &item : _vItems)
        {
            if (item->getPositionY()==getContentSize().height/2) {
                pNode = item;
                break;
            }
        }
        if(_adjustCall){
            _adjustCall(pNode);
        }
    }
    else
    {
        Vec2 delta = _adjustDelta.getNormalized() * 3;
        moveItems(delta);
    }
}

#pragma mark 计算自动校正距离
Vec2 CircularScrollView::countAdjustDelta()
{
    Vec2 adjustDelta;
    for (const auto &item : _vItems)
    {
        if (fabs(item->getPositionY() - _flagPosition) <= _cellSize.height * 0.5)
        {
            _flagNode = item;
            // 需要自动校正的距离
            adjustDelta = Vec2(0, _flagPosition - item->getPositionY());
        }
    }
    return adjustDelta;
}

#pragma mark 滚动到指定Item
void CircularScrollView::scrollToItem(float index, float duration)
{
    if (index <= 0 || index > _vItems.size())
    {
        log("CircularScrollView : %s : index is out of range", __FUNCTION__);
        return;
    }
    
    _specificNode = _vItems.at(index - 1);
    _distanceToSpecified = _flagPosition - _specificNode->getPosition().y;
    _autoScrollSpeed = _distanceToSpecified / duration;
    
    this->schedule(schedule_selector(CircularScrollView::autoScroll));
}

#pragma mark 滚动到第一个
void CircularScrollView::scrollToFirst(float duration)
{
    scrollToItem(1, duration);
}

#pragma mark 滚动到最后一个
void CircularScrollView::scrollToLast(float duration)
{
    scrollToItem(_vItems.size(), duration);
}

#pragma mark 自动滚动
void CircularScrollView::autoScroll(float dt)
{
    if (!_specificNode)
    {
        this->unschedule(schedule_selector(CircularScrollView::autoScroll));
        return;
    }
    _distanceToSpecified = _flagPosition - _specificNode->getPosition().y;
    if (fabs(_distanceToSpecified) < fabs(_autoScrollSpeed * dt))
    {
        moveItems(Vec2(0, _distanceToSpecified));
        this->unschedule(schedule_selector(CircularScrollView::autoScroll));
    }
    else
    {
        moveItems(Vec2(0, _autoScrollSpeed * dt));
    }
}

#pragma mark - Touch事件
void CircularScrollView::setTouchEnable(bool isTouchEnable)
{
    _eventDispatcher->removeEventListener(_touchListener);
    _touchListener = nullptr;
    
    if (isTouchEnable)
    {
        _touchListener = EventListenerTouchOneByOne::create();
        _touchListener->onTouchBegan = CC_CALLBACK_2(CircularScrollView::onTouchBegan, this);
        _touchListener->onTouchMoved = CC_CALLBACK_2(CircularScrollView::onTouchMoved, this);
        _touchListener->onTouchEnded = CC_CALLBACK_2(CircularScrollView::onTouchEnded, this);
        _touchListener->onTouchCancelled = CC_CALLBACK_2(CircularScrollView::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
    }
}

bool CircularScrollView::onTouchBegan(Touch *touch, Event *event)
{
    if (!this->isVisible())
    {
        return false;
    }
    
    if(_isLocked){
        return false;
    }
    
    if (this->getBoundingBox().containsPoint(touch->getLocation()))
    {
        _touchIsOut = false;
        _scrollDistance = Vec2::ZERO;
        _adjustDelta = Vec2::ZERO;
        _specificNode = nullptr;
        _distanceToSpecified = 0;
        
        _currentTouchNode = getTouchNode(touch);
        _move = 0.0f;
        if (_adapter && _currentTouchNode)
            _adapter->itemDown(_currentTouchNode);
        return true;
    }
    else
    {
        return false;
    }
}

void CircularScrollView::onTouchMoved(Touch *touch, Event *event)
{
    Rect frame = getViewRect();
    if (frame.containsPoint(touch->getLocation()) && !_touchIsOut)
    {
        handleMoveLogic(touch);
    }
    else
    {
        _touchIsOut = true;
    }
    if (_move <= CLICK_THRESHOLD)
    {
        Vec2 delta = touch->getDelta();
        _move += delta.length();
        if (_move > CLICK_THRESHOLD)
        {
            if (_adapter && _currentTouchNode)
                _adapter->itemCancel(_currentTouchNode);
            _currentTouchNode = nullptr;
        }
    }
}

void CircularScrollView::onTouchEnded(Touch *touch, Event *event)
{
    _move = 0.0f;
    _isLocked = true;
    if (_currentTouchNode)
    {
        if (_adapter)
            _adapter->itemUp(_currentTouchNode);
        if (_fnClickCall)
        {
            long index = _vItems.getIndex(_currentTouchNode);
            _fnClickCall(this, index + 1);
        }
    }
    
    // 抛投
    if (_throwEnable && !_touchIsOut)
    {
        this->schedule(schedule_selector(CircularScrollView::decelerateScrolling));
    }
    else if (_adjustEnable) // 自动校正
    {
        this->schedule(schedule_selector(CircularScrollView::autoAdjustPosition));
    }
}

void CircularScrollView::onTouchCancelled(Touch *touch, Event *event)
{
    if (_adapter && _currentTouchNode)
        _adapter->itemCancel(_currentTouchNode);
    _currentTouchNode = nullptr;
}

void CircularScrollView::handleMoveLogic(Touch* touch)
{
    Vec2 delta = touch->getDelta();
    _scrollDistance = Vec2(0, delta.y);
    moveItems(_scrollDistance);
}

void CircularScrollView::handleReleaseLogic(Touch* touch)
{
    
}

Node* CircularScrollView::getTouchNode(Touch *touch)
{
    Node* node = nullptr;
    for (auto iter = _vItems.begin(); iter != _vItems.end(); ++iter)
    {
        Rect rect;
        rect.origin = (*iter)->convertToWorldSpace(Vec2::ZERO);
        rect.size = (*iter)->getContentSize();
        if (rect.containsPoint(touch->getLocation()))
        {
            node = (*iter);
            break;
        }
    }
    return node;
}

#pragma mark - 循环边界判断
void CircularScrollView::checkItemCirculation(Node *node)
{
    float item_y = node->getPositionY();
    if (item_y > _boundRightOrBottom)
    {
        node->setPositionY(node->getPositionY() - _totalHeight);
    }
    else if (item_y < _boundLeftOrUp)
    {
        node->setPositionY(node->getPositionY() + _totalHeight);
    }
}

#pragma mark - 显示区域控制
void CircularScrollView::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags)
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

void CircularScrollView::beforeDraw()
{
    _beforeDrawCommand.init(_globalZOrder);
    _beforeDrawCommand.func = CC_CALLBACK_0(CircularScrollView::onBeforeDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_beforeDrawCommand);
}

void CircularScrollView::onBeforeDraw()
{
    _isScissorRestored = false;
    Rect frame = getViewRect();
    auto glview = Director::getInstance()->getOpenGLView();
    
    if (glview->isScissorEnabled())
    {
        _isScissorRestored = true;
        _parentScissorRect = glview->getScissorRect();
        //set the intersection of _parentScissorRect and frame as the new scissor rect
        if (frame.intersectsRect(_parentScissorRect))
        {
            float x = MAX(frame.origin.x, _parentScissorRect.origin.x);
            float y = MAX(frame.origin.y, _parentScissorRect.origin.y);
            float xx = MIN(frame.origin.x+frame.size.width,
                           _parentScissorRect.origin.x+_parentScissorRect.size.width);
            float yy = MIN(frame.origin.y+frame.size.height,
                           _parentScissorRect.origin.y+_parentScissorRect.size.height);
            glview->setScissorInPoints(x, y, xx-x, yy-y);
        }
    }
    else
    {
        glEnable(GL_SCISSOR_TEST);
        glview->setScissorInPoints(frame.origin.x, frame.origin.y,
                                   frame.size.width, frame.size.height);
    }
}

void CircularScrollView::afterDraw()
{
    _afterDrawCommand.init(_globalZOrder);
    _afterDrawCommand.func = CC_CALLBACK_0(CircularScrollView::onAfterDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_afterDrawCommand);
}

void CircularScrollView::onAfterDraw()
{
    if (_isScissorRestored)
    {
        //restore the parent's scissor rect
        auto glview = Director::getInstance()->getOpenGLView();
        
        glview->setScissorInPoints(_parentScissorRect.origin.x, _parentScissorRect.origin.y, _parentScissorRect.size.width, _parentScissorRect.size.height);
    }
    else
    {
        glDisable(GL_SCISSOR_TEST);
    }
}

Rect CircularScrollView::getViewRect()
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
        screenPos.x += _viewSize.width*scaleX;
        scaleX = -scaleX;
    }
    
    if(scaleY<0.f)
    {
        screenPos.y += _viewSize.height*scaleY;
        scaleY = -scaleY;
    }
    
    return Rect(screenPos.x, screenPos.y, _viewSize.width*scaleX, _viewSize.height*scaleY);
}

#pragma mark - Adapter
CircularAdapter* CircularAdapter::create(CircularScrollView *circularScrollView)
{
    CircularAdapter* adapter = new CircularAdapter(circularScrollView);
    if (adapter)
    {
        adapter->autorelease();
        return adapter;
    }
    CC_SAFE_RELEASE_NULL(adapter);
    return nullptr;
}

void CircularAdapter::setDatas(const Vector<Node *> &vDatas)
{
    // 移除之前的数据
    for (auto iter = _vDatas.begin(); iter != _vDatas.end(); ++iter)
    {
        _circularScrollView->removeItem(*iter);
    }
    _vDatas.clear();
    _vDatas.pushBack(vDatas);
    for (auto iter = _vDatas.begin(); iter != _vDatas.end(); ++iter)
    {
        itemView(*iter, iter - _vDatas.begin() + 1);
        _circularScrollView->addItem(*iter);
    }
}

void CircularAdapter::addData(Node *data)
{
    _vDatas.pushBack(data);
    
    long index = _vDatas.getIndex(data);
    itemView(data, index + 1);
    _circularScrollView->addItem(data);
}

void CircularAdapter::addDatas(const Vector<Node *> &vDatas)
{
    _vDatas.pushBack(vDatas);
    for (decltype(vDatas.size()) i = 0; i != vDatas.size(); ++i)
    {
        auto index = _vDatas.getIndex(_vDatas.at(i));
        itemView(vDatas.at(i), index + 1);
        _circularScrollView->addItem(vDatas.at(i));
    }
}

void CircularAdapter::refresh()
{
    for (decltype(_vDatas.size()) i = 0; i != _vDatas.size(); ++i)
    {
        itemRefresh(_vDatas.at(i), i + 1);
    }
}

long CircularAdapter::getCount()
{
    return _vDatas.size();
}

bool CircularAdapter::isEmpty()
{
    return _vDatas.size() == 0;
}

void CircularAdapter::itemDown(Node *item)
{
    
}

void CircularAdapter::itemUp(Node *item)
{
    
}

void CircularAdapter::itemCancel(Node *item)
{
    
}

void CircularAdapter::itemView(Node *item, long index)
{
    
}

void CircularAdapter::itemRefresh(Node *item, long index)
{
    
}
