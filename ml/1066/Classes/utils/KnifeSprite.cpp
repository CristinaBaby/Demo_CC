//
//  KnifeSprite.cpp
//  LunchFoodMaker
//
//  Created by huxixiang on 15/9/23.
//
//

#include "KnifeSprite.h"
#include "AdapterScreen.h"


KnifeSprite::KnifeSprite():m_pDelegate(nullptr),m_bIsLock(true),m_bFlag(true),m_fLength(0),m_fRotateValue(0),m_fRateX(1.0f),m_fRateY(1.0f),m_fMinY(370),m_fMaxY(550),m_fResLength(0),_touchListener(nullptr)
{
    
}

KnifeSprite::~KnifeSprite()
{
    this->_eventDispatcher->removeEventListener(_touchListener);
    _touchListener->release();
}

KnifeSprite* KnifeSprite::create(const char* pImage)
{
    KnifeSprite* bRet = new KnifeSprite();
    
    if (bRet && bRet->init(pImage))
    {
        bRet->autorelease();
        return bRet;
    }
    CC_SAFE_RELEASE_NULL(bRet);
    return nullptr;
}

bool KnifeSprite::init(const char* pImage)
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!Sprite::initWithFile(pImage));
        
        _touchListener = EventListenerTouchOneByOne::create();
        _touchListener->retain();
        _touchListener->onTouchBegan = CC_CALLBACK_2(KnifeSprite::onKnifeTouchBegan, this);
        _touchListener->onTouchMoved = CC_CALLBACK_2(KnifeSprite::onKnifeTouchMove, this);
        _touchListener->onTouchEnded = CC_CALLBACK_2(KnifeSprite::onKnifeTouchEnd, this);
        _touchListener->onTouchCancelled = CC_CALLBACK_2(KnifeSprite::onKnifeTouchCancel, this);
        _touchListener->setSwallowTouches(false);
//        this->_eventDispatcher->addEventListenerWithSceneGraphPriority(e, this);
        this->_eventDispatcher->addEventListenerWithFixedPriority(_touchListener, -1);
        
        m_oOriginal = Vec2::ZERO;
        endVec = Vec2::ZERO;
        toLine = Vec2::ZERO;
        m_fRotateValue = this->getRotation();
        
        bRet = true;
    } while (0);
    return bRet;
}

bool KnifeSprite::onKnifeTouchBegan(Touch *touch , Event *unusedEvent)
{
    if (!m_bIsLock)
        return false;
    
    if (!m_bFlag)
        return false;
    
    Rect limit = Rect(0, 0, this->getContentSize().width, this->getContentSize().height);
    if(limit.containsPoint(this->convertToNodeSpace(touch->getLocation())))
    {
        if (m_pDelegate) {
            m_pDelegate->touchDown();
        }
        return true;
    }
    
    return false;
}

void KnifeSprite::onKnifeTouchMove(Touch *touch , Event *unusedEvent)
{
    if (!m_bFlag)
        return;
    
    auto p  = getPosition() + getShadow(touch->getDelta(), toLine);
    auto lineLen = (endVec - m_oOriginal).getLength();
    if((p - endVec).getLength() < lineLen)
        setPosition(p);
    
    if((p - m_oOriginal).getLength() >= lineLen * 2 && m_pDelegate)
    {
        if (m_bFlag) {
            m_bFlag = false;
            m_pDelegate->cut();
        }
    }
}

void KnifeSprite::onKnifeTouchEnd(Touch *touch , Event *unusedEvent)
{
    if (!m_bFlag)
    {
        m_bFlag = true;
        return;
    }
    
    if (m_pDelegate) {
        m_pDelegate->touchEnd();
    }
}
void KnifeSprite::onKnifeTouchCancel(Touch *touch , Event *unusedEvent)
{
    onKnifeTouchEnd(touch , unusedEvent);
}

void KnifeSprite::setDelegate(KnifeSpriteDelegate* delegate)
{
    m_pDelegate = delegate;
}

void KnifeSprite::setLength(float length)
{
    m_fLength = length;
}

void KnifeSprite::setOriginalPoint(Vec2 vec, bool isOffset)
{
    if (isOffset) {
        m_oOriginal = vec + Vec2(kAdapterScreen->g_oOffset.x, kAdapterScreen->g_oOffset.y);
    }else{
        m_oOriginal = vec;
    }
}

Vec2 KnifeSprite::getOriginalPoint()
{
    return m_oOriginal;
}

Vec2 KnifeSprite::getEndVec()
{
    return endVec;
}

void KnifeSprite::setEndVec(Vec2 vec, bool isOffset) {
    if (isOffset) {
        endVec = vec + Vec2(kAdapterScreen->g_oOffset.x, kAdapterScreen->g_oOffset.y);
    }else{
        endVec = vec;
    }
}
void KnifeSprite::setLine(Vec2 vec) {
    toLine = vec;
}

Vec2 KnifeSprite::getShadow(Vec2 from,Vec2 to){
    auto m = from.dot(to);
    return (m / to.getLengthSq()) * to;
}

void KnifeSprite::setSpriteEnabled(bool isEnabled)
{
    m_bIsLock = isEnabled;
}

void KnifeSprite::clearData()
{
    m_oOriginal = Vec2::ZERO;
    endVec = Vec2::ZERO;
    toLine = Vec2::ZERO;
}

void KnifeSprite::setSprtieRotate(float value)
{
    m_fRotateValue = value;
}

void KnifeSprite::setPosRate(float x, float y)
{
    m_fRateX = x;
    m_fRateY = y;
}

void KnifeSprite::setFlag(bool flag)
{
    m_bFlag = flag;
}

void KnifeSprite::setPosYMaxAndMin(float max, float min)
{
    m_fMaxY = max + kAdapterScreen->g_oOffset.y;
    m_fMinY = min + kAdapterScreen->g_oOffset.y;
}

void KnifeSprite::setResLength(float length)
{
    m_fResLength = length;
}

void KnifeSprite::onEnter()
{
    Sprite::onEnter();
    m_fRotateValue = this->getRotation();
    
//    m_fResLength = m_fLength;
}

void KnifeSprite::onExit()
{
    Sprite::onExit();
}