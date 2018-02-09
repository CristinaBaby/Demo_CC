


#include "XCScrolledList1.h"

#define kBackgroundZorder 1
#define kContainerZorder 5

XCScrolledList1::XCScrolledList1()
:m_nMaxColumn(10000)
,m_fDragCallback(nullptr)
,m_bStopTouch(false)
,m_bAutoHiddenBar(true)
,m_pImage(nullptr)
,m_pBarBg(nullptr)
{
    
}

XCScrolledList1 *XCScrolledList1::create(const Size &aContentSize)
{
    XCScrolledList1 *layer = new XCScrolledList1();
    if (layer && layer->initWithXCScrolledList(aContentSize)) {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool XCScrolledList1::initWithXCScrolledList(const Size &aContentSize)
{
    
    if (!XCScrollView::initWithViewSize(aContentSize)) {
        return false;
    }
    
    setDirection(XCScrollView::Direction::VERTICAL);
    
    return true;
}

void XCScrolledList1::moveList(int aNumber)
{
    setTouchEnabled(false);
    if (_direction == Direction::VERTICAL) {
        _container->runAction(Sequence::create(EaseSineOut::create(MoveBy::create(0.3, Vec2(0, m_nMoveStepY * aNumber))), CallFunc::create([this](){
            relocateContainer(true);
            _delegate->scrollViewDidScroll(this);
            this->setTouchEnabled(true)
            ; }), nullptr));
    } else if (_direction == Direction::HORIZONTAL) {
        _container->runAction(Sequence::create(EaseSineOut::create(MoveBy::create(0.3, Vec2(m_nMoveStepX * aNumber, 0))), CallFunc::create([this](){
            relocateContainer(true);
            _delegate->scrollViewDidScroll(this);
            this->setTouchEnabled(true); }), nullptr));
    }
}

void XCScrolledList1::pushItems(std::vector<cocos2d::Node *> &aItems)
{
    if (aItems.empty()) {
        return;
    }
    
    for (auto item : aItems) {
        _container->addChild(item);
    }
    
    updateContainerSize();
    
    updateItemsPosition();
}

void XCScrolledList1::pushItem(cocos2d::Node *aItem)
{
    if (aItem == nullptr) {
        return;
    }
    
    _container->addChild(aItem);
    
    updateContainerSize();
    
    updateItemsPosition();
}

void XCScrolledList1::removeItem(const std::string &aName)
{
    removeItem(_container->getChildByName(aName));
}

void XCScrolledList1::removeItem(cocos2d::Node *aItem)
{
    if (aItem == nullptr) {
        return;
    }
    
    auto children = _container->getChildren();
    
    if (std::find(children.begin(), children.end(), aItem) == children.end()) {
        log("Not found item");
        return;
    }
    
    _container->removeChild(aItem);
    
    updateContainerSize();
    
    updateItemsPosition();
}

bool XCScrolledList1::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (m_bStopTouch == true) {
        return false;
    }
    return XCScrollView::onTouchBegan(touch, event);
}

void XCScrolledList1::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (m_bStopTouch == true) {
        return;
    }
    XCScrollView::onTouchMoved(touch, event);
}

void XCScrolledList1::cleanupScrollList()
{
    _container->removeAllChildrenWithCleanup(true);
    updateContainerSize();
    updateItemsPosition();
}

std::vector<Node *> XCScrolledList1::getShowInContentRectItems()
{
    
    std::vector<Node *> showItems;
    
    auto children = _container->getChildren();
    
    float startY = fabsf(_container->getPositionY());
    float endY = _viewSize.height + startY;
    
    for (auto ingredientIcon : children) {
        float PosY = ingredientIcon->getPositionY();
        if (PosY >= startY && PosY <= endY) {
            showItems.push_back(ingredientIcon);
        }
    }
    
    return showItems;
}

