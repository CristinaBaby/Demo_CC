


#include "SwitchButton.h"

SwitchButton::SwitchButton()
:m_pNormal(nullptr)
,m_pSelected(nullptr)
,m_pDisable(nullptr)
,m_fTouchCallback(nullptr)
,m_fStatusChange(nullptr)
,m_eStatus(STATE_DISABLE)
{
    
}

SwitchButton *SwitchButton::create(const std::string &aNormal, const std::string &aSelected, const std::string &aDisable)
{
    SwitchButton *pSWitch = new SwitchButton();
    if (pSWitch && pSWitch->initWithSwitchButton(aNormal, aSelected, aDisable)) {
        pSWitch->autorelease();
        return pSWitch;
    }
    CC_SAFE_DELETE(pSWitch);
    return nullptr;
}

bool SwitchButton::initWithSwitchButton(const std::string &aNormal, const std::string &aSelected, const std::string &aDisable)
{
    if ( !Sprite::init() )
    {
        return false;
    }
    
    if (aNormal.empty() || aSelected.empty()) {
        return false;
    }
    
    if (!aDisable.empty()) {
        m_pDisable = Sprite::create(aDisable);
        addChild(m_pDisable);
    }
    
    m_pNormal = Sprite::create(aNormal);
    addChild(m_pNormal);
    
    m_pSelected = Sprite::create(aSelected);
    addChild(m_pSelected);
    
    changeStatus(STATE_NORMAL);
    
    updateImagesVisibility();
    
    auto pTouch = EventListenerTouchOneByOne::create();
    pTouch->setSwallowTouches(true);
    pTouch->onTouchBegan = CC_CALLBACK_2(SwitchButton::onTouchBegan, this);
    pTouch->onTouchMoved = CC_CALLBACK_2(SwitchButton::onTouchMoved, this);
    pTouch->onTouchEnded = CC_CALLBACK_2(SwitchButton::onTouchEnded, this);
    pTouch->onTouchCancelled = CC_CALLBACK_2(SwitchButton::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(pTouch, this);
    
    return true;
}

void SwitchButton::changeStatus(ButtonStatus aStatus)
{
    if (aStatus == m_eStatus) {
        return;
    }
    
    switch (aStatus) {
        case STATE_NORMAL:
        {
            m_eStatus = STATE_NORMAL;
            setContentSize(m_pNormal->getContentSize());
            m_pNormal->setPosition(getContentSize() / 2);
        }
            break;
        case STATE_SELECTED:
        {
            m_eStatus = STATE_SELECTED;
            setContentSize(m_pSelected->getContentSize());
            m_pSelected->setPosition(getContentSize() / 2);
        }
            break;
        case STATE_DISABLE:
        {
            m_eStatus = STATE_DISABLE;
            if (m_pDisable) {
                setContentSize(m_pDisable->getContentSize());
                m_pDisable->setPosition(getContentSize() / 2);
            }
        }
            break;
        default:
            log("SwitchButton: Unknow type!");
            break;
    }
    updateImagesVisibility();
}

void SwitchButton::updateImagesVisibility()
{
    switch (m_eStatus) {
        case STATE_NORMAL:
            if (m_pDisable) {
                m_pDisable->setVisible(false);
            }
            m_pNormal->setVisible(true);
            m_pSelected->setVisible(false);
            break;
        case STATE_SELECTED:
            if (m_pDisable) {
                m_pDisable->setVisible(false);
            }
            m_pNormal->setVisible(false);
            m_pSelected->setVisible(true);
            break;
        case STATE_DISABLE:
            if (m_pDisable) {
                m_pDisable->setVisible(true);
                m_pNormal->setVisible(false);
                m_pSelected->setVisible(false);
            } else {
                m_pDisable->setVisible(false);
                m_pNormal->setVisible(true);
                m_pSelected->setVisible(false);
            }
            break;
        default:
            break;
    }
}

bool SwitchButton::onTouchBegan(cocos2d::Touch *aTouch, cocos2d::Event *aEvent)
{
    
    if (STATE_DISABLE == m_eStatus) {
        return false;
    }
    
    if (getBoundingBox().containsPoint(_parent->convertToNodeSpace(aTouch->getLocation()))) {
        return true;
    }
    
    return false;
}

void SwitchButton::onTouchMoved(cocos2d::Touch *aTouch, cocos2d::Event *aEvent){}

void SwitchButton::onTouchEnded(cocos2d::Touch *aTouch, cocos2d::Event *aEvent)
{
    if (getBoundingBox().containsPoint(_parent->convertToNodeSpace(aTouch->getLocation()))) {
        if (m_fTouchCallback) {
            m_fTouchCallback(this);
        }
        
        if (m_fStatusChange) {
            if (m_eStatus == STATE_NORMAL) {
                changeStatus(STATE_SELECTED);
            } else {
                changeStatus(STATE_NORMAL);
            }
            m_fStatusChange(m_eStatus);
        } else {
            changeStatus(STATE_SELECTED);
        }
    }
}

SwitchButton::~SwitchButton()
{
    
}