
#include "LockItem.h"
LockItem::LockItem()
{
    m_pBgSrite = nullptr;
    m_pContentSrite = nullptr;
    m_pLock = nullptr;
    m_bLocked = false;
    onItemClicked = nullptr;
}

LockItem::~LockItem()
{
}
bool LockItem::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    m_bTouchEnable = true;
    
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(LockItem::ccTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LockItem::ccTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LockItem::ccTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(LockItem::ccTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void LockItem::initWithName(const std::string contentPath, const std::string lockPath,bool locked)
{
    if (std::strcmp(contentPath.c_str(), "")!=0){
        m_pContentSrite = Sprite::create(contentPath);
        this->addChild(m_pContentSrite);
        this->setContentSize(m_pContentSrite->getContentSize());
    }
    
    m_bLocked = locked;
    if (m_bLocked) {
        if (std::strcmp(lockPath.c_str(), "")!=0) {
            m_pLock = Sprite::create(lockPath);
            m_pLock->setPosition(m_pLock->getContentSize().width/2, -m_pLock->getContentSize().height/2);
            this->addChild(m_pLock,10);
        }
    }
    
}

void LockItem::unlock()
{
    if (m_bLocked) {
        m_bLocked =  false;
        if (m_pLock) {
            m_pLock->setVisible(false);
        }
    }
    
}


bool LockItem::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (!m_bTouchEnable) {
        return false;
    }
    if (m_pContentSrite) {
        Vec2 point = pTouch->getLocation();
        
        Rect rect = Rect(-_contentSize.width*m_pContentSrite->getAnchorPoint().x, -_contentSize.height*m_pContentSrite->getAnchorPoint().y, _contentSize.width, _contentSize.height);
        Rect boundBox = RectApplyAffineTransform(rect, getNodeToParentAffineTransform());
        boundBox.origin = this->getParent()->convertToWorldSpace(boundBox.origin);
        
        if (boundBox.containsPoint(point)) {
            return true;
        }
    }
    return false;
}
void LockItem::ccTouchMoved(Touch *pTouch, Event *pEvent)
{
    
}
void LockItem::ccTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (m_pContentSrite) {
        Vec2 point = pTouch->getLocation();
        
        Rect rect = Rect(-_contentSize.width*m_pContentSrite->getAnchorPoint().x, -_contentSize.height*m_pContentSrite->getAnchorPoint().y, _contentSize.width, _contentSize.height);
        Rect boundBox = RectApplyAffineTransform(rect, getNodeToParentAffineTransform());
        boundBox.origin = this->getParent()->convertToWorldSpace(boundBox.origin);
        
        if (boundBox.containsPoint(point)) {
            if (onItemClicked) {
                onItemClicked(this);
            }
        }
    }
}
void LockItem::ccTouchCancelled(Touch *pTouch, Event *pEvent)
{
    
}