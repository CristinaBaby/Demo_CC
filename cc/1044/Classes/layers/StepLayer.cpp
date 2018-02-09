//
//  StepLayer.cpp
//  OreoMaker
//
//  Created by wangbosheng1 on 14-10-24.
//
//

#include "StepLayer.h"
StepLayer::StepLayer(GameLayer* gameLayer)
{
    BaseLayer();
    m_pGameLayer=gameLayer;
}

StepLayer::~StepLayer()
{
    
}

void StepLayer::onEnter(){
    BaseLayer::onEnter();
}

StepLayer* StepLayer::create(GameLayer* gameLayer)
{
    
    StepLayer *pRet = new StepLayer(gameLayer);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
        CC_SAFE_DELETE(pRet);
        return NULL;
}

bool StepLayer::init()
{
    bool isInit=false;
    
    do {
        CC_BREAK_IF(!BaseLayer::init());
        CC_BREAK_IF(!initUI());
        isInit=true;
    } while (0);
    
    return isInit;
}

bool StepLayer::initUI()
{
    return true;
}