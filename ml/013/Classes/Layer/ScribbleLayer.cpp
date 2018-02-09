//
//  ScribbleLayer.cpp
//  Make
//
//  Created by QAMAC01 on 15-3-27.
//
//

#include "ScribbleLayer.h"
#include "GameMaster.h"
#include "IAPManager.h"
#include "Config.h"
#include "KSVisibleRect.h"


Scene* ScribbleLayer::scene()
{
    Scene *pScene = Scene::create();
    pScene -> addChild(ScribbleLayer::create());
    return pScene;
    
}


bool ScribbleLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    resetBtn_ -> setVisible(true);
    
    makeLayer_ = MakeLayer::create();
    makeLayer_ -> setPosition(KSVisibleRect::getPosition(0, 0));
    addChild(makeLayer_);
    makeLayer_ -> showDefaultDecorationUnit();
    
    chooseFlavorView_ = ChooseFlavorView::create(0);
    chooseFlavorView_ -> setPosition(KSVisibleRect::getPosition(0, 50,kBorderNone,kBorderTop));
    chooseFlavorView_ -> setDelegate(this);
    addChild(chooseFlavorView_,2);

    
    return true;
}


void ScribbleLayer::shopClicked()
{
    GameMaster::getInstance() -> goToShopLayer(this);
}

void ScribbleLayer::unitSelected(const char* pUnitImageName)
{
    makeLayer_ -> updateUnit(pUnitImageName);
    nextBtn_ -> setVisible(true);

}

void ScribbleLayer::nextBtnClicked()
{
    BaseLayer::nextBtnClicked();
    GameMaster::getInstance() -> setMakeNode(makeLayer_);
    GameMaster::getInstance() -> goToDecorationLayer();
}


void ScribbleLayer::resetBtnClicked()
{
    BaseLayer::resetBtnClicked();
    makeLayer_ -> resetScribble();
}



void ScribbleLayer::onEnter()
{
    BaseLayer::onEnter();
    if(makeLayer_)
        makeLayer_ -> backToDecLayer();
    if (chooseFlavorView_)
    {
        chooseFlavorView_ -> reloadData();
    }
    
}

void ScribbleLayer::onExit()
{
    BaseLayer::onExit();
    if(makeLayer_)
        makeLayer_ -> inDecLayer();

}
