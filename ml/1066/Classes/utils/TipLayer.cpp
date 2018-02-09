//
//  TipLayer.cpp
//  KCI1011
//
//  Created by liqiang on 15/10/20.
//
//

#include "TipLayer.h"

TipLayer* TipLayer::create(const Color4B& color)
{
    TipLayer* pRet = new TipLayer();
    if (pRet && pRet->initwithcolor(color)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE_NULL(pRet);
    return nullptr;
}

TipLayer::TipLayer():m_fTime(5.0f),m_fTouchbegan(nullptr),m_fschedule(nullptr),m_bIsReturn(false)
{
    
}

TipLayer::~TipLayer()
{
    
}

bool TipLayer::initwithcolor(const Color4B& color)
{
    if(!LayerColor::initWithColor(color))
    {
        return false;
    }

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(TipLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(TipLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(TipLayer::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(TipLayer::onTouchCancelled, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void TipLayer::Schedule(float f)
{
    stopSchedule();
    if (m_fschedule)
    {
        m_fschedule(this);
    }
}

void TipLayer::startSchedule()
{
     this->schedule(schedule_selector(TipLayer::Schedule), m_fTime);
}

void TipLayer::stopSchedule()
{
    this->unschedule(schedule_selector(TipLayer::Schedule));
}

bool TipLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
    if(m_fTouchbegan)
    {
        m_fTouchbegan(this);
    }
    stopSchedule();

    return true;
}

void TipLayer::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void TipLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
    if (m_bIsReturn)
        return;
    
    startSchedule();
}

void TipLayer::onTouchCancelled(Touch *touch, Event *unused_event)
{
    onTouchEnded(touch,unused_event);
}

void TipLayer::onEnter()
{
    LayerColor::onEnter();
    
    this->schedule(schedule_selector(TipLayer::Schedule), m_fTime);
}

void TipLayer::onExit()
{
     LayerColor::onExit();
}