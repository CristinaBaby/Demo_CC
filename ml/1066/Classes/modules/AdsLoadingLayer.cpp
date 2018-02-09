//
//  AdsLoadingLayer.cpp
//  BPLAY4401_HotAirBalloonMaker
//
//  Created by tangbowen on 15/7/3.
//
//

#include "AdsLoadingLayer.h"
#include "cocostudio/CocoStudio.h"
#include "AdapterScreen.h"
#include "IAPManager.h"
#include "AdsManager.h"

using namespace cocostudio;

AdsLoadingLayer::AdsLoadingLayer()
{
    
}

AdsLoadingLayer::~AdsLoadingLayer()
{
    ArmatureDataManager::getInstance()->removeArmatureFileInfo("images/loading/loading.ExportJson");
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
  
    Sprite* bgSprite = Sprite::create(IMAGE_BG_PATH);
    kAdapterScreen->setExactPosition(bgSprite, 480, 320);
    this->addChild(bgSprite);

    ArmatureDataManager::getInstance()->addArmatureFileInfo("images/loading/loading.ExportJson");
    
    
    Armature* m_pModel = Armature::create("loading");
    m_pModel->setScale(0.7f);
    m_pModel->ignoreAnchorPointForPosition(false);
    m_pModel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_pModel->getAnimation()->play("Animation1");
    kAdapterScreen->setExactPosition(m_pModel, 480, 320);
    this->addChild(m_pModel);
 
    return true;
}

void AdsLoadingLayer::onEnter()
{
    AdLoadingLayerBase::onEnter();
    if (kIAPManager->isShowAds())
        AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds, false);
}

void AdsLoadingLayer::onExit()
{
    AdLoadingLayerBase::onExit();
    
    if (kIAPManager->isShowAds())
        AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds, true);
}