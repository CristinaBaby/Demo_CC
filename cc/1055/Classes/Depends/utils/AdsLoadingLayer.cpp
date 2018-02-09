//
//  AdsLoadingLayer.cpp
//  BPLAY4401_HotAirBalloonMaker
//
//  Created by tangbowen on 15/7/3.
//
//

#include "AdsLoadingLayer.h"
#include "AdapterScreen.h"
#include "cocostudio/CocoStudio.h"
#include "./extensions/cocos-ext.h"


bool AdsLoadingLayer::m_bIsFrom = false;
AdsLoadingLayer::AdsLoadingLayer()
{
    
}

AdsLoadingLayer::~AdsLoadingLayer()
{
    
}

AdsLoadingLayer* AdsLoadingLayer::create()
{
    AdsLoadingLayer* pRet = new AdsLoadingLayer();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_RELEASE_NULL(pRet);
    return nullptr;
}

bool AdsLoadingLayer::init()
{
    if (!AdLoadingLayerBase::init())
    {
        return false;
    }
    
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    this->setAnchorPoint(Vec2::ZERO);
    this->setPosition(Vec2::ZERO);
    this->setContentSize(_visibleSize);

    Sprite* bgSprite = Sprite::create(IMAGE_BG_PATH);
    bgSprite->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height / 2));
    this->addChild(bgSprite);
    
    return true;
}

void AdsLoadingLayer::updatePos(float dt)
{
    Size _visibleSize = Director::getInstance()->getVisibleSize();
    for (int i = 0; i < 3; i++)
    {
        m_vecDots.at(i)->runAction(MoveBy::create(1.2f, Vec2(_visibleSize.width / 2, 0)));
        if (m_vecDots.at(i)->getPositionX() > _visibleSize.width * 1.3f)
        {
            m_vecDots.at(i)->stopAllActions();
            m_vecDots.at(i)->setPosition(Vec2(0, _visibleSize.height / 3 - 60));
            m_vecDots.at(i)->runAction(MoveBy::create(1.2f, Vec2(_visibleSize.width / 2, 0)));
        }
    }
}
void AdsLoadingLayer::onEnter()
{
    AdLoadingLayerBase::onEnter();
  AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds, false);
}

void AdsLoadingLayer::onExit()
{
    AdLoadingLayerBase::onExit();
   AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds, true);
}