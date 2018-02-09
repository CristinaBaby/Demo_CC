//
//  ChooseCupLayer.cpp
//  Make
//
//  Created by QAMAC01 on 15-3-27.
//
//

#include "ChooseCupLayer.h"
#include "GameMaster.h"
#include "IAPManager.h"
#include "Config.h"
#include "KSVisibleRect.h"
#include "ShopSelView.h"
Scene* ChooseCupLayer::scene()
{
    Scene *pScene = Scene::create();
    pScene -> addChild(ChooseCupLayer::create());
    return pScene;
    
}

bool ChooseCupLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    this ->  setBackground("makeIce/bg2.png");
    
    _unitView = UIUnitScrollView::createWithTypeName("cups");
    _unitView -> setAnchorPoint(Vec2(0.5, 1));
    _unitView -> setPosition(KSVisibleRect::getPosition(0, 150+90,kBorderNone,kBorderTop));
    _unitView -> freeUnitClicked = CC_CALLBACK_1(ChooseCupLayer::freeUnitClicked, this);
    _unitView -> lockedUnitClicked = CC_CALLBACK_0(ChooseCupLayer::lockedUnitClicked, this);
    this -> addChild(_unitView);
    
    cup_ = Sprite::create("decoration/cups/cups0.png");
    cup_ -> setPosition(KSVisibleRect::getPosition(320, 420-20));
    this -> addChild(cup_);
    
    nextBtn_ -> setVisible(true);
    
    return true;
}

void ChooseCupLayer::nextBtnClicked()
{
    BaseLayer::nextBtnClicked();
    
    GameMaster::getInstance() -> goToChooseShapeLayer();
}

void ChooseCupLayer::lockedUnitClicked()
{
//    GameMaster::getInstance() -> goToShopLayer(this);
    
    auto shop = ShopSelView::create();
    shop -> setPosition(KSVisibleRect::getPosition(0, 0));
    this -> addChild(shop,99);
    shop -> callback = CC_CALLBACK_0(ChooseCupLayer::checkUnlock, this);
}

void ChooseCupLayer::freeUnitClicked(std::string pIconImageName)
{
    cup_ -> setRotation(0);
    cup_ -> runAction(RotateBy::create(0.5, 360));
    cup_ -> setTexture(pIconImageName);
    GameMaster::getInstance() -> setCupName(__String::create(pIconImageName));
    CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/select.mp3");
    
    auto p = ParticleSystemQuad::create("stars.plist");
    p -> setPosition(KSVisibleRect::getPosition(320, 480));
    this -> addChild(p,10);
}

void ChooseCupLayer::checkUnlock()
{
    _unitView -> loadUnitList("cups");
    GameMaster::getInstance() -> showBannerAds();
}


void ChooseCupLayer::onEnter()
{
    BaseLayer::onEnter();

}

void ChooseCupLayer::onExit()
{
    BaseLayer::onExit();
    
}
