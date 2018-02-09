
#include "ItemScrollView.h"

ItemCell* ItemCell::create(const std::string bgPath,const std::string bgHighlight,const std::string btn,const std::string lock)
{
    
    auto pRet = new ItemCell();
    if (pRet && pRet->init(bgPath,bgHighlight,btn,lock))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}
bool ItemCell::init(const std::string bgPath,const std::string bgHighlight,const std::string btn,const std::string lock)
{
    if (!Node::init() )
    {
        return false;
    };
    m_pBg = nullptr;
    onItemCellSelected = nullptr;
    m_pLock = nullptr;
    
    m_sBgPath = bgPath;
    m_sBgHighlightPath = bgHighlight;
    
    if (std::strcmp(m_sBgPath.c_str(), "")) {
        m_pBg = Sprite::create(bgPath);
        if (m_pBg) {
            this->addChild(m_pBg);
        }
    }
    if (std::strcmp(m_sBgHighlightPath.c_str(), "")) {
        m_pBgHeighlight = Sprite::create(m_sBgHighlightPath);
        if (m_pBgHeighlight) {
            this->addChild(m_pBgHeighlight);
            m_pBgHeighlight->setVisible(false);
        }
    }
    m_pBtn = Button::create(btn,btn,btn);
    m_pBtn->setPressedActionEnabled(false);
    this->addChild(m_pBtn);
    
    if (std::strcmp(lock.c_str(), "")) {
        m_pLock = Sprite::create(lock);
        if (m_pLock) {
            this->addChild(m_pLock);
            m_pLock->setPosition(Vec2(m_pBtn->getContentSize().width/2-20, -m_pBtn->getContentSize().width/2+20));
        }
    }
    m_pBtn->setTouchEnabled(false);
    this->setContentSize(m_pBtn->getContentSize());
//    m_pBtn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
//        if (type==Widget::TouchEventType::ENDED){
//            m_pBtn->setTouchEnabled(false);
////            this->setSelected(true);
//            if (onItemCellSelected) {
//                this->onItemCellSelected(this);
//            }
//        }
//    });
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch* pTouch, Event* event)
    {
        Vec2 point = pTouch->getLocation();
        point = this->convertToNodeSpace(point);
        Rect rect = m_pBtn->getBoundingBox();
        if (!rect.containsPoint(point)) {
            return false;
        }
        log("contain");
        return true;
    };
    listener->onTouchEnded = [=](Touch*, Event*)
    {
        if (onItemCellSelected) {
            this->onItemCellSelected(this);
        }
    };
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//    m_pBtn->setTouchEnabled(false);
    
    return true;
}

void ItemCell::setSelected(bool selected)
{
    if (selected) {
//        m_pBtn->setTouchEnabled(false);
        
        if (m_pBg) {
            m_pBg->setVisible(false);
        }
        if (m_pBgHeighlight) {
            m_pBgHeighlight->setVisible(true);
        }
    }else {
//        m_pBtn->setTouchEnabled(true);
        
        if (m_pBg) {
            m_pBg->setVisible(true);
        }
        if (m_pBgHeighlight) {
            m_pBgHeighlight->setVisible(false);
        }
        
    }
}



ItemScrollView::ItemScrollView()
{
    onOKClicked = nullptr;
    m_bUseTableView = true;
    m_pScrollView = nullptr;
    m_pTableView = nullptr;
    m_nMargin = -15;
}