void XCScrolledList1::setBarImage(cocos2d::extension::Scale9Sprite *aBg, cocos2d::extension::Scale9Sprite *aImage)
{
    
    if (m_pBarBg) {
        m_pBarBg->removeFromParent();
        m_pBarBg = nullptr;
    }
    
    if (m_pImage) {
        m_pImage->removeFromParent();
        m_pImage = nullptr;
    }
    
    m_pBarBg = aBg;
    m_pImage = aImage;
    
    m_pBarBg->setContentSize(Size(20, _viewSize.height));
    m_pBarBg->setPosition(_viewSize.width - 10,  _viewSize.height / 2);
    m_pBarBg->setLocalZOrder(100);
    Node::addChild(m_pBarBg);
    
    m_pImage->setContentSize(Size(20, MIN(_viewSize.height * (_viewSize.height / _container->getContentSize().height ), _viewSize.height)));
    m_pImage->setLocalZOrder(101);
    Node::addChild(m_pImage);
    
    updateBarPosition();
}

void XCScrolledList1::setContentOffset(Vec2 offset, bool animated)
{
    XCScrollView::setContentOffset(offset, animated);
    updateBarPosition();
}

void XCScrolledList1::addChild(Node* child, int localZOrder, const std::string &name)
{
    Node::addChild(child, localZOrder, name);
}

void XCScrolledList1::addChild(cocos2d::Node *child, int zOrder, int tag)
{
    Node::addChild(child, zOrder, tag);
}

void XCScrolledList1::updateContainerSize()
{
    
    size_t allItemsCount = _container->getChildrenCount();
    
    size_t rowCount = allItemsCount / m_nMaxColumn < 1 ? 1 : ceil((float)allItemsCount / (float)m_nMaxColumn);
    size_t columnCount = allItemsCount > m_nMaxColumn ? m_nMaxColumn : allItemsCount;
    
    float containerWidth = columnCount * m_nMoveStepX;
    float containerHeight = rowCount  * m_nMoveStepY;
    
    _container->setContentSize(Size(containerWidth, containerHeight));
    
}

void XCScrolledList1::updateItemsPosition()
{
    short column = 1;
    
    float widthDistance = m_nMoveStepX * 0.5;
    float heightDistance = m_nMoveStepY * 0.5;
    
    int placeCount = (_container->getContentSize().width / m_nMoveStepX) *(_container->getContentSize().height / m_nMoveStepY);
    column += placeCount - _container->getChildrenCount();
    Vec2 lastPos = Vec2(_container->getContentSize().width - ((placeCount - _container->getChildrenCount()) * m_nMoveStepX), 0);
    Vector<Node *>::reverse_iterator itemItr = _container->getChildren().rbegin();
    while (itemItr != _container->getChildren().rend()) {
        
        if (column > m_nMaxColumn) {
            lastPos.y += m_nMoveStepY;
            lastPos.x = _container->getContentSize().width;
            column = 1;
        }
        
        float x = lastPos.x - widthDistance;
        float y = lastPos.y + heightDistance;
        if (x < 0 || y < 0) {
            log("error!!! calculation Position wrong！");
        } else {
            (*itemItr)->setPosition(x, y);
        }
        
        lastPos.x -= m_nMoveStepX;
        ++ column;
        ++ itemItr;
        
    }
    
    updateInset();
    
    if (_direction == XCScrollView::Direction::VERTICAL) {
        _container->setPosition(minContainerOffset());
    } else if (_direction == XCScrollView::Direction::HORIZONTAL) {
        _container->setPosition(maxContainerOffset());
    }
}

void XCScrolledList1::updateBarPosition()
{
    
    if (!m_pImage) {
        return;
    }
    
    Vec2 currentOffset = getContentOffset();
    
    Size viewSize = getViewSize();
    Size barSize = m_pImage->getContentSize();
    
    Vec2 barPos = Vec2(viewSize.width - 10, barSize.height * 0.5 + viewSize.height * (-currentOffset.y / _container->getContentSize().height));
    
    if(barPos.y < barSize.height/2)
    {
        barPos.y = barSize.height/2;
    }
    else if(barPos.y + barSize.height/2 > viewSize.height)
    {
        barPos.y = viewSize.height- barSize.height/2;
    }
    
    m_pImage->setPosition(barPos);
}

void XCScrolledList1::deaccelerateScrolling(float dt)
{
    XCScrollView::deaccelerateScrolling(dt);
    updateBarPosition();
}

XCScrolledList1::~XCScrolledList1()
{
    
}