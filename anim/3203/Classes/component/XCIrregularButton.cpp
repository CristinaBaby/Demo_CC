


#include "XCIrregularButton.h"

XCIrregularButton::XCIrregularButton()
:m_bIstouch(false)
,m_pImage(nullptr)
,m_fCallback(nullptr)
,m_bClose(false)
,m_cMinAlpha(1)
{
    
}

XCIrregularButton *XCIrregularButton::create(std::string pName)
{
    XCIrregularButton *pButton = new XCIrregularButton();
    if (pButton && pButton->initWithXCIrregularButton(pName)) {
        pButton->autorelease();
        return pButton;
    }
    CC_SAFE_DELETE(pButton);
    return nullptr;
}

bool XCIrregularButton::initWithXCIrregularButton(std::string pName)
{
    if ( !Sprite::initWithFile(pName) ) {
        return false;
    }
    
    m_pImage = new Image();
    m_pImage->initWithImageFile(pName);
    
    EventListenerTouchOneByOne *pTouch = EventListenerTouchOneByOne::create();
    pTouch->onTouchBegan = std::bind(&XCIrregularButton::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
    pTouch->onTouchMoved = std::bind(&XCIrregularButton::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
    pTouch->onTouchEnded = std::bind(&XCIrregularButton::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
    pTouch->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(pTouch, this);
    
    return true;
}

void XCIrregularButton::setEventCallBack(std::function<void(Node *)> callBack)
{
    if (callBack == nullptr) {
        return;
    }
    m_fCallback = callBack;
}

void XCIrregularButton::closeTouch(bool close)
{
    m_bClose = close;
    _eventDispatcher->removeEventListenersForTarget(this);
}

void XCIrregularButton::setTouchAlphaValue(unsigned char aAlpha)
{
    
}

bool XCIrregularButton::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    
    if (m_bClose == true) {
        return false;
    }
    
    m_bIstouch = false;
    
    Vec2 lPointInPattern = convertToNodeSpace(pTouch->getLocation());
    
    if (getAlpha(lPointInPattern) > m_cMinAlpha) {
        m_bIstouch = true;
        setColor(Color3B::GRAY);
    } else {
        m_bIstouch = false;
    }
    
    return m_bIstouch;
}

void XCIrregularButton::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    Vec2 touchPoint = pTouch->getLocation();
    if (getBoundingBox().containsPoint(touchPoint)) {
        
        Vec2 lPointInPattern = convertToNodeSpace(pTouch->getLocation());
        
        if (getAlpha(lPointInPattern) < m_cMinAlpha)
        {
            setColor(Color3B::WHITE);
            m_bIstouch = false;
        } else if (getAlpha(lPointInPattern) > m_cMinAlpha) {
            setColor(Color3B::GRAY);
            m_bIstouch = true;
        }
        
    } else {
        setColor(Color3B::WHITE);
        m_bIstouch = false;
    }
}

void XCIrregularButton::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    if (m_bIstouch) {
        setColor(Color3B::WHITE);
        if (m_fCallback) {
            m_fCallback(this);
        }
    }
}

unsigned char XCIrregularButton::getAlpha(const cocos2d::Vec2 &aPos)
{
    unsigned char *data = m_pImage->getData();
    const int bytesPerPixel = m_pImage->getBitPerPixel() / 8;
    
    int row = m_pImage->getHeight() - aPos.y;
    int col = aPos.x;
    if (row < 0) {
        row = 0;
    }
    else if(row >= m_pImage->getHeight()){
        row = m_pImage->getHeight() - 1;
    }
    if (col < 0) {
        col = 0;
    }
    else if(col >= m_pImage->getWidth()){
        col = m_pImage->getWidth() - 1;
    }
    int index = row * m_pImage->getWidth() + col;
    index *= bytesPerPixel;
    return (3 == bytesPerPixel) ? 255 : data[index + 3];
}

XCIrregularButton::~XCIrregularButton()
{
    CC_SAFE_DELETE(m_pImage);
}