//
//  MyMySpriteButton.cpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 15/12/21.
//
//

#include "MySpriteButton.h"
#define kOffeset 5.f
#define GetRect(X) Rect(0, 0, X->getContentSize().width, X->getContentSize().height)

MySpriteButton::MySpriteButton()
:__enable(true),__pSelectorTouchEnd(nullptr),__pSelectorTouchBegan(nullptr)
{}
MySpriteButton::~MySpriteButton()
{}
MySpriteButton* MySpriteButton::create(const char* pImage , std::function<void(Ref*)> selector ,bool SwallowTouch , bool selectedStastus )
{
    auto pRet = new MySpriteButton();
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
MySpriteButton* MySpriteButton::createTexture(Texture2D* texture , std::function<void(Ref*)> selector ,bool SwallowTouch , bool selectedStastus)
{
    auto pRet = new MySpriteButton();
    if(pRet->initTexture(texture,selector,SwallowTouch,selectedStastus))
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
bool MySpriteButton::initTexture(Texture2D* texture , std::function<void(Ref*)> selector , bool SwallowTouch ,bool selectedStastus)
{
    if(Sprite::initWithTexture(texture))
    {
        __selected = selectedStastus;
        __touchListener = EventListenerTouchOneByOne::create();
        
        __touchListener->onTouchBegan = CC_CALLBACK_2(MySpriteButton::onBtnTouchBegan, this);
        __touchListener->onTouchMoved = CC_CALLBACK_2(MySpriteButton::onBtnTouchMove, this);
        __touchListener->onTouchEnded = CC_CALLBACK_2(MySpriteButton::onBtnTouchEnd, this);
        __touchListener->onTouchCancelled = CC_CALLBACK_2(MySpriteButton::onBtnTouchCancel, this);
        __touchListener->setSwallowTouches(SwallowTouch);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(__touchListener, this);
        
        __pSelectorTouchEnd = selector;
        
        return true;
    }
    return false;
    

}
bool MySpriteButton::init(const char* pImage , std::function<void(Ref*)> selector,bool SwallowTouch , bool selectedStastus)
{
    if(Sprite::initWithFile(pImage))
    {
        __selected = selectedStastus;
        __touchListener = EventListenerTouchOneByOne::create();
        
        __touchListener->onTouchBegan = CC_CALLBACK_2(MySpriteButton::onBtnTouchBegan, this);
        __touchListener->onTouchMoved = CC_CALLBACK_2(MySpriteButton::onBtnTouchMove, this);
        __touchListener->onTouchEnded = CC_CALLBACK_2(MySpriteButton::onBtnTouchEnd, this);
        __touchListener->onTouchCancelled = CC_CALLBACK_2(MySpriteButton::onBtnTouchCancel, this);
        __touchListener->setSwallowTouches(SwallowTouch);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(__touchListener, this);
        __pSelectorTouchEnd = selector;
        
        return true;
    }
    return false;
}
void MySpriteButton::setSelectorTouchBegan(std::function<void(Ref*)> selector)
{

    __pSelectorTouchBegan = selector;
}
bool MySpriteButton::onBtnTouchBegan(Touch *touch , Event *unusedEvent)
{
    if(!__enable)
        return false;
    if(GetRect(this).containsPoint(this->convertToNodeSpace(touch->getLocation())))
    {
        if(__pSelectorTouchBegan){
        
            __pSelectorTouchBegan(this);
            return true;
        }
        
        __startPos = touch->getLocation();

//        addGray();
        this->setColor(Color3B::GRAY);
        return true;
    }
    return false;
}
void MySpriteButton::onBtnTouchMove(Touch *touch , Event *unusedEvent)
{
    if(!GetRect(this).containsPoint(this->convertToNodeSpace(touch->getLocation())))
    {
//        this->setScale(1);
        this->setColor(Color3B::WHITE);
    }
}
void MySpriteButton::onBtnTouchEnd(Touch *touch , Event *unusedEvent)
{
//    this->setScale(1);
    this->setColor(Color3B::WHITE);

    if(GetRect(this).containsPoint(this->convertToNodeSpace(touch->getLocation())))
    {
        if (__pSelectorTouchEnd) {
            __pSelectorTouchEnd(this);
        }
        
    }
    
}
void MySpriteButton::onBtnTouchCancel(Touch *touch , Event *unusedEvent)
{
    onBtnTouchEnd(touch, unusedEvent);
}
void MySpriteButton::onExit()
{
    Sprite::onExit();
}
