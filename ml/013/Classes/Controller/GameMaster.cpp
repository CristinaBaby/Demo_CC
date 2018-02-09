//
//  GameMaster.cpp
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-7-31.
//
//

#include "GameMaster.h"
//minigame

#include "LoadingPage.h"
#include "HomeLayer.h"
#include "MakeIceLayer.h"
#include "ChooseCupLayer.h"
#include "GetSnowLayer.h"
#include "ChooseShapLayer.h"
#include "ScribbleLayer.h"
#include "DecorationLayer.h"
#include "ShareLayer.h"
#include "ShopLayer.h"
#include "FavLayer.h"
#include "STSystemFunction.h"
#include "ShaveIceLayer.h"
#include "KSVisibleRect.h"
#include "AdsManager.h"
#include "IAPManager.h"
static GameMaster *instance = NULL;

GameMaster* GameMaster::getInstance()
{
    if (instance == NULL)
    {
        instance = new GameMaster();
    }
    return instance;
}

void GameMaster::init()
{
    cupName_ = __String::create("decoration/cups/cups0.png");
    shape_ = "shape0.png";
    cupName_ -> retain();
    hidebanner_ = false;
}

void GameMaster::goToScene(Scene *pScene)
{
    if (Director::getInstance() -> getRunningScene())
    {
        Director::getInstance() -> replaceScene(pScene);
    }
    else
        Director::getInstance() -> runWithScene(pScene);
}

void GameMaster::goToScribbleLayer()
{
    Scene *pScene = ScribbleLayer::scene();
    goToScene(pScene);
}

void GameMaster::goToDecorationLayer()
{
    Scene *pScene = DecorationLayer::scene();
    goToScene(pScene);
}

void GameMaster::goToShareLayer()
{
    log("makeNode_ -> getReferenceCount() %d",makeNode_ -> getReferenceCount());
    
    Scene *pScene = ShareLayer::scene();
    Director::getInstance() -> pushScene(pScene);
}
//
void GameMaster::goToLoadingPage()
{
    Scene *pScene = LoadingPage::scene();
    goToScene(pScene);
}
Node* GameMaster::goToShopLayer(Layer* layer)
{
//    Scene *pScene = ShopLayer::scene();
//    Director::getInstance() -> pushScene(pScene);
    
    auto shop = ShopSelView::create();
    shop -> setPosition(KSVisibleRect::getPosition(0, 0));
    layer -> addChild(shop,9999);
    
    return shop;
}
//
//
void GameMaster::goToFavLayer()
{
    Scene *pScene = FavLayer::scene();
    Director::getInstance() -> pushScene(pScene);
    
}
//




void GameMaster::goToHomeLayer()
{
    Scene *pScene = HomeLayer::scene();
    goToScene(pScene);
}


void GameMaster::goToMiniGameLayer()
{

}



void GameMaster::goToMakeIceLayer()
{

    Scene *pScene = MakeIceLayer::scene();
    goToScene(pScene);

}

void GameMaster::goToChooseCupLayer()
{
    
    Scene *pScene = ChooseCupLayer::scene();
    goToScene(pScene);
    
}


void GameMaster::goToGetSnowLayer()
{
    
    Scene *pScene = GetSnowLayer::scene();
    goToScene(pScene);
    
}


void GameMaster::goToChooseShapeLayer()
{

    Scene *pScene = ChooseShapLayer::scene();
    goToScene(pScene);

}

void GameMaster::goToShaveIceLayer()
{
    
    Scene *pScene = ShaveIceLayer::scene();
    goToScene(pScene);
    
}

//
//
void GameMaster::goToMoreGame()
{
    STSystemFunction *pFunction = new STSystemFunction();
    pFunction -> showMoreGame();

}
////
//void GameMaster::goToPrivacy()
//{
//    STSystemFunction *pFunction = new STSystemFunction();
//    pFunction -> goToPrivacy();
//    
//}
void GameMaster::showBannerAds()
{
    if (IAPManager::getInstance()->isShowAds())
    {
        AdsManager::getInstance()->showAds(kTypeBannerAds);
    }
}

