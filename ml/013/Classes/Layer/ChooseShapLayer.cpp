//
//  ChooseShapLayer.cpp
//  Make
//
//  Created by QAMAC01 on 15-3-27.
//
//

#include "ChooseShapLayer.h"
#include "GameMaster.h"
#include "IAPManager.h"
#include "Config.h"
#include "KSVisibleRect.h"
#include "UIUnitScrollView.h"
#include "ShopSelView.h"

Scene* ChooseShapLayer::scene()
{
    Scene *pScene = Scene::create();
    pScene -> addChild(ChooseShapLayer::create());
    return pScene;
    
}


bool ChooseShapLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    this -> setBackground("makeIce/bg2.png");
    nextBtn_ -> setVisible(false);

    chooseView_ = ChooseShapeView::create(0);
    chooseView_ -> setPosition(KSVisibleRect::getPosition(0, 0));
    chooseView_ -> setDelegate(this);
    this -> addChild(chooseView_,3);

//    auto chooseView = UIUnitScrollView::create();
//    chooseView -> loadUnitListColumn("shape", 2);
//    this -> addChild(chooseView,3);

    
    return true;
}

void ChooseShapLayer::shopClicked()
{
    auto shop = dynamic_cast<ShopSelView*   >(GameMaster::getInstance() -> goToShopLayer(this));
    shop -> callback = CC_CALLBACK_0(ChooseShapLayer::checkunlock, this);
}


void ChooseShapLayer::unitSelected(std::string pUnitImageName ,int idx)
{
    
    GameMaster::getInstance() -> setShapeIdx(idx);
    
    nextBtn_ -> setVisible(true);
    
}

void ChooseShapLayer::checkunlock()
{
    chooseView_ -> reloadData();
    GameMaster::getInstance() -> showBannerAds();

}


void ChooseShapLayer::nextBtnClicked()
{
    BaseLayer::nextBtnClicked();
    
    GameMaster::getInstance() -> goToShaveIceLayer();
}



void ChooseShapLayer::onEnter()
{
    BaseLayer::onEnter();
    if(chooseView_)
        chooseView_ -> reloadData();
    
}

void ChooseShapLayer::onExit()
{
    BaseLayer::onExit();
    
}
