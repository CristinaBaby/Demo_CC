
#include "IAPItem.h"

IAPItem::IAPItem()
{
    m_pContentSrite = nullptr;
    m_pBuy = nullptr;
    onItemClicked = nullptr;
}

IAPItem::~IAPItem()
{
}
bool IAPItem::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(IAPItem::ccTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(IAPItem::ccTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(IAPItem::ccTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(IAPItem::ccTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void IAPItem::initWithName(const std::string contentPath, const std::string buyPath)
{
    if (std::strcmp(contentPath.c_str(), "")!=0){
        m_pContentSrite = Sprite::create(contentPath);
        this->addChild(m_pContentSrite);
        
    }
    
    if (std::strcmp(buyPath.c_str(), "")!=0) {
        m_pBuy = Sprite::create(buyPath);
        m_pBuy->setPosition(Vec2(0,-100));
        this->addChild(m_pBuy);
        m_pBuy->setLocalZOrder(10);
    }
}

bool IAPItem::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
    Vec2 point = pTouch->getLocation();
    Rect rect = visibleWorldRect;
    if (!rect.equals(Rect::ZERO)) {
        if (!rect.containsPoint(point)) {
            return false;
        }
    }
    return true;
}
void IAPItem::ccTouchMoved(Touch *pTouch, Event *pEvent)
{
    
}
void IAPItem::ccTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (m_pContentSrite) {
        Vec2 point = pTouch->getLocation();
        point = this->convertToNodeSpace(point);
        Rect rect = m_pContentSrite->getBoundingBox();
        log("===%f,%f",point.x,point.y);
        //        rect.origin = m_pContentSrite->getParent()->convertToWorldSpace(rect.origin);
        log("===%f,%f,%f,%f",rect.origin.x,rect.origin.y,rect.size.width,rect.size.height);
        if (rect.containsPoint(point)) {
            if (onItemClicked) {
                onItemClicked(this->getTag());
            }
        }
    }
}
void IAPItem::ccTouchCancelled(Touch *pTouch, Event *pEvent)
{
    
}
