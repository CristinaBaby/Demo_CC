//
//  ComponentCycleScroll.cpp
//
//
//  Created by maxiang on 3/30/16.
//
//

#include "ComponentCycleScroll.h"

bool ComponentCycleScroll::_hasAddListener = false;
std::vector<ComponentCycleScroll*>* ComponentCycleScroll::_cycleScrollsVector = nullptr;

ComponentCycleScroll::ComponentCycleScroll():
_scrollView(nullptr),
_margin(150.0),
_isReady(false),
_isScrollMoving(false),
_eventListenerTouchOneByOne(nullptr),
_touchedItem(nullptr),
_isMovingItem(false),
_scrollStopMovingTime(0.0),
_moveScrollContainer(false),
_enableMoveItem(true),
_directionsFlag(DIRECTION_TOP),
_hasSetInnerPosition(false),
_fixedPriority(-1),
_scrollEnable(true),
_timeInterval(0.0),
_ignoreTouchMove(false),
_ignoreTouchOnce(false),
_ignoreAllTouch(false),
_scrollSoundEable(true),
_maxContainerLenght(1000000.0)
{
    Director::getInstance()->getScheduler()->schedule(schedule_selector(ComponentCycleScroll::timerUpdate), this, 1.0/60.0, false);
    
    if (!_cycleScrollsVector)
    {
        _cycleScrollsVector = new std::vector<ComponentCycleScroll*>();
    }
    _cycleScrollsVector->push_back(this);
    
    if (!_hasAddListener)
    {
        _hasAddListener = true;
        
        //这里的CustomListener优先级要比较高，防止被屏蔽
        {
            EventListenerCustom *listener = EventListenerCustom::create(CSIgnoreTouchOnceNotification, [=](EventCustom*){
                
                for (auto scroll : *_cycleScrollsVector)
                {
                    scroll->_ignoreTouchOnce = true;
                }
            });
            
            Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, -2);
        }
        
        {
            EventListenerCustom *listener = EventListenerCustom::create(CSIgnoreAllTouchNotification, [=](EventCustom*){
                
                for (auto scroll : *_cycleScrollsVector)
                {
                    scroll->_ignoreAllTouch = true;
                }
            });
            
            Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, -3);
        }
        
        {
            EventListenerCustom *listener = EventListenerCustom::create(CSCancelIgnoreAllTouchNotification, [=](EventCustom*){
                
                for (auto scroll : *_cycleScrollsVector)
                {
                    scroll->_ignoreAllTouch = false;
                }
            });
            
            Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, -4);
        }
        {
            EventListenerCustom *listener = EventListenerCustom::create(CSCancelIgnoreAllTouchNotification, [=](EventCustom*){
                
                for (auto scroll : *_cycleScrollsVector)
                {
                    scroll->_ignoreAllTouch = false;
                }
            });
            
            Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, -4);
        }
    }
}

ComponentCycleScroll::~ComponentCycleScroll()
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(ComponentCycleScroll::timerUpdate), this);
    CC_SAFE_RELEASE_NULL(_scrollView);
    
    removeTouchEventListener();
    
    for (auto link : _linksVector)
    {
        delete link;
    }
    _linksVector.clear();
    
    auto iterator = std::find(_cycleScrollsVector->begin(), _cycleScrollsVector->end(), this);
    if (iterator != _cycleScrollsVector->end())
    {
        _cycleScrollsVector->erase(iterator);
    }
}

bool ComponentCycleScroll::init()
{
    if (!ComponentBase::init())
        return false;
    
    _scrollView = ScrollView::create();
    _scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
    _scrollView->setBounceEnabled(true);
    _scrollView->setScrollBarEnabled(false);
    _scrollView->retain();
    
    _scrollView->addEventListener([=](Ref* ref, ScrollView::EventType type){
        
        //        if (type == ScrollView::EventType::SCROLLING)
        //        {
        //            if (_scrollSoundEable)
        //            {
        //                SoundManager::getInstance()->stopEffect("Gears.mp3");
        //                SoundManager::getInstance()->playEffect("Gears.mp3");
        //            }
        //        }
        if (type == ScrollView::EventType::CONTAINER_MOVED)
        {
            scrollMove();
            if (scrollMoveCallback)
                scrollMoveCallback();
        }
    });
    return true;
}

