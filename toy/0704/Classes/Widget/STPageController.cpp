//
//  STPageController.cpp
//  SpotDifference
//
//  Created by Steven.Xc.Tian.
//
//

//#include <type_traits>
#include "STPageController.h"

USING_NS_CC;
USING_NS_CC_EXT;

static const float  MOVE_DISTANCE_OFFSET  =  (1.f / 7.f);

STPageController * STPageController::create(TableViewDataSource* dataSource, Size size)
{
    return create(dataSource, size, nullptr);
}

STPageController* STPageController::create(TableViewDataSource* dataSource, cocos2d::Size size, Node *container)
{
    STPageController *table = new STPageController();
    
    if (table && table->init(dataSource, size, nullptr)) {
        table->autorelease();
        return table;
    }
    
    CC_SAFE_DELETE(table);

    return nullptr;
}


bool STPageController::init(TableViewDataSource* dataSource, Size size, Node *container)
{
    if ( !TableView::initWithViewSize(size, container) )
    {
        return false;
    }
    
    this->setDirection(Direction::HORIZONTAL);

    this->setDataSource(dataSource);
    this->_updateCellPositions();
    this->_updateContentSize();
    
    this->setDelegate(this);

    _selectPage = nullptr;
    
    return true;
}

void STPageController::setTouchMoved(bool moved)
{
    _touchMoved = moved;
}

void STPageController:: reloadData()
{
    TableView::reloadData();
    
    // turn to previous page
    unsigned int temp = _mFocusPageIndex;
    _mFocusPageIndex= 0;
    this->turnToPage(temp);
}


void STPageController::relocateContainer()
{
    // stop the bounce animation which set in super class CCScrollView
    this->unscheduleAllSelectors();
    
    
    Point oldPoint, min, max;
    float newX, newY;
    
    min = this->minContainerOffset();
    max = this->maxContainerOffset();
    
    
    oldPoint = this->getContentOffset();
    
    newX     = oldPoint.x;
    newY     = oldPoint.y;
    
    const float itemWidth = _dataSource->tableCellSizeForIndex(this, 0).width;
    const float tolerance = itemWidth * MOVE_DISTANCE_OFFSET;
    
    Point moveDelta = _mStartPoint - oldPoint;

    
    if (_direction == Direction::HORIZONTAL)
    {
        if (fabsf(moveDelta.x) < tolerance)
        {
            newX = _mStartPoint.x;
        }
        else
        {
            if (moveDelta.x < 0)
            {
                this->turnPage(TurnPageDirection::LEFT);
            }
            else if (moveDelta.x >0)
            {
                this->turnPage(TurnPageDirection::RIGHT);
            }
        }
        
        
        newX     = MAX(newX, min.x);
        newX     = MIN(newX, max.x);
    }
    
    if (_direction == Direction::VERTICAL)
    {
        newY     = MIN(newY, max.y);
        newY     = MAX(newY, min.y);
    }

    
    if (newY != oldPoint.y || newX != oldPoint.x)
    {
        this->setContentOffset(Point(newX, newY));
    }
}

bool STPageController:: onTouchBegan(Touch *pTouch, Event *pEvent)
{
    _touchMoved = false;
    _touches.clear();
    
    bool bRet = TableView::onTouchBegan(pTouch, pEvent);
    
    if (bRet)
    {
        _mStartPoint = this->getContentOffset();
    }
    
    return bRet;
}


void STPageController:: onTouchEnded(Touch *pTouch, Event *pEvent)
{
    TableView::onTouchEnded(pTouch, pEvent);
    
    // must call this after super class method
    relocateContainer();
    
    _mStartPoint = Point::ZERO;
    
    //MX
    if (_selectPage) {
        _selectPage->touchEnd(pTouch);
        _selectPage = nullptr;
    }
    
    if (_mpPageCell) {
        _mpPageCell->onClickedPage(pTouch);
        
        _mpPageCell = nullptr;
    } 
}

void STPageController:: onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    _mStartPoint = Point::ZERO;
}

