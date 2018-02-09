//
// LoadingLayer.cpp
// ANIM3203
//
// Created by dengcheng on 15/10/10.
//
//

#include "LoadingLayer.h"
#include "XCVisibleRect.h"
#include "IAPManager.h"

LoadingLayer::LoadingLayer()
:m_vDots(std::vector<Sprite *>())
{
    
}

void LoadingLayer::onEnter()
{
    AdLoadingLayerBase::onEnter();
    AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
}

LoadingLayer* LoadingLayer::create()
{
    auto pRet = new LoadingLayer();
    if(pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    pRet = nullptr;
    return nullptr;
}

bool LoadingLayer::init()
{
    if ( !AdLoadingLayerBase::init() )
    {
        return false;
    }
    
    Sprite *pBg = Sprite::create("loading/loading_bg_portait.jpg");
    pBg->setPosition(XCVisibleRect::getPosition(320, 480));
    addChild(pBg, 1);
    
    Sprite *pDog = Sprite::create();
    pDog->setPosition(XCVisibleRect::getPosition(314, 547));
    addChild(pDog, 2);
    
    Sprite *pTitle = Sprite::create("loading/animalhouse.png");
    pTitle->setPosition(XCVisibleRect::getPosition(320, 393));
    addChild(pTitle, 3);
    
    Sprite *pLoading = Sprite::create("loading/loading.png");
    pLoading->setPosition(XCVisibleRect::getPosition(255, 316));
    addChild(pLoading, 4);
    
    static const float pos[] = {340, 357, 374};
    for (int i=0; i<3; ++i) {
        Sprite *pLoading = Sprite::create("loading/dot.png");
        pLoading->setPosition(XCVisibleRect::getPosition(pos[i], 306));
        addChild(pLoading, 5);
        pLoading->setVisible(false);
        m_vDots.push_back(pLoading);
    }
    
    auto pDogAnimation = Animation::create();
    for (int i=1; i <= 3; ++i) {
        std::string dogStr = StringUtils::format("loading/loading_dog_%d.png", i);
        pDogAnimation->addSpriteFrameWithFile(dogStr);
    }
    pDogAnimation->setDelayPerUnit(1.0 / 3);
    pDogAnimation->setRestoreOriginalFrame(true);
    Animate *pDogAnimate = Animate::create(pDogAnimation);
    pDog->runAction(RepeatForever::create(pDogAnimate));
    
    schedule(schedule_selector(LoadingLayer::onChangeDotStatus), 0.4);
    
    return true;
}

void LoadingLayer::onChangeDotStatus(float aTime)
{
    
    for (auto pDot : m_vDots) {
        if (!pDot->isVisible()) {
            pDot->setVisible(true);
            return;
        }
    }
    
    for (auto pDot : m_vDots) {
        pDot->setVisible(false);
    }
    
}

void LoadingLayer::onExit()
{
    if (Director::getInstance()->getRunningScene()->getName() != "HomeScene" && !IAPManager::getInstance()->isAdRemoved()) {
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
    }
    AdLoadingLayerBase::onExit();
}
LoadingLayer::~LoadingLayer()
{
    
}