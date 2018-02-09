//
// XCListButton.cpp
// SLUSHY3016
//
// Created by dengcheng on 15/7/23.
//
//

#include "XCListButton.h"

XCListButton::XCListButton()
:m_bClick(true)
,m_fTouch(nullptr)
,m_pNormal(nullptr)
,m_pSelected(nullptr)
,m_pDisable(nullptr)
,m_eStatus(STATE_DISABLE)
{
    
}

void XCListButton::onEnter()
{
    Sprite::onEnter();
    
}

XCListButton *XCListButton::create(const std::string &aNormal, const std::string &aSelected, const std::string &aDisable)
{
    XCListButton *pSWitch = new XCListButton();
    if (pSWitch && pSWitch->initWithXCListButton(aNormal, aSelected, aDisable)) {
        pSWitch->autorelease();
        return pSWitch;
    }
    CC_SAFE_DELETE(pSWitch);
    return nullptr;
}

bool XCListButton::initWithXCListButton(const std::string &aNormal, const std::string &aSelected, const std::string &aDisable)
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
    pTouch->onTouchBegan = CC_CALLBACK_2(XCListButton::onTouchBegan, this);
    pTouch->onTouchMoved = CC_CALLBACK_2(XCListButton::onTouchMoved, this);
    pTouch->onTouchEnded = CC_CALLBACK_2(XCListButton::onTouchEnded, this);
    pTouch->onTouchCancelled = CC_CALLBACK_2(XCListButton::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(pTouch, this);
    
    return true;
}

void XCListButton::changeStatus(ButtonStatus aStatus)
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

void XCListButton::updateImagesVisibility()
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

bool XCListButton::onTouchBegan(cocos2d::Touch *aTouch, cocos2d::Event *aEvent)
{
    
    m_bClick = true;
    
    if (STATE_DISABLE == m_eStatus) {
        return false;
    }
    
    if (getBoundingBox().containsPoint(_parent->convertToNodeSpace(aTouch->getLocation()))) {
        changeStatus(STATE_SELECTED);
        return true;
    }
    
    return false;
}

void XCListButton::onTouchMoved(cocos2d::Touch *aTouch, cocos2d::Event *aEvent)
{
    if (aTouch->getLocation().distance(aTouch->getStartLocation()) > 15) {
        m_bClick = false;
    }
}

void XCListButton::onTouchEnded(cocos2d::Touch *aTouch, cocos2d::Event *aEvent)
{
    
    changeStatus(STATE_NORMAL);
    
    if (m_bClick && m_fTouch) {
        m_fTouch(this);
    }
    
}

void XCListButton::onExit()
{

    Sprite::onExit();
}
XCListButton::~XCListButton()
{
    
}