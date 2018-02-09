//
//  DrinkSprite.cpp
//  MyTest
//
//  Created by huxixiang on 15/11/17.
//
//

#include "DrinkSprite.hpp"
#include "SoundPlayer.h"

DrinkSprite::DrinkSprite():m_pDrink(nullptr),m_pStartBtn(nullptr),m_bIsStartDrink(false),m_iDrinkSoundId(-1),m_bIsSoundPlay(false)
{

}

DrinkSprite::~DrinkSprite()
{

}

DrinkSprite* DrinkSprite::create(const char* cupDownPath, const char* maskPath, const char* drinkPath, const char* cupUpPath, const char* tempPath)
{
    DrinkSprite* bRet = new DrinkSprite();
    
    if (bRet && bRet->init(cupDownPath, maskPath, drinkPath, cupUpPath, tempPath))
    {
        bRet->autorelease();
        return bRet;
    }
    
    CC_SAFE_RELEASE_NULL(bRet);
    
    return nullptr;
}

bool DrinkSprite::init(const char* cupDownPath, const char* maskPath, const char* drinkPath, const char* cupUpPath, const char* tempPath)
{
    bool isInit = false;
    do
    {
        CC_BREAK_IF(!Sprite::initWithFile(cupDownPath));
        
        CC_BREAK_IF(!initUI(cupDownPath, maskPath, drinkPath, cupUpPath, tempPath));
        
        isInit = true;
    } while (0);
    return isInit;
}

bool DrinkSprite::initUI(const char* cupDownPath, const char* maskPath, const char* drinkPath, const char* cupUpPath, const char* tempPath)
{
    ////////////////////////////////////////////////////////////////
    // add clipping node
    auto stencil = Sprite::create(maskPath);
    stencil->setPosition(this->getContentSize() / 2);
    
    auto pClipNode = ClippingNode::create();
    pClipNode->setContentSize(this->getContentSize());
    pClipNode->setStencil(stencil);
    pClipNode->setInverted(false);
    pClipNode->setAlphaThreshold(0);
    pClipNode->ignoreAnchorPointForPosition(false);
    pClipNode->setAnchorPoint(Point(0.5f, 0.5f));
    pClipNode->setPosition(this->getContentSize() / 2);
    this->addChild(pClipNode);
    
    // add drink
    m_pDrink = Sprite::create(drinkPath);
    m_pDrink->setPosition(this->getContentSize() / 2);
    pClipNode->addChild(m_pDrink);

    
    ////////////////////////////////////////////////////////////////
    // add cup: up
    auto pCup_up = Sprite::create(cupUpPath);
    pCup_up->setPosition(this->getContentSize() / 2);
    this->addChild(pCup_up, 100);
    
    ////////////////////////////////////////////////////////////////
    // add temp
    m_pStartBtn = Button::create(tempPath);
    m_pStartBtn->setAnchorPoint(Vec2(0.5f, 1));
    m_pStartBtn->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height + 20));
    m_pStartBtn->setContentSize(Size(this->getContentSize().width, 100));
    this->addChild(m_pStartBtn, INT16_MAX);
    m_pStartBtn->addTouchEventListener(CC_CALLBACK_2(DrinkSprite::onStartBtnCallback, this));
    
    return true;
}

void DrinkSprite::onStartBtnCallback(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::BEGAN)
    {
        if (!m_bIsStartDrink) {
            m_bIsStartDrink = true;
            this->drinkCallback();
            
            if (m_fnDrinkBeginCallback)
                m_fnDrinkBeginCallback();
        }else{
            if (!m_bIsSoundPlay) {
                m_iDrinkSoundId = SoundPlayer::getInstance()->playEffectLoop("drink.mp3");
                m_bIsSoundPlay = true;
            }
            Director::getInstance()->getActionManager()->resumeTarget(m_pDrink);
        }
    }
    
    if (type == Widget::TouchEventType::ENDED)
    {
        if (m_bIsSoundPlay) {
            SoundPlayer::getInstance()->stopEffectLoop(m_iDrinkSoundId);
            m_bIsSoundPlay = false;
        }
        Director::getInstance()->getActionManager()->pauseTarget(m_pDrink);
    }
    
    if (type == Widget::TouchEventType::CANCELED)
    {
        onStartBtnCallback(pSender, Widget::TouchEventType::ENDED);
    }
}

void DrinkSprite::drinkCallback()
{
    if (!m_bIsSoundPlay) {
        m_iDrinkSoundId = SoundPlayer::getInstance()->playEffectLoop("drink.mp3");
        m_bIsSoundPlay = true;
    }
    
    // run action
    auto pMove = MoveBy::create(m_pDrink->getContentSize().height / 50.0f, Vec2(0, -this->getContentSize().height + 40));
    m_pDrink->runAction(Sequence::create(pMove, CallFunc::create([=](){
        
        m_pStartBtn->setTouchEnabled(false);
        if (m_bIsSoundPlay) {
            SoundPlayer::getInstance()->stopEffectLoop(m_iDrinkSoundId);
            m_bIsSoundPlay = false;
        }
        
        /////////////////////////////
        // drink over
        if (m_fnDrinkEndCallback)
            m_fnDrinkEndCallback();
        
    }), nullptr));
}

void DrinkSprite::onEnter()
{
    Sprite::onEnter();
}

void DrinkSprite::onExit()
{
    Sprite::onExit();
    
    if (m_bIsSoundPlay) {
        SoundPlayer::getInstance()->stopEffectLoop(m_iDrinkSoundId);
        m_bIsSoundPlay = false;
    }
}