//
//  STUILayer.cpp
//  SpotDifference
//
//  Created by Steven.Xc.Tian.
//
//

#include "STUILayer.h"
#include "../AppConfig.h"

USING_NS_CC;

STUILayer* STUILayer::create()
{
    STUILayer* pRet = new STUILayer();
    if (pRet && pRet->initData(Color4B(0,0,0,0)))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}


STUILayer * STUILayer::createWithColor(const Color4B& color)
{
    STUILayer * pLayer = new STUILayer();
    
    if(pLayer && pLayer->initData(color))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}


bool STUILayer:: initData(const Color4B& color)
{
    if ( ! LayerColor::initWithColor(color) ) {
        return false;
    }
    
    this->setScale(UI_SCALE);
    
    // this game use FIXED_WIDTH mode, so we adjust the y-position
    Point newP = this->getPosition() - Point(0, this->getBoundingBox().origin.y);
    this->setPosition(newP);
    
    
    return true;
}


