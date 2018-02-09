//
//  AdsLoadingLayer.cpp
//  Make
//
//  Created by liji on 15/7/14.
//
//

#include "AdsLoadingLayer.h"
#include "KSVisibleRect.h"
#include "AdsManager.h"
#include "GameMaster.h"
bool AdsLoadingLayer::init()
{
    if (!AdLoadingLayerBase::init())
    {
        return false;
    }

    _loadingMinTime = 3;
    
    auto bg = Sprite::create("loading/loading.jpg");
    bg -> setPosition(KSVisibleRect::getPosition(320, 480));
    this -> addChild(bg);
    
    cocostudio::ArmatureDataManager::getInstance() -> addArmatureFileInfo("loading/loading.ExportJson");
    cocostudio::Armature *armature = cocostudio::Armature::create("loading");
    armature -> setPosition(KSVisibleRect::getPosition(320, 280));
    armature->getAnimation()->playWithIndex(0);
    this->addChild(armature);
    
    
    return true;
}

void AdsLoadingLayer::onEnter()
{
    AdLoadingLayerBase::onEnter();
    AdsManager::getInstance()->setVisiable(kTypeBannerAds, false);

}


void AdsLoadingLayer::onExit()
{
    AdLoadingLayerBase::onExit();
    
//    if(!GameMaster::getInstance() -> getHidebanner())
//    {
//        GameMaster::getInstance()  -> showBannerAds();
//    }

    AdsManager::getInstance()->setVisiable(kTypeBannerAds, true);
}