void ComponentCycleScroll::setScrollEnable(bool enable)
{
    if (!_scrollView)
        return;
    
    _scrollEnable = enable;
    _scrollView->setEnabled(enable);
}

void ComponentCycleScroll::setCycleScrollDirection(ScrollView::Direction dir)
{
    if (!_scrollView)
        return;
    
    CCASSERT(dir != ScrollView::Direction::NONE && dir != ScrollView::Direction::BOTH, "* ComponentCycleScroll: 不支持的方向! *");
    _scrollView->setDirection(dir);
    
    _isReady = false;
    setInnerContainerSize(_scrollView->getContentSize());
    resetLink();
}

void ComponentCycleScroll::setInnerContainerSize(const Size& size)
{
    if (!_scrollView)
        return;
    log("setInnerContainerSize===");
    
    if (_scrollView->getDirection() == ScrollView::Direction::HORIZONTAL)
        _scrollView->getInnerContainer()->setContentSize(Size(_maxContainerLenght, _scrollView->getContentSize().height));
    else
        _scrollView->getInnerContainer()->setContentSize(Size(_scrollView->getContentSize().width, _maxContainerLenght));
}

void ComponentCycleScroll::setCycleScrollSize(const Size& size)
{
    if (!_scrollView)
        return;
    
    _scrollView->setContentSize(size);
    _scrollView->setPosition(Vec2(-_scrollView->getContentSize().width/2, -_scrollView->getContentSize().height/2));
    
    setInnerContainerSize(size);
    resetLink();
}

void ComponentCycleScroll::setItemMoveDirection(uint32_t directions)
{
    if (_scrollView->getDirection() == ScrollView::Direction::HORIZONTAL)
    {
        CCASSERT((directions & DIRECTION_TOP) || (directions & DIRECTION_BOTTOM), "* ComponentCycleScroll: 不支持的拖动方向! *");
    }
    else if (_scrollView->getDirection() == ScrollView::Direction::VERTICAL)
    {
        CCASSERT((directions & DIRECTION_LEFT) || (directions & DIRECTION_RIGHT), "* ComponentCycleScroll: 不支持的拖动方向! *");
    }
    
    _directionsFlag = directions;
}

void ComponentCycleScroll::onAdd()
{
    ComponentBase::onAdd();
    
    _owner->addChild(_scrollView);
    addTouchEventListener();
}

void ComponentCycleScroll::start()
{
    ComponentBase::start();
    
    float itemTotalLenght = _margin/2.0 + _itemsVector.size() * _margin;
    float scrollVisibleLenght = _scrollView->getContentSize().width;
    if (_scrollView->getDirection() == ScrollView::Direction::VERTICAL)
        scrollVisibleLenght = _scrollView->getContentSize().height;
    
    CCASSERT(itemTotalLenght > scrollVisibleLenght + 50.0, "* ComponentCycleScroll: item太少或margin太小，无法占满一个scroll，无法循环滑动! *");
    
    _isReady = true;
    _hasSetInnerPosition = false;
}

void ComponentCycleScroll::stop()
{
    removeTouchEventListener();
    setScrollEnable(false);
    ComponentBase::stop();
}

void ComponentCycleScroll::scrollBy(const float duration,
                                    const Vec2& offset,
                                    const std::function<void()>& callback)
{
    if (!_owner)
        return;
    
    _moveScrollContainer = true;
    _scrollView->setTouchEnabled(false);
    _scrollView->getInnerContainer()->runAction(Sequence::create(MoveBy::create(duration, offset), CallFunc::create([this, callback](){
        
        _moveScrollContainer = false;
        _scrollView->setTouchEnabled(true);
        
        if (callback)
            callback();
    }), NULL));
}

void ComponentCycleScroll::timerUpdate(float time)
{
    _timeInterval += time;
    if (_timeInterval >= 0.3)
    {
        //每0.3秒检测一次，用于判断scroll是否在滑动
        _isScrollMoving = false;
        _timeInterval = 0.0;
    }
    
    if (_moveScrollContainer)
        scrollMove();
}

