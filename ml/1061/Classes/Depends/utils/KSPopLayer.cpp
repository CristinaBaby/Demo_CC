//
//  KSPopLayer.cpp
//  CrazyCalculate
//
//  Created by tangbowen on 15-1-8.
//
//

#include "KSPopLayer.h"

KSPopLayer::KSPopLayer():m_sVisibleSize(Size::ZERO),m_bIsDialog(false)
{
    
}

KSPopLayer::~KSPopLayer()
{

}

KSPopLayer* KSPopLayer::create(cocos2d::Color4B color)
{
    KSPopLayer* pRet = new KSPopLayer();
    if (pRet && pRet->init(color)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE_NULL(pRet);
    return nullptr;
}

bool KSPopLayer::init(cocos2d::Color4B color)
{
    if (!LayerColor::initWithColor(color)) {
        return false;
    }
    m_sVisibleSize = Director::getInstance()->getVisibleSize();
    this->setContentSize(m_sVisibleSize);
    this->ignoreAnchorPointForPosition(false);
    
    EventListenerTouchOneByOne* e = EventListenerTouchOneByOne::create();
    e->setSwallowTouches(true);
    e->onTouchBegan = [this](Touch* touch, Event* event){
        if (!this->isAncestorsVisible(this)) {
            return false;
        }
        if (m_fnMoveCallBack) {
            m_fnMoveCallBack(this, TouchEventType::BEGAN, touch, event);
        }
        Vec2 touchPos = touch->getLocation();
        Vec2 realPos = this->getParent()->convertToNodeSpace(touchPos);
        if (boundingBox().containsPoint(realPos)) {
            if (m_fnCallBack) {
                m_fnCallBack(this, TouchEventType::BEGAN);
            }
            return true;
        }
        return false;
    };
    e->onTouchMoved = [this](Touch* touch, Event* event){
        if (m_fnMoveCallBack) {
            m_fnMoveCallBack(this, TouchEventType::MOVED, touch, event);
        }
        if (m_fnCallBack) {
            m_fnCallBack(this, TouchEventType::MOVED);
        }
    };
    e->onTouchEnded = [this](Touch* touch, Event* event){
        if (m_fnMoveCallBack) {
            m_fnMoveCallBack(this, TouchEventType::ENDED, touch, event);
        }
        if (m_fnCallBack) {
            m_fnCallBack(this, TouchEventType::ENDED);
        }
    };
    e->onTouchCancelled = [this](Touch* touch, Event* event){
        if (m_fnMoveCallBack) {
            m_fnMoveCallBack(this, TouchEventType::CANCELED, touch, event);
        }
        if (m_fnCallBack) {
            m_fnCallBack(this, TouchEventType::CANCELED);
        }
    };
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(e, this);
    
    return true;
}

bool KSPopLayer::isAncestorsVisible(Node* node)
{
    if (nullptr == node){
        return true;
    }
    Node* parent = node->getParent();
    if (!node->isVisible() || (parent && !parent->isVisible())){
        return false;
    }
    return this->isAncestorsVisible(parent);
}

void KSPopLayer::onEnter()
{
    LayerColor::onEnter();
    
}

void KSPopLayer::onExit()
{
    LayerColor::onExit();
}
void KSPopLayer::onEnterTransitionDidFinish()
{
    LayerColor::onEnterTransitionDidFinish();
    if(m_bIsDialog){
        auto func=CallFunc::create([&](){
            this->removeFromParent();
            if (m_fnCallBack) {
                m_fnCallBack(this, TouchEventType::REMOVE);
            }
        });
        
        this->runAction(Sequence::create(DelayTime::create(5.0),func, NULL));
    }
    
}