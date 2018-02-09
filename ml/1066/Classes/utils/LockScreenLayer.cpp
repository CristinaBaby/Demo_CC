//
//  LockScreenLayer.cpp
//  KidsMeal
//
//  Created by huxixiang on 1/4/15.
//
//

#include "LockScreenLayer.h"

LockScreenLayer::LockScreenLayer()
{

}

LockScreenLayer::~LockScreenLayer()
{

}

LockScreenLayer* LockScreenLayer::create(const Color4B& color)
{
    auto layer = new LockScreenLayer();
    if(layer && layer->initWithColor(color))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_RELEASE_NULL(layer);
    return nullptr;
}

bool LockScreenLayer::initWithColor(const Color4B& color)
{
    bool isInit = false;
    do{
        CC_BREAK_IF(!LayerColor::initWithColor(color));
        CC_BREAK_IF(!initLayer());
        isInit = true;
    }while(0);
    
    return isInit;
}

bool LockScreenLayer::initLayer()
{
    EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
    
    pListener->setSwallowTouches(true);
    
    pListener->onTouchBegan = [=](Touch* touch, Event* event){
        
        
        return this->isVisible();
    };
    pListener->onTouchMoved = [=](Touch* touch, Event* event){
        
        
    };
    pListener->onTouchEnded = [=](Touch* touch, Event* event){
        
        
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);
    
    return true;
}