void ComponentCycleScroll::scrollMove()
{
    _isScrollMoving = true;
    
    if (!_isReady)
    {
        CCLOG("* ComponentCycleScroll: 请调用start()! *");
        return;
    }
    
    if (_scrollView->getDirection() == ScrollView::Direction::HORIZONTAL)
    {
        horizontalScrollMove();
    }
    else if (_scrollView->getDirection() == ScrollView::Direction::VERTICAL)
    {
        verticalScrollMove();
    }
}

void ComponentCycleScroll::horizontalScrollMove()
{
    static Vec2 previousPosition = Vec2(-_scrollView->getContentSize().width, 0.0);
    
    auto position = _scrollView->getInnerContainerPosition();
    auto offset = previousPosition - position;
    previousPosition = position;
    
    if (!_hasSetInnerPosition)
    {
        log("horizontalScrollMove===");

        _scrollView->getInnerContainer()->setPosition(Vec2(-_maxContainerLenght/2, 0.0));
        _hasSetInnerPosition = true;
        return;
    }
    
    //在scroll可视区域内的item
    std::vector<Link *> _visibleLink;
    for (int i = 0; i < _linksVector.size(); ++i)
    {
        auto link = _linksVector.at(i);
        auto item = link->item;
        
        auto position = _scrollView->convertToNodeSpace(item->convertToWorldSpaceAR(Vec2(item->getContentSize().width - item->getAnchorPointInPoints().x, 0.0)));
        
        if (position.x <= (_scrollView->getContentSize().width + item->getContentSize().width) &&
            position.x >= 0)
        {
            _visibleLink.push_back(link);
        }
    }
    
    if (_visibleLink.size() == 0)
        return;
    
    std::sort(_visibleLink.begin(), _visibleLink.end(), [](Link *a, Link *b) -> bool{
        
        return a->item->getPosition().x < b->item->getPosition().x;
    });
    
    if (offset.x >= 0)
    {
        //向左滑动
        auto _visibleItemStart = _visibleLink.at(0)->item;
        
        auto endLink = *(_visibleLink.end() - 1);
        auto next = endLink->next;
        Widget *item = endLink->item;
        
        while (next->item != _visibleItemStart) {
            next->item->setPosition(Vec2(item->getPosition().x + _margin, item->getPosition().y));
            item = next->item;
            next = next->next;
        }
    }
    else
    {
        //向右滑动
        auto _visibleItemEnd = (*(_visibleLink.end() - 1))->item;
        
        auto startLink = *_visibleLink.begin();
        auto previous = startLink->previous;
        Widget *item = startLink->item;
        
        while (previous->item != _visibleItemEnd) {
            previous->item->setPosition(Vec2(item->getPosition().x - _margin, item->getPosition().y));
            item = previous->item;
            previous = previous->previous;
        }
    }
}

void ComponentCycleScroll::verticalScrollMove()
{
    static Vec2 previousPosition = Vec2(-_scrollView->getContentSize().height, 0.0);
    
    auto position = _scrollView->getInnerContainerPosition();
    auto offset = previousPosition - position;
    previousPosition = position;
    
    if (!_hasSetInnerPosition)
    {
        _scrollView->getInnerContainer()->setPosition(Vec2(0.0, -_maxContainerLenght/2));
        _hasSetInnerPosition = true;
        return;
    }
    
    //在scroll可视区域内的item
    std::vector<Link *> _visibleLink;
    for (int i = 0; i < _linksVector.size(); ++i)
    {
        auto link = _linksVector.at(i);
        auto item = link->item;
        
        auto position = _scrollView->convertToNodeSpace(item->convertToWorldSpaceAR(Vec2(0.0, item->getContentSize().height - item->getAnchorPointInPoints().y)));
        
        if (position.y <= (_scrollView->getContentSize().height + item->getContentSize().height) &&
            position.y >= 0)
        {
            _visibleLink.push_back(link);
        }
    }
    
    if (_visibleLink.size() == 0)
        return;
    
    std::sort(_visibleLink.begin(), _visibleLink.end(), [](Link *a, Link *b) -> bool{
        
        return a->item->getPosition().y < b->item->getPosition().y;
    });
    
    if (offset.y >= 0)
    {
        //向上滑动
        auto _visibleItemStart = _visibleLink.at(0)->item;
        
        auto endLink = *(_visibleLink.end() - 1);
        auto next = endLink->next;
        Widget *item = endLink->item;
        
        while (next->item != _visibleItemStart) {
            next->item->setPosition(Vec2(item->getPosition().x, item->getPosition().y + _margin));
            item = next->item;
            next = next->next;
        }
    }
    else
    {
        //向右滑动
        auto _visibleItemEnd = (*(_visibleLink.end() - 1))->item;
        
        auto startLink = *_visibleLink.begin();
        auto previous = startLink->previous;
        Widget *item = startLink->item;
        
        while (previous->item != _visibleItemEnd) {
            previous->item->setPosition(Vec2(item->getPosition().x, item->getPosition().y - _margin));
            item = previous->item;
            previous = previous->previous;
        }
    }
}