ItemScrollView::~ItemScrollView()
{
    
}
ItemScrollView* ItemScrollView::createWithSize(cocos2d::Size size,bool useTableView)
{
    auto pRet = new ItemScrollView();
    if (pRet && pRet->init(size,useTableView))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool ItemScrollView::init(Size size,bool useTableView)
{
    if ( !Layer::init() )
    {
        return false;
    };
    decorationData.init();
    indexOffset = 0;
    lockFreeCount = 0;
    levelFreeCount  = 0;
    colNum = 1;
    m_pCurCell = nullptr;
    onItemCellSelected = nullptr;
    
    m_pBoard = Sprite::create("content/category/icon/dec_box_2.png");
    this->addChild(m_pBoard);
//    CMVisibleRect::setPositionAdapted(m_pBoard, m_pBoard->getContentSize().width*0.6, size.height/2);
    m_pBoard->setPosition(Vec2( size.width*0.5, size.height*0.5));
    
    m_bUseTableView = useTableView;
    if (m_bUseTableView) {
        m_pTableView = TableView::create(this, size);
        //    m_pTableView->setColor(Color3B(200, 200, 200));
        this->addChild(m_pTableView);
        m_pTableView->setVerticalFillOrder(cocos2d::extension::TableView::VerticalFillOrder::TOP_DOWN);
        m_pTableView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
        m_pTableView->setDelegate(this);
        m_pTableView->setSwallowsTouches(true);
    }else{
        m_pScrollView = ExpandScrollView::create();
        m_pScrollView->setContentSize(size);
        this->addChild(m_pScrollView);
        m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
        m_pScrollView->setScrollBarEnabled(false);
        m_nPercent = 0;
    }
    
    this->setContentSize(size);
    
//    Button* pOk = Button::create("content/category/icon/btn_ok.png");
//    this->addChild(pOk);
//    pOk->setName("ok");
//    pOk->setPosition(Vec2(size.width, size.height+30));
//    pOk->addTouchEventListener(CC_CALLBACK_2(ItemScrollView::_onButtonCallback, this));
//    pOk->runAction(RepeatForever::create(Sequence::create(EaseBackOut::create(ScaleTo::create(0.5, 1.1)),
//                                                          DelayTime::create(0.5),
//                                                          ScaleTo::create(0.5, 1), NULL)));
//    
//    Button* pUp = Button::create("content/category/icon/arrow_up.png");
//    this->addChild(pUp);
//    pUp->setPosition(Vec2(size.width/2, size.height+20));
//    
//    pUp->setName("up");
//    pUp->addTouchEventListener(CC_CALLBACK_2(ItemScrollView::_onButtonCallback, this));
//    
//    Button* pDown = Button::create("content/category/icon/arrow_down.png");
//    this->addChild(pDown);
//    pDown->setPosition(Vec2(size.width/2, -30));
//    pDown->setName("down");
//    pDown->addTouchEventListener(CC_CALLBACK_2(ItemScrollView::_onButtonCallback, this));
    
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
//    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch* pTouch, Event* event)
    {
        return true;
    };
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void ItemScrollView::reloadData()
{
    if(m_bUseTableView){
        m_pCurCell = nullptr;
        m_pTableView->reloadData();
        return;
    }
    int count =  (decorationData.totalCount+colNum)/colNum;
    m_pScrollView->getInnerContainer()->removeAllChildren();
    
    float margin = m_nMargin;
    Vec2 prePosition = Vec2::ZERO;
    Size preSize = Size::ZERO;
    for (int index = m_eDirection==Direction::eDirectionH?0:count-1; m_eDirection==Direction::eDirectionH?(index<count):(index>=0); m_eDirection==Direction::eDirectionH?(index++):(index--)) {
        for (int i = 0; i<colNum; i++) {
            if (index*colNum+i>=decorationData.totalCount) {
                break;
            }
            std::stringstream ostr;
            ostr<<btnPathEX<<index*colNum+i+decorationData.beginIndex<<".png";
            string lockName = "content/common/lock.png";
            if (index*colNum+i<decorationData.freeCount) {
                lockName="";
            }else if (index*colNum+i<decorationData.totalCount+decorationData.beginIndex && index+i>=decorationData.holidayIndex && decorationData.holidayIndex>=0 && decorationData.holidayCount==0){
                lockName="";
            }
            if (gNoneIap){
                lockName="";
            }
            ItemCell* item = ItemCell::create(bgPath, bgHighlightPath, std::string(ostr.str()),lockName);
            item->setTag(index*colNum+i+decorationData.beginIndex);
            m_pScrollView->getInnerContainer()->addChild(item);
            item->onItemCellSelected = CC_CALLBACK_1(ItemScrollView::_itemSelected, this);
            if (i%colNum==0) {
                if (m_eDirection==Direction::eDirectionH) {
                    item->setPosition(Vec2(prePosition.x+margin+preSize.width/2+item->getContentSize().width/2,getContentSize().height/2));
                }else{
                    item->setPosition(Vec2(this->getContentSize().width/2+1, prePosition.y+margin+preSize.height/2+item->getContentSize().height/2));
                }
            }else{
                if (m_eDirection==Direction::eDirectionH) {
                    item->setPosition(Vec2(prePosition.x, prePosition.x+margin+preSize.height/2+item->getContentSize().height/2));
                }else{
                    item->setPosition(Vec2(prePosition.x+margin+preSize.width/2+item->getContentSize().width/2, prePosition.y));
                }
            }
            prePosition = item->getPosition();
            preSize = item->getContentSize();
        }
        
    }
    if (m_pScrollView->getDirection()==ui::ScrollView::Direction::HORIZONTAL) {
        m_pScrollView->setInnerContainerSize(Size(prePosition.x+preSize.width,m_pScrollView->getContentSize().height));
        m_pBoard->setPosition(Vec2( getContentSize().width*0.5, getContentSize().height*0.5));
    }else{
        m_pScrollView->setInnerContainerSize(Size(m_pScrollView->getContentSize().width, prePosition.y+preSize.height/2+margin));
        m_pBoard->setPosition(Vec2( m_pBoard->getContentSize().width*0.5, getContentSize().height*0.5));
    }
}

TableViewCell* ItemScrollView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell* cell = table->dequeueCell();
    if(!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildrenWithCleanup(true);
    log("idx = %zd",idx);
    int index = idx*colNum;
//    if (idx>=decorationData.freeCount+levelFreeCount && idx<decorationData.freeCount+decorationData.levelCount) {
//        lockName = "Cocos/ui/studio_ui/pubilc/lmg_lock_level.png";
//    }else if (idx>=decorationData.freeCount+decorationData.levelCount+lockFreeCount){
//        lockName = "Cocos/ui/studio_ui/pubilc/img_lock_normal.png";
//    }
    for (int i = 0; i<colNum; i++) {
        if (index+i>=decorationData.totalCount) {
            break;
        }
        std::stringstream ostr;
        ostr<<btnPathEX<<index+i+decorationData.beginIndex<<".png";
        string lockName = "content/common/lock.png";
        if (index+i<decorationData.freeCount) {
            lockName="";
        }else if (index+i<decorationData.totalCount+decorationData.beginIndex && index+i>=decorationData.holidayIndex && decorationData.holidayIndex>=0 && decorationData.holidayCount==0){
            lockName="";            
        }
        if (gNoneIap){
            lockName="";
        }
        ItemCell* item = ItemCell::create(bgPath, bgHighlightPath, std::string(ostr.str()),lockName);
        item->setTag(index+i+decorationData.beginIndex);
        cell->addChild(item);
        item->onItemCellSelected = CC_CALLBACK_1(ItemScrollView::_itemSelected, this);
        item->setPosition(Vec2(85+155*i, 60));
    }
    return cell;
    
}

ssize_t ItemScrollView::numberOfCellsInTableView(TableView *table)
{
    return (decorationData.totalCount+colNum)/colNum;
}

void ItemScrollView::tableCellTouched(TableView* table, TableViewCell* cell)
{
    log("=====");
//    int index = cell->getTag();
//    if (cell==m_pCurCell) {
//        
//    }else{
//        if (m_pCurCell) {
//            ItemCell* item = (ItemCell*)m_pCurCell->getChildByTag(0);
//            if (item) {
//                item->setSelected(false);
//            }
//        }
//        m_pCurCell = cell;
//        ItemCell* item = (ItemCell*)m_pCurCell->getChildByTag(0);
//        if (item) {
//            item->setSelected(true);
//        }
//        if(this->onItemCellSelected){
//            this->onItemCellSelected(index);
//        }
//    }
}

void ItemScrollView::scrollToNext()
{
    if (m_bUseTableView) {
        if (m_pTableView->getDirection()==cocos2d::extension::ScrollView::Direction::HORIZONTAL) {
            Vec2 offset = m_pTableView->getContentOffset();
            if (offset.x-500>-m_pTableView->getContentSize().width) {
                m_pTableView->setContentOffset(Vec2(offset.x-500, 0),0.2);
            }else{
                m_pTableView->setContentOffset(Vec2(500-m_pTableView->getContentSize().width, 0),0.2);
            }
        }else{
            Vec2 offset =  m_pTableView->getContentOffset();
            Vec2 offset2 = Vec2(0, offset.y-m_pTableView->getViewSize().height);
            if (offset2.y<m_pTableView->getViewSize().height - m_pTableView->getContentSize().height) {
                offset2.y = m_pTableView->getViewSize().height - m_pTableView->getContentSize().height;
            }
            m_pTableView->setContentOffsetInDuration(offset2, 0.5);
        }
        Vec2 offset =  m_pTableView->getContentOffset();
        Vec2 offset2 = Vec2(0, offset.y+m_pTableView->getViewSize().height);
        if (offset2.y>50) {
            offset2.y = 0;
        }
        m_pTableView->setContentOffsetInDuration(offset2, 0.5);
    }else{
        int percent = getPercent();
        if (m_pScrollView->getDirection()== cocos2d::ui::ScrollView::Direction::HORIZONTAL){
//            往左滑动
            m_pScrollView->scrollToPercentHorizontal(percent, 1, true);
        }else{
//            往上滑动
            percent+= (100.0f*m_pScrollView->getContentSize().height/m_pScrollView->getInnerContainerSize().height);
            if (percent>100) {
                percent = 100;
            }
            
            m_pScrollView->scrollToPercentVertical(percent, 1, true);
        }
        m_nPercent = percent;
    }
}

void ItemScrollView::scrollToPre()
{
    if (m_bUseTableView){
        if (m_pTableView->getDirection()==cocos2d::extension::ScrollView::Direction::HORIZONTAL){
            Vec2 offset =  m_pTableView->getContentOffset();
            Vec2 offset2 = Vec2(0, offset.y+m_pTableView->getViewSize().height);
            if (offset2.y>50) {
                offset2.y = 0;
            }
            m_pTableView->setContentOffsetInDuration(offset2, 0.5);
        }else{
            Vec2 offset = m_pTableView->getContentOffset();
            if (offset.x+500<0) {
                m_pTableView->setContentOffset(Vec2(offset.x+500, 0),0.2);
            }else{
                m_pTableView->setContentOffset(Vec2::ZERO,0.2);
            }
        }
    }else{
        if (m_pScrollView->getDirection()== cocos2d::ui::ScrollView::Direction::HORIZONTAL){
//            往右滑动
            float percent = getPercent();
            percent-=10;
            if (percent<0) {
                percent = 0;
            }
            m_pScrollView->scrollToPercentHorizontal(percent, 1, true);
            m_nPercent = percent;
        }else{
//            往下滑动
            float percent = getPercent();
            percent-=(100.0f*m_pScrollView->getContentSize().height/m_pScrollView->getInnerContainerSize().height);
            if (percent<0) {
                percent = 0;
            }
            m_pScrollView->scrollToPercentVertical(percent, 1, true);
            m_nPercent = percent;
        }
    }
    
}

int ItemScrollView::getPercent()
{
    if (m_pScrollView->getDirection()== cocos2d::ui::ScrollView::Direction::HORIZONTAL) {
        float offsetX = -m_pScrollView->getInnerContainer()->getPositionX();
        m_nPercent = (offsetX+m_pScrollView->getContentSize().width)*100/m_pScrollView->getInnerContainerSize().width;
        return m_nPercent;
    }else{
        float offsetY = -m_pScrollView->getInnerContainer()->getPositionY();
        m_nPercent = 100.0f-100.0f*offsetY/(m_pScrollView->getInnerContainerSize().height-m_pScrollView->getContentSize().height);
        return m_nPercent;
    }
}

void ItemScrollView::_itemSelected(ItemCell* cell)
{
    if (m_bUseTableView) {
        if (m_pTableView->isTouchMoved()) {
            return;
        }
    }else{
        if (m_pScrollView->isScrolled()) {
            return;
        }
    }
    Rect rect = Rect::ZERO;
    Vec2 pointTop;
    Vec2 pointDown;
    if (m_bUseTableView) {
        rect = m_pTableView->boundingBox();
        rect.origin = m_pTableView->getParent()->convertToWorldSpace(rect.origin);
        pointTop = cell->getParent()->convertToWorldSpace(Vec2(cell->getContentSize().width*0.5, cell->getContentSize().height*0.5));
        pointDown = cell->getParent()->convertToWorldSpace(Vec2(cell->getContentSize().width*0.5, cell->getContentSize().height*0.5));
    }else{
        rect = m_pScrollView->boundingBox();
        rect.origin = m_pScrollView->getParent()->convertToWorldSpace(rect.origin);
        pointTop = cell->convertToWorldSpace(Vec2(cell->getContentSize().width*0.5, cell->getContentSize().height*0.3));
        pointDown = cell->convertToWorldSpace(Vec2(cell->getContentSize().width*0.5, -cell->getContentSize().height*0.3));
    }
    if (!rect.containsPoint(pointTop) && !rect.containsPoint(pointDown)) {
        log("touch no in scrollview");
        return;
    }
    if(onItemCellSelected){
        onItemCellSelected(cell->getTag(),DecorateConfigData::copy(decorationData));
    }
}

void ItemScrollView::_onButtonCallback(cocos2d::Ref *ref, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED) {
        Button* pBtn = (Button*)ref;
        pBtn->setEnabled(false);
        pBtn->runAction(Sequence::create(DelayTime::create(0.5),
                                         CallFunc::create([=]()
                                                          {
                                                              pBtn->setEnabled(true);
                                                          }), NULL));
        std::string name = pBtn->getName();
        if (strcmp(name.c_str(), "up")==0) {
            scrollToPre();
//            Vec2 offset =  m_pTableView->getContentOffset();
//            Vec2 offset2 = Vec2(0, offset.y+m_pTableView->getViewSize().height);
//            if (offset2.y>50) {
//                offset2.y = 0;
//            }
//            m_pTableView->setContentOffsetInDuration(offset2, 0.5);
        }else if (strcmp(name.c_str(), "down")==0) {
            scrollToNext();
//            Vec2 offset =  m_pTableView->getContentOffset();
//            Vec2 offset2 = Vec2(0, offset.y-m_pTableView->getViewSize().height);
//            if (offset2.y<m_pTableView->getViewSize().height - m_pTableView->getContentSize().height) {
//                offset2.y = m_pTableView->getViewSize().height - m_pTableView->getContentSize().height;
//            }
//            m_pTableView->setContentOffsetInDuration(offset2, 0.5);
            
        }else if (strcmp(name.c_str(), "ok")==0) {
            if (onOKClicked) {
                onOKClicked();
            }
            
        }
    }
}