//
//  MakeIceLayer.cpp
//  Make
//
//  Created by QAMAC01 on 15-3-27.
//
//

#include "MakeIceLayer.h"
#include "GameMaster.h"
#include "IAPManager.h"
#include "Config.h"
#include "KSVisibleRect.h"
#include "MachineView.h"
Scene* MakeIceLayer::scene()
{
    Scene *pScene = Scene::create();
    pScene -> addChild(MakeIceLayer::create());
    return pScene;
    
}


bool MakeIceLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    
    this ->  setBackground("makeIce/default_bg.png");
    
    MachineView* machine = MachineView::create();
    machine -> setPosition(KSVisibleRect::getPosition(0, 0));
    this -> addChild(machine);
    machine -> setDelegate(this);
    
    return true;
}



void MakeIceLayer::allStepFinished()
{
    nextBtn_ -> setVisible(true);
}

void MakeIceLayer::nextBtnClicked()
{
    BaseLayer::nextBtnClicked();
    
    GameMaster::getInstance() -> goToGetSnowLayer();
}


void MakeIceLayer::onEnter()
{
    BaseLayer::onEnter();
}

void MakeIceLayer::onExit()
{
    BaseLayer::onExit();
    
}