void ComponentCycleScroll::pushCustomItem(Widget *item)
{
    CCASSERT(item, "* ComponentCycleScroll: item不能为空! *");
    
    _scrollView->addChild(item);
    _itemsVector.push_back(item);
    
    resetLink();
}

void ComponentCycleScroll::resetLink()
{
    for (auto link : _linksVector)
    {
        delete link;
    }
    _linksVector.clear();
    
    for (auto i = 0; i < _itemsVector.size(); ++i)
    {
        auto item = _itemsVector.at(i);
        
        if (_scrollView->getDirection() == ScrollView::Direction::HORIZONTAL)
        {
            log("resetLink===");
            item->setPosition(Vec2(_margin/2.0 + (i * _margin) + _maxContainerLenght/2, _scrollView->getContentSize().height/2));
        }
        else
            item->setPosition(Vec2(_scrollView->getContentSize().width/2, _margin/2.0 + (i * _margin) + _maxContainerLenght/2));
        
        Link *link = new Link;
        link->item = item;
        link->index = i+1;
        _linksVector.push_back(link);
    }
    
    for (auto i = 0; i < _linksVector.size(); ++i)
    {
        auto link = _linksVector.at(i);
        
        Link *nextItem = nullptr;
        Link *previousItem = nullptr;
        if (i == 0)
            previousItem = *(_linksVector.end() - 1);
        else
            previousItem = _linksVector.at(i - 1);
        
        if (i == _itemsVector.size() - 1)
            nextItem = *_linksVector.begin();
        else
            nextItem = _linksVector.at(i + 1);
        
        link->next = nextItem;
        link->previous = previousItem;
    }
}

void ComponentCycleScroll::addTouchEventListener()
{
    if (_eventListenerTouchOneByOne || !getOwner())
        return;
    
    _eventListenerTouchOneByOne = EventListenerTouchOneByOne::create();
    _eventListenerTouchOneByOne->setSwallowTouches(false);
    _eventListenerTouchOneByOne->onTouchBegan = CC_CALLBACK_2(ComponentCycleScroll::onTouchBegan, this);
    _eventListenerTouchOneByOne->onTouchMoved = CC_CALLBACK_2(ComponentCycleScroll::onTouchMoved, this);
    _eventListenerTouchOneByOne->onTouchEnded = CC_CALLBACK_2(ComponentCycleScroll::onTouchEnded, this);
    _eventListenerTouchOneByOne->onTouchCancelled = CC_CALLBACK_2(ComponentCycleScroll::onTouchCancelled, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_eventListenerTouchOneByOne, _fixedPriority);
}

void ComponentCycleScroll::removeTouchEventListener()
{
    if (_eventListenerTouchOneByOne)
    {
        Director::getInstance()->getEventDispatcher()->removeEventListener(_eventListenerTouchOneByOne);
        _eventListenerTouchOneByOne = nullptr;
    }
}

