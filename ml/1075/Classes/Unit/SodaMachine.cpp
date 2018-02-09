
#include "SodaMachine.h"
SodaMachine::SodaMachine()
{
    m_pContentSrite = nullptr;
    m_pLock = nullptr;
    m_pFlavor = nullptr;
    m_bLocked = false;
    onItemClicked = nullptr;
    onOpen = nullptr;
    onClose = nullptr;
}

SodaMachine::~SodaMachine()
{
}
bool SodaMachine::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    m_pCoverBoad = Sprite::create("content/make/soda/machine0.png");
    this->addChild(m_pCoverBoad);
    m_pCoverBoad->setPosition(Vec2(106, 100));
    m_pCoverBoad->setLocalZOrder(1);
    
    m_pMaterialValve = Sprite::create("content/make/soda/machine1.png");
    this->addChild(m_pMaterialValve);
    m_pMaterialValve->setPosition(Vec2(106, 100));
    m_pMaterialValve->setLocalZOrder(10);
    
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(SodaMachine::TouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(SodaMachine::TouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(SodaMachine::TouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(SodaMachine::TouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void SodaMachine::initWithFlavor(int index, const std::string lockPath,bool locked)
{
    Node* stencil = Node::create();
    Sprite*pNode = Sprite::create("content/make/soda/machine_mask.png");
    stencil->addChild(pNode);
    
    std::stringstream ostr;
    ostr<<"content/make/soda/flavor"<<index<<".png";
    m_pWater = Sprite::create(ostr.str());
    
    m_pFlavor = ClippingNode::create(pNode);
    m_pFlavor->addChild(m_pWater);
    this->addChild(m_pFlavor);
    m_pFlavor->setPosition(Vec2(106, 100));
//    m_pFlavor->setInverted(true);
    m_pFlavor->setAlphaThreshold(0.5);
    
    ParticleSystemQuad*pParticle = ParticleSystemQuad::create("particle/soda.plist");
    m_pFlavor->addChild(pParticle);
    pParticle->setPosition(Vec2(0,-100));
    
    std::stringstream ostrNormal;
    ostrNormal<<"content/make/soda/flavor_button"<<index<<"_0.png";
    std::stringstream ostrHighlight;
    ostrHighlight<<"content/make/soda/flavor_button"<<index<<"_1.png";
    
    m_pSwitch = Button::create(ostrNormal.str(),ostrHighlight.str(),ostrNormal.str());
    this->addChild(m_pSwitch);
    m_pSwitch->setPosition(Vec2(106, 245));
//    m_pSwitch->setLocalZOrder(10);
    m_pSwitch->setEnabled(true);
    m_pSwitch->addTouchEventListener([=](Ref*,Widget::TouchEventType type){
        if (type==Widget::TouchEventType::BEGAN){
            rotateFlaver();
            if (onOpen) {
                onOpen(this);
            }
        }else if (type==Widget::TouchEventType::ENDED || type==Widget::TouchEventType::CANCELED){
            stopRotateFlavor();
            if (onClose) {
                onClose(this);
            }
        }
        
    });
    
    m_bLocked = locked;
    if (m_bLocked) {
        if (std::strcmp(lockPath.c_str(), "")!=0) {
            m_pLock = Sprite::create(lockPath);
            this->addChild(m_pLock);
            m_pLock->setLocalZOrder(10);
        }
//        m_pSwitch->setEnabled(false);
    }
}

void SodaMachine::initWithName(const std::string contentPath, const std::string lockPath,bool locked)
{
    if (std::strcmp(contentPath.c_str(), "")!=0){
        m_pContentSrite = Sprite::create(contentPath);
        this->addChild(m_pContentSrite);
        this->setContentSize(m_pContentSrite->getContentSize());
    }
    
    
}
Vec2 SodaMachine::getMaterialValveWorldPoint()
{
    return m_pMaterialValve->convertToWorldSpace(Vec2(110, 25));
}
void SodaMachine::unlock()
{
    if (m_bLocked) {
        m_bLocked =  false;
        if (m_pLock) {
            m_pLock->setVisible(false);
        }
        m_pSwitch->setEnabled(true);
    }
    
}

void SodaMachine::rotateFlaver()
{
    if (m_pWater) {
        m_pWater->runAction(RepeatForever::create(RotateBy::create(5, 360)));
    }
}

void SodaMachine::stopRotateFlavor()
{
    if (m_pWater) {
        m_pWater->stopAllActions();
    }
}

bool SodaMachine::TouchBegan(Touch *pTouch, Event *pEvent)
{
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
void SodaMachine::TouchMoved(Touch *pTouch, Event *pEvent)
{
    
}
void SodaMachine::TouchEnded(Touch *pTouch, Event *pEvent)
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
void SodaMachine::TouchCancelled(Touch *pTouch, Event *pEvent)
{
    
}