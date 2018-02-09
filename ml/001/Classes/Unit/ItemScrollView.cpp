
#include "ItemScrollView.h"
#include "RewardManager.h"
#include "LockUIManager.h"

#include "SSCInternalLibManager.h"
ItemCell* ItemCell::create(const std::string bgPath,const std::string bgHighlight,const std::string frontHighlight,const std::string btn,const std::string lock,int lockType)
{
    
    auto pRet = new ItemCell();
    if (pRet && pRet->init(bgPath,bgHighlight,frontHighlight,btn,lock,lockType))
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
bool ItemCell::init(const std::string bgPath,const std::string bgHighlight,const std::string frontHighlight,const std::string btn,const std::string lock,int lockType)
{
    if (!Node::init() )
    {
        return false;
    };
    decType = 0;
    m_pBg = nullptr;
    onItemCellSelected = nullptr;
    m_pLock = nullptr;
    
    m_sBgPath = bgPath;
    m_sBgHighlightPath = bgHighlight;
    m_sFrontHighlightPath = frontHighlight;
    
    Size bgSize = Size::ZERO;
    if (std::strcmp(m_sBgPath.c_str(), "")) {
        m_pBg = Sprite::create(bgPath);
        if (m_pBg) {
            this->addChild(m_pBg);
            bgSize = m_pBg->getContentSize();
        }
    }
    if (std::strcmp(m_sBgHighlightPath.c_str(), "")) {
        m_pBgHeighlight = Sprite::create(m_sBgHighlightPath);
        if (m_pBgHeighlight) {
            this->addChild(m_pBgHeighlight);
            m_pBgHeighlight->setVisible(false);
        }
    }
    if (std::strcmp(m_sFrontHighlightPath.c_str(), "")) {
        m_pFrontHeighlight = Sprite::create(m_sFrontHighlightPath);
        if (m_pFrontHeighlight) {
            this->addChild(m_pFrontHeighlight,2);
            m_pFrontHeighlight->setVisible(false);
        }
    }
    m_pBtn = Button::create(btn,btn,btn);
    m_pBtn->setPressedActionEnabled(false);
    this->addChild(m_pBtn);
//    所有icon切图太大 缩放一些
    m_pBtn->setScale(0.6);
    
    if (std::strcmp(lock.c_str(), "")) {
        m_pLock = Sprite::create(lock);
        if (m_pLock) {
            this->addChild(m_pLock,5);
            m_pLock->setTag(lockType);
            m_pLock->setPosition(Vec2(30, -30));
        }
    }
    m_pBtn->setTouchEnabled(false);
    if (bgSize.width>m_pBtn->getContentSize().width) {
        this->setContentSize(bgSize);
    }else{
        this->setContentSize(m_pBtn->getContentSize());
    }
    this->setContentSize(bgSize);
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
        if (m_pFrontHeighlight) {
            m_pFrontHeighlight->setVisible(true);
        }
    }else {
//        m_pBtn->setTouchEnabled(true);
        
        if (m_pBg) {
            m_pBg->setVisible(true);
        }
        if (m_pBgHeighlight) {
            m_pBgHeighlight->setVisible(false);
        }
        if (m_pFrontHeighlight) {
            m_pFrontHeighlight->setVisible(false);
        }
        
    }
}