void STPageController::backToOriginal()
{
    _mStartPoint = Point::ZERO;
    setContentOffset(Vec2(0, 0));
}


void STPageController:: tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCASSERT( (std::is_convertible<Page*, TableViewCell*>::value), "InValue Cell Type for STPageController!");
    _selectPage = dynamic_cast<Page *>(cell);
}

void STPageController:: tableCellHighlight(TableView* table, TableViewCell* cell)
{
    
}

void STPageController:: tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    
}

void STPageController:: tableCellWillRecycle(TableView* table, TableViewCell* cell)
{
    
}



void STPageController::turnPage(TurnPageDirection dire)
{
    switch (dire)
    {
        case TurnPageDirection::LEFT:
        {
            if (_mFocusPageIndex == 0)
                return;
            _mFocusPageIndex --;
        }
            break;
            
        case TurnPageDirection::RIGHT:
        {
            const ssize_t index = _dataSource->numberOfCellsInTableView(this);
            if (index == 0 || _mFocusPageIndex == (index - 1))
                return;
            _mFocusPageIndex ++;
        }
            break;
            
        default:
            break;
    }
    
    float delta = 0.f;
    const Size _cellSize = _dataSource->tableCellSizeForIndex(this, 0);
    
    if (_direction == Direction::HORIZONTAL)
    {
        
        delta = _cellSize.width * _mFocusPageIndex;
        this->setContentOffset(Point(-delta, 0), true);
    }
    else if (_direction == Direction::VERTICAL)
    {
        delta = _cellSize.height * _mFocusPageIndex;
        this->setContentOffset(Point(0, delta), true);
    }
    
    // inform indicator
    if (_mpIndicator)
    {
        _mpIndicator->onTurnPage(_mFocusPageIndex);
    }
    
    log("forcus index: %d", (int)_mFocusPageIndex);
    _pageControl->setSelectPage(_mFocusPageIndex);
}

void STPageController::setPageControl(PageControl* pageControl)
{
    _pageControl = pageControl;
}

void STPageController::turnToPage(unsigned pageIndex)
{
    if (pageIndex == _mFocusPageIndex || pageIndex >= (_dataSource->numberOfCellsInTableView(this)))
        return;
    
    _mFocusPageIndex = pageIndex;
    
    float delta = 0.f;
    const Size _cellSize = _dataSource->tableCellSizeForIndex(this, 0);
    
    if (_direction == Direction::HORIZONTAL)
    {
        delta = _cellSize.width * _mFocusPageIndex;
        this->setContentOffset(Point(-delta, 0), true);
    }
    else if (_direction == Direction::VERTICAL)
    {
        delta = _cellSize.height * _mFocusPageIndex;
        this->setContentOffset(Point(0, delta), true);
    }
    
    // inform indicator
    if (_mpIndicator)
    {
        _mpIndicator->onTurnPage(_mFocusPageIndex);
    }
}

void STPageController::turnToPage(unsigned pageIndex, bool animation)
{
    if (pageIndex == _mFocusPageIndex || pageIndex >= (_dataSource->numberOfCellsInTableView(this)))
        return;
    
    _mFocusPageIndex = pageIndex;
    
    float delta = 0.f;
    const Size _cellSize = _dataSource->tableCellSizeForIndex(this, 0);
    
    if (_direction == Direction::HORIZONTAL)
    {
        delta = _cellSize.width * _mFocusPageIndex;
        this->setContentOffset(Point(-delta, 0), animation);
    }
    else if (_direction == Direction::VERTICAL)
    {
        delta = _cellSize.height * _mFocusPageIndex;
        this->setContentOffset(Point(0, delta), animation);
    }
    
    // inform indicator
    if (_mpIndicator)
    {
        _mpIndicator->onTurnPage(_mFocusPageIndex);
    }
}


void STPageController:: setIndicator(STIndicator* indicator)
{
    if (indicator) {
        _mpIndicator = indicator;
        _mpIndicator->setName("STIndicator");
    }
}



// ========================================================================


