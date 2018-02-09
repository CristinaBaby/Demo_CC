//
//  SpriteButton.cpp
//  FMAKER3042_Taco
//
//  Created by jianghai on 14-8-29.
//
//
#include "SpriteButton.h"
#define kOffeset 5.f
#define GetRect(X) Rect(0, 0, X->getContentSize().width, X->getContentSize().height)

SpriteButton::SpriteButton()
:__enable(true)
{}
SpriteButton::~SpriteButton()
{}
SpriteButton* SpriteButton::create(const char* pImage , std::function<void(Ref*)> selector ,bool SwallowTouch , bool selectedStastus )
{
    auto pRet = new SpriteButton();
    if(pRet->init(pImage,selector,SwallowTouch,selectedStastus))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}
bool SpriteButton::init(const char* pImage , std::function<void(Ref*)> selector,bool SwallowTouch , bool selectedStastus)
{
    if(Sprite::initWithFile(pImage))
    {
        __selected = selectedStastus;
        __touchListener = EventListenerTouchOneByOne::create();
        
        __touchListener->onTouchBegan = CC_CALLBACK_2(SpriteButton::onBtnTouchBegan, this);
        __touchListener->onTouchMoved = CC_CALLBACK_2(SpriteButton::onBtnTouchMove, this);
        __touchListener->onTouchEnded = CC_CALLBACK_2(SpriteButton::onBtnTouchEnd, this);
        __touchListener->onTouchCancelled = CC_CALLBACK_2(SpriteButton::onBtnTouchCancel, this);
        __touchListener->setSwallowTouches(SwallowTouch);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(__touchListener, this);
        __pSelector = selector;
        
        return true;
    }
    return false;
}

bool SpriteButton::onBtnTouchBegan(Touch *touch , Event *unusedEvent)
{
    if(!__enable)
        return false;
    if(GetRect(this).containsPoint(this->convertToNodeSpace(touch->getLocation())))
    {
        __startPos = touch->getLocation();
        if(__selected)
            this->setColor(Color3B::GRAY);
        else
            this->setScale(1.1);
        return true;
    }
    return false;
}
void SpriteButton::onBtnTouchMove(Touch *touch , Event *unusedEvent)
{
    if(!GetRect(this).containsPoint(this->convertToNodeSpace(touch->getLocation())))
    {
        this->setScale(1);
        this->setColor(Color3B::WHITE);
    }
}
void SpriteButton::onBtnTouchEnd(Touch *touch , Event *unusedEvent)
{
    this->setScale(1);
    this->setColor(Color3B::WHITE);
    if(GetRect(this).containsPoint(this->convertToNodeSpace(touch->getLocation())))
    {
        __pSelector(this);
    }
    
}
void SpriteButton::onBtnTouchCancel(Touch *touch , Event *unusedEvent)
{
    onBtnTouchEnd(touch, unusedEvent);
}
void SpriteButton::onExit()
{
    Sprite::onExit();
}