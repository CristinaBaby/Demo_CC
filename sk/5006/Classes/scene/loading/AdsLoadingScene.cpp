//
//  AdsLoadingScene.cpp
//  Make
//
//  Created by liji on 15/7/14.
//
//

#include "AdsLoadingScene.h"
#include "depends/component/screenadapter/CMVisibleRect.h"
#include "AdsManager.h"
#include "scene/loading/LoadingTransitionScene.h"


bool AdsLoadingScene::s_showAdsBanner = true;
bool AdsLoadingScene::s_enableBackClick = true;
bool AdsLoadingScene::s_showAdsBg = false;

bool AdsLoadingScene::init()
{
    if (!AdLoadingLayerBase::init())
    {
        return false;
    }
    _loadingMinTime = 0.1;
    _loadingMaxTime = 0.3;

    if (AdsLoadingScene::s_showAdsBg)
    {
        auto ld = LoadingTransitionScene::create();
        this->addChild(ld);
    }
    
    return true;
}

void AdsLoadingScene::onEnter()
{
    AdLoadingLayerBase::onEnter();
//    AdsManager::getInstance()->setVisiable(kTypeBannerAds, false);
    s_enableBackClick = false;

}


void AdsLoadingScene::onExit()
{
    AdLoadingLayerBase::onExit();
    
//    if(!GameMaster::getInstance()->getHidebanner())
//    {
//        GameMaster::getInstance() ->showBannerAds();
//    }

//    AdsManager::getInstance()->setVisiable(kTypeBannerAds, s_showAdsBanner);
    
    s_enableBackClick = true;
}