static const float DEFAULT_MARGIN = 10;


STIndicator* STIndicator::create(const unsigned capacity, SpriteFrame *normalFrame, SpriteFrame *fucosFrame)
{
    STIndicator *pobSprite = new STIndicator();
    if (pobSprite && pobSprite->init(capacity, normalFrame, fucosFrame))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

STIndicator* STIndicator::create(const unsigned capacity, std::string normalFrameName, std::string fucosFrameName)
{
    STIndicator *pobSprite = new STIndicator();
    if (pobSprite && pobSprite->init(capacity, normalFrameName, fucosFrameName))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}



STIndicator::~STIndicator()
{
    CC_SAFE_RELEASE_NULL(mp_NormalFrame);
    CC_SAFE_RELEASE_NULL(mp_SelectedFrame);
    _mDots.clear();
}


bool STIndicator::init(const unsigned capacity, std::string normalFrameName, std::string fucosFrameName)
{
    return init(capacity,
                SpriteFrameCache::getInstance()->getSpriteFrameByName(normalFrameName),
                SpriteFrameCache::getInstance()->getSpriteFrameByName(fucosFrameName)
                );
}

bool STIndicator::init(const unsigned capacity, SpriteFrame *normalFrame, SpriteFrame *fucosFrame)
{
    bool bRet = false;
    
    do
    {
        
        mp_NormalFrame = normalFrame;
        mp_SelectedFrame = fucosFrame;
        
        CC_SAFE_RETAIN(mp_NormalFrame);
        CC_SAFE_RETAIN(mp_SelectedFrame);
        
        
        _mDots.clear();
        mPreFocusPosition = 0;
        
        float dotWith, dotHeight;
        
        for (int i = 0; i < capacity; i++)
        {
            Sprite *dot = nullptr;
            
            if (i == mPreFocusPosition)
                dot = Sprite::createWithSpriteFrame(mp_SelectedFrame);
            else
                dot = Sprite::createWithSpriteFrame(mp_NormalFrame);
            
            if (!dot)
                return bRet;
            
            dotWith = dot->getContentSize().width;
            dotHeight = dot->getContentSize().height;
            
            this->addChild(dot);
            _mDots.pushBack(dot);
            
            dot->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
            dot->setPosition( Point( ( i * (dotWith + DEFAULT_MARGIN) ), 0) );
        }
        
        const float layerWidth = dotWith + (dotWith + DEFAULT_MARGIN) * (capacity - 1);
        
        CC_BREAK_IF(! CCLayerColor::initWithColor(Color4B(0, 0, 0, 0), layerWidth, dotHeight));
        
        this->ignoreAnchorPointForPosition(false);

        
        bRet = true;
    } while (0);
    
    return bRet;
}

void STIndicator::onTurnPage(unsigned curPageIndex)
{
    if (curPageIndex == mPreFocusPosition)
        return;
    
    auto focus = _mDots.at(mPreFocusPosition);
    if (focus)
        focus->setDisplayFrame(mp_NormalFrame);
    
    mPreFocusPosition = curPageIndex;
    
    focus = _mDots.at(curPageIndex);
    if (focus)
        focus->setDisplayFrame(mp_SelectedFrame);
}





// ============================================================



STPage* STPage::create(const Size &size)
{
    STPage* pRet = new STPage();
    if (pRet && pRet->init(size))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

STPage * STPage::create(const Color4B& color, const Size &size)
{
    STPage * pLayer = new STPage();
    
    if(pLayer && pLayer->initWithColor(color, size))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

bool STPage:: init(const Size &size)
{
    return initWithColor(Color4B(0, 0, 0, 0), size);
}

bool STPage::initWithColor(const Color4B& color , const Size &size)
{
    bool bRet = false;
    do
    {
        this->setContentSize(size);
        
        _backgroundLayer = CCLayerColor::create(color, size.width, size.height);
        CC_BREAK_IF(!_backgroundLayer);
        this->addChild(_backgroundLayer, 0);
        
        bRet = true;
    } while (0);
    
    return bRet;
}









