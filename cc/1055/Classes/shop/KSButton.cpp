//
//  KSButton.cpp
//  GameFramework
//
//  Created by tangbowen on 14-12-5.
//
//

#include "KSButton.h"

KSButton::KSButton():m_pNormalSprite(nullptr),m_pSelectedSprite(nullptr),m_bEnable(true)
{
    
}

KSButton::~KSButton()
{
    
}

KSButton* KSButton::create(const string &normalImage, const string& selectedImage)
{
    Sprite* normal = Sprite::create(normalImage);
    Sprite* selected = nullptr;
    if (selectedImage != "") {
        selected = Sprite::create(selectedImage);
    }
    return create(normal, selected);
}

KSButton* KSButton::create(Sprite *normalSprite, Sprite* selectedSprite)
{
    if (!selectedSprite) {
        selectedSprite = Sprite::create();
        selectedSprite->setContentSize(normalSprite->getContentSize());
        selectedSprite->setSpriteFrame(normalSprite->getSpriteFrame());
        selectedSprite->setColor(Color3B(125, 125, 125));
    }
    
    KSButton* pRet = new KSButton();
    if (pRet && pRet->initWithSprite(normalSprite, selectedSprite)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE_NULL(pRet);
    return nullptr;
}

bool KSButton::initWithSprite(cocos2d::Sprite *normalSprite, cocos2d::Sprite *selectedSprite)
{
    bool bRet = Node::init();
    m_pNormalSprite = normalSprite;
    m_pSelectedSprite = selectedSprite;
    this->setContentSize(m_pNormalSprite->getContentSize());
    
    m_pNormalSprite->setPosition(Vec2::ZERO);
    m_pNormalSprite->setAnchorPoint(Vec2::ZERO);
    m_pSelectedSprite->setPosition(Vec2::ZERO);
    m_pSelectedSprite->setAnchorPoint(Vec2::ZERO);
    
    this->addChild(m_pSelectedSprite);
    this->addChild(m_pNormalSprite);
    
    // event
    EventListenerTouchOneByOne* e = EventListenerTouchOneByOne::create();
    e->setSwallowTouches(true);
    e->onTouchBegan = CC_CALLBACK_2(KSButton::onTouchBegan, this);
    e->onTouchMoved = CC_CALLBACK_2(KSButton::onTouchMoved, this);
    e->onTouchEnded = CC_CALLBACK_2(KSButton::onTouchEnded, this);
    e->onTouchCancelled = CC_CALLBACK_2(KSButton::onTouchCancelled, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(e, this);
    
    return bRet;
}

bool KSButton::isAncestorsVisible(Node* node)
{
    if (nullptr == node)
    {
        return true;
    }
    Node* parent = node->getParent();
    
    if (!node->isVisible() || (parent && !parent->isVisible()))
    {
        return false;
    }
    return this->isAncestorsVisible(parent);
}

bool KSButton::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (!this->isAncestorsVisible(this) || !m_bEnable) {
        return false;
    }
    Vec2 touchPos = touch->getLocation();
    Vec2 realPos = this->getParent()->convertToNodeSpace(touchPos);
    if (boundingBox().containsPoint(realPos)) {
        this->setButtonState(KSButton::ButtonState::StateSelected);
        this->callFunction(KSButton::TouchEventType::BEGAN);
        return true;
    }
    return false;
}

void KSButton::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    this->callFunction(KSButton::TouchEventType::MOVED);
    Vec2 touchPos = touch->getLocation();
    Vec2 realPos = this->getParent()->convertToNodeSpace(touchPos);
    if (boundingBox().containsPoint(realPos)) {
        this->setButtonState(KSButton::ButtonState::StateSelected);
    }else {
        this->setButtonState(KSButton::ButtonState::StateNormal);
    }
}

void KSButton::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    this->setButtonState(KSButton::ButtonState::StateNormal);
    Vec2 touchPos = touch->getLocation();
    Vec2 realPos = this->getParent()->convertToNodeSpace(touchPos);
    if (boundingBox().containsPoint(realPos)) {
        // call back
        if (m_fnClickCallBack) {
            m_fnClickCallBack(this);
        }
        this->callFunction(KSButton::TouchEventType::ENDED);
    }
}

void KSButton::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{
    this->callFunction(KSButton::TouchEventType::CANCELED);
    this->onTouchEnded(touch, event);
}

void KSButton::setButtonState(KSButton::ButtonState state)
{
    if (state == ButtonState::StateNormal) {
        m_pNormalSprite->setVisible(true);
        m_pSelectedSprite->setVisible(false);
    }else{
        m_pSelectedSprite->setVisible(true);
        m_pNormalSprite->setVisible(false);
    }
}

void KSButton::callFunction(KSButton::TouchEventType type)
{
    if (m_pCallBack) {
        m_pCallBack(this, type);
    }
}

void KSButton::onEnter()
{
    Node::onEnter();
}

void KSButton::onExit()
{
    Node::onExit();
}