bool ComponentCycleScroll::onTouchBegan(Touch* touch, Event* event)
{
    log("onTouchBegan===%f, %f", _scrollView->getInnerContainerPosition().x,_scrollView->getInnerContainerPosition().y);

    _ignoreTouchMove = false;
    _ignoreTouchOnce = false;
    if (_isScrollMoving ||
        !_enabled ||
        !getOwner()->isVisible() ||
        _ignoreAllTouch)
        return false;
    
    for (auto item : _itemsVector)
    {
        Rect bounding;
        bounding.origin = Vec2(-item->getAnchorPointInPoints().x, -item->getAnchorPointInPoints().y);
        bounding.size = item->getContentSize();
        
        if (bounding.containsPoint(item->convertTouchToNodeSpaceAR(touch)))
        {
            _touchedItem = item;
            return true;
        }
    }
    
    return false;
}

void ComponentCycleScroll::onTouchMoved(Touch* touch, Event* event)
{
    if (!_touchedItem ||
        !_enableMoveItem ||
        _ignoreTouchMove ||
        _ignoreTouchOnce ||
        _ignoreAllTouch)
        return;

    if (!_isMovingItem)
    {
        auto offset = touch->getLocation() - touch->getStartLocation();
        
        auto moveFunc = [=]()
        {
            _scrollView->setTouchEnabled(false);
            
            _isMovingItem = true;
            if (itemWillMoveCallback)
            {
                bool handle = itemWillMoveCallback(_touchedItem, touch);
                if (!handle)
                    _ignoreTouchMove = true;
            }
        };
        
        //在没有拖动之前，如果在x轴上移动了10个像素，则识别为滑动scroll
        if (_scrollView->getDirection() == ScrollView::Direction::HORIZONTAL)
        {
            if (fabsf(offset.x) > 20.0)
            {
                _touchedItem = nullptr;
                return;
            }
            
            if (((_directionsFlag & DIRECTION_TOP) && offset.y > 6.0) ||
                ((_directionsFlag & DIRECTION_BOTTOM) && offset.y < -6.0))
            {
                moveFunc();
            }
        }
        else if (_scrollView->getDirection() == ScrollView::Direction::VERTICAL)
        {
            if (fabsf(offset.y) > 20.0)
            {
                _touchedItem = nullptr;
                return;
            }
            
            if (((_directionsFlag & DIRECTION_RIGHT) && offset.x > 6.0) ||
                ((_directionsFlag & DIRECTION_LEFT) && offset.x < -6.0))
            {
                moveFunc();
            }
        }
    }
    else
    {
        if (itemTouchMoveCallback)
            itemTouchMoveCallback(_touchedItem, touch);
    }
}

void ComponentCycleScroll::onTouchEnded(Touch* touch, Event* event)
{
    if (_ignoreTouchOnce || _ignoreAllTouch)
        return;
    
    if (_isMovingItem && !_ignoreTouchMove)
    {
        if (itemTouchEndCallback)
            itemTouchEndCallback(_touchedItem, touch);
        _innercontainerPos = _scrollView->getInnerContainerPosition();
    }
    else
    {
        if (_touchedItem)
        {
            auto offset = touch->getLocation() - touch->getStartLocation();
            
            //如果释放位置与起始位置之差小于20个像素，判断为点击
            if (fabsf(offset.x) < 20.0 &&
                fabsf(offset.y) < 20.0)
            {
                if (itemClickCallback)
                    itemClickCallback(_touchedItem, touch);
            }
        }
    }
    log("onTouchEnded===%f, %f", _scrollView->getInnerContainerPosition().x,_scrollView->getInnerContainerPosition().y);
    _scrollView->setTouchEnabled(true);
    _isMovingItem = false;
    _touchedItem = nullptr;
}

void ComponentCycleScroll::onTouchCancelled(Touch* touch, Event* event)
{
    if (_ignoreTouchOnce || _ignoreAllTouch)
        return;
    
    if (_isMovingItem)
    {
        if (itemTouchEndCallback)
            itemTouchEndCallback(_touchedItem, touch);
    }
    
    _scrollView->setTouchEnabled(true);
    _touchedItem = nullptr;
    _isMovingItem = false;
}