ItemScrollView::ItemScrollView()
{
    onOKClicked = nullptr;
    m_bUseTableView = true;
    m_pScrollView = nullptr;
    m_pTableView = nullptr;
    m_pPreCell = nullptr;
    m_pBoard = nullptr;
    m_nMargin = 0;
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

void ItemScrollView::insertOthers(DecorateConfigData decData,bool second,bool front)
{
    if(m_bUseTableView){
        m_pCurCell = nullptr;
        m_pTableView->reloadData();
        return;
    }
    int count =  (decData.totalCount+colNum)/colNum;
    
    float margin = m_nMargin;
    Vec2 prePosition = m_pScrollView->getInnerContainer()->getContentSize();
    prePosition.x -= m_nMargin;
    Size preSize = Size::ZERO;
    
    if (front) {
        prePosition = Vec2::ZERO;
    }
    bool networkAvailable = true;
    
    if (!SSCInternalLibManager::getInstance()->checkNetworkAvailable()) {
        networkAvailable = false;
    }
    insertDecData.push_back(decData);
    for (int index = m_eDirection==Direction::eDirectionH?0:count-1; m_eDirection==Direction::eDirectionH?(index<count):(index>=0); m_eDirection==Direction::eDirectionH?(index++):(index--)) {
        for (int i = 0; i<colNum; i++) {
            if (index*colNum+i>=decData.totalCount) {
                break;
            }
            std::stringstream ostr;
            ostr<<btnPathEX<<decData.decTypeName<<"/"<<index*colNum+i+decData.beginIndex<<".png";
            string lockName = "";
            int tag = index*colNum+i+decData.beginIndex;
            
            
            bool locked = false;
            if (!IAPManager::getInstance()->getItemIsBought(1)){
                if ((index*colNum+i)%2==second?1:0){
                    if (ConfigManager::getInstance()->getVideoUnLocked(decData.decTypeName, tag)){
                        lockName = "";
                    }else{
                        lockName = "content/common/video.png";
                        locked = true;
                    }
                }
            }
            
            ItemCell* item = ItemCell::create(bgPath, bgHighlightPath,frontHighlightPath, std::string(ostr.str()),lockName,(TAG_REWARD_LOCK));
            item->lockType = 0;
            if (locked) {
                item->lockType = 1;
                if (!ConfigManager::getInstance()->getVideoUnLocked(decData.decTypeName, tag)) {
                    LockUIManager::getInstance()->registerLock(decData.decTypeName, tag, item);
                    LockUIManager::getInstance()->setLockOriginPos(Vec2(30, -30));
                }
            }
            item->setTag(tag);
            item->decType = insertDecData.size()-1;
            m_pScrollView->getInnerContainer()->addChild(item);
            item->onItemCellSelected = CC_CALLBACK_1(ItemScrollView::_itemSelected, this);
            if (front) {
                m_ItemVector.insert(m_ItemVector.begin(), item);
            }else{
                m_ItemVector.push_back(item);
            }
            if (i%colNum==0) {
                if (m_eDirection==Direction::eDirectionH) {
                    if (front) {
                        if (i==0) {
                            item->setPosition(Vec2(prePosition.x+margin+preSize.width/2+item->getContentSize().width/2,getContentSize().height/2));
                        }else{
                            item->setPosition(Vec2(prePosition.x+margin+item->getContentSize().width/2,getContentSize().height/2));
                        }
                    }else{
                        item->setPosition(Vec2(prePosition.x+margin+preSize.width/2+item->getContentSize().width/2,getContentSize().height/2));
                    }
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
    _relayout();
}

void ItemScrollView::_relayout(){
    float margin = m_nMargin;
    Vec2 prePosition = Vec2::ZERO;
    Size preSize = Size::ZERO;
    for(int i = 0 ;i<m_ItemVector.size();i++){
        ItemCell* pItem = m_ItemVector.at(i);
        if (i%colNum==0) {
            if (m_eDirection==Direction::eDirectionH) {
                pItem->setPosition(Vec2(prePosition.x+margin+preSize.width/2+pItem->getContentSize().width/2,getContentSize().height/2));
            }else{
                pItem->setPosition(Vec2(this->getContentSize().width/2+1, prePosition.y+margin+preSize.height/2+pItem->getContentSize().height/2));
            }
        }else{
            if (m_eDirection==Direction::eDirectionH) {
                pItem->setPosition(Vec2(prePosition.x, prePosition.x+margin+preSize.height/2+pItem->getContentSize().height/2));
            }else{
                pItem->setPosition(Vec2(prePosition.x+margin+preSize.width/2+pItem->getContentSize().width/2, prePosition.y));
            }
        }
        prePosition = pItem->getPosition();
        preSize = pItem->getContentSize();

    }
    if (m_pScrollView->getDirection()==ui::ScrollView::Direction::HORIZONTAL) {
        m_pScrollView->setInnerContainerSize(Size(prePosition.x+preSize.width,m_pScrollView->getContentSize().height));
    }else{
        m_pScrollView->setInnerContainerSize(Size(m_pScrollView->getContentSize().width, prePosition.y+preSize.height/2+margin));
    }
}
void ItemScrollView::reloadData(bool second)
{
    m_pPreCell = nullptr;
    if(m_bUseTableView){
        m_pCurCell = nullptr;
        m_pTableView->reloadData();
        return;
    }
    insertDecData.clear();
    int count =  (decorationData.totalCount+colNum)/colNum;
    m_pScrollView->getInnerContainer()->removeAllChildren();
    m_ItemVector.clear();
    
    insertDecData.push_back(decorationData);
    if (!m_pBoard) {
        if (boxPathEX=="") {
            m_pBoard = Sprite::create();
        }else{
            m_pBoard = Sprite::create(boxPathEX);
        }
        this->addChild(m_pBoard,-1);
    }
//    float margin = m_nMargin;
//    Vec2 prePosition = Vec2::ZERO;
//    Size preSize = Size::ZERO;
    
    bool networkAvailable = true;
   
    if (!SSCInternalLibManager::getInstance()->checkNetworkAvailable()) {
        networkAvailable = false;
    }
    for (int index = m_eDirection==Direction::eDirectionH?0:count-1; m_eDirection==Direction::eDirectionH?(index<count):(index>=0); m_eDirection==Direction::eDirectionH?(index++):(index--)) {
        for (int i = 0; i<colNum; i++) {
            if (index*colNum+i>=decorationData.totalCount) {
                break;
            }
            std::stringstream ostr;
            ostr<<btnPathEX<<index*colNum+i+decorationData.beginIndex<<".png";
            string lockName = "";
//            if (index*colNum+i<decorationData.freeCount) {
//                lockName="";
//            }else if (index*colNum+i<decorationData.totalCount+decorationData.beginIndex && index+i>=decorationData.holidayIndex && decorationData.holidayIndex>=0 && decorationData.holidayCount==0){
//                lockName="";
//            }
//            if (gNoneIap){
//                lockName="";
//            }
            int tag = index*colNum+i+decorationData.beginIndex;
            
            
            bool locked = false;
            if (!IAPManager::getInstance()->getItemIsBought(1)){
                if ((index*colNum+i)%2==second?1:0){
                    if (ConfigManager::getInstance()->getVideoUnLocked(decorationData.decTypeName, tag)){
                        lockName = "";
                    }else{
                        lockName = "content/common/video.png";
                        locked = true;
                    }
                }
            }
            
            ItemCell* item = ItemCell::create(bgPath, bgHighlightPath,frontHighlightPath, std::string(ostr.str()),lockName,(TAG_REWARD_LOCK));
            item->lockType = 0;
            m_ItemVector.push_back(item);
            if (locked) {
                item->lockType = 1;
                if (!ConfigManager::getInstance()->getVideoUnLocked(decorationData.decTypeName, tag)) {
                    LockUIManager::getInstance()->registerLock(decorationData.decTypeName, tag, item);
//                    LockUIManager::getInstance()->setLockOriginPos(Vec2(30, -30));
                }
            }
            item->setTag(tag);
            item->decType = 0;
            m_pScrollView->getInnerContainer()->addChild(item);
            item->onItemCellSelected = CC_CALLBACK_1(ItemScrollView::_itemSelected, this);
//            if (i%colNum==0) {
//                if (m_eDirection==Direction::eDirectionH) {
//                    item->setPosition(Vec2(prePosition.x+margin+preSize.width/2+item->getContentSize().width/2,getContentSize().height/2));
//                }else{
//                    item->setPosition(Vec2(this->getContentSize().width/2+1, prePosition.y+margin+preSize.height/2+item->getContentSize().height/2));
//                }
//            }else{
//                if (m_eDirection==Direction::eDirectionH) {
//                    item->setPosition(Vec2(prePosition.x, prePosition.x+margin+preSize.height/2+item->getContentSize().height/2));
//                }else{
//                    item->setPosition(Vec2(prePosition.x+margin+preSize.width/2+item->getContentSize().width/2, prePosition.y));
//                }
//            }
//            prePosition = item->getPosition();
//            preSize = item->getContentSize();
        }
        
    }
    _relayout();
    if (m_pScrollView->getDirection()==ui::ScrollView::Direction::HORIZONTAL) {
//        m_pScrollView->setInnerContainerSize(Size(prePosition.x+preSize.width,m_pScrollView->getContentSize().height));
        m_pBoard->setPosition(Vec2( getContentSize().width*0.5, getContentSize().height*0.5));
        this->runAction(Sequence::create(DelayTime::create(0.1),
                                         CallFunc::create([=](){
            m_pScrollView->scrollToPercentHorizontal(20, 1, true);
        }), DelayTime::create(1),
                                         CallFunc::create([=](){
            m_pScrollView->scrollToLeft(0.1, false);
        }),NULL));
    }else{
//        m_pScrollView->setInnerContainerSize(Size(m_pScrollView->getContentSize().width, prePosition.y+preSize.height/2+margin));
        m_pBoard->setPosition(Vec2( m_pBoard->getContentSize().width*0.5, getContentSize().height*0.5));
        m_pScrollView->scrollToPercentVertical(50, 0.1, false);
        this->runAction(Sequence::create(DelayTime::create(0.2),
                                         CallFunc::create([=](){
            m_pScrollView->scrollToTop(0.5, true);
        }), NULL));
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
    
    bool networkAvailable = true;
   
    if (!SSCInternalLibManager::getInstance()->checkNetworkAvailable()) {
        networkAvailable = false;
    }
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
        int tag = index*colNum+i+decorationData.beginIndex;
        int type = 0;
        
        if (!networkAvailable) {
            decorationData.videoCount = 0;
        }
        if (decorationData.videoCount>0) {
            type = (index*colNum+i-decorationData.freeCount)%2;
            if (index*colNum+i>=decorationData.freeCount){
                if (index*colNum+i+1>decorationData.videoCount*2+decorationData.freeCount) {
                    type = 0;
                }
                if (type==1) {
                    if (ConfigManager::getInstance()->getVideoUnLocked(decorationData.decTypeName, tag)) {
                        lockName = "";
                    }else{
                        
                        lockName = "content/common/video.png";
                    }
                }
            }
        }
        ItemCell* item = ItemCell::create(bgPath, bgHighlightPath,frontHighlightPath, std::string(ostr.str()),lockName,type==0?TAG_IAP_LOCK:TAG_REWARD_LOCK);
        if (type==1 && decorationData.videoCount>0) {
            if (!ConfigManager::getInstance()->getVideoUnLocked(decorationData.decTypeName, tag)) {
                LockUIManager::getInstance()->registerLock(decorationData.decTypeName, tag, item);
                LockUIManager::getInstance()->setLockOriginPos(Vec2(30, -30));
            }
        }
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
    if (m_pPreCell) {
        m_pPreCell->setSelected(false);
    }
    cell->setSelected(true);
    m_pPreCell = cell;
    
    if(onItemCellSelected){
        onItemCellSelected(cell->getTag(),cell->lockType,DecorateConfigData::copy(insertDecData.at(cell->decType)));
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