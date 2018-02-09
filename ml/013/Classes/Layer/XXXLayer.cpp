//
//  XXXLayer.cpp
//  Make
//
//  Created by QAMAC01 on 15-3-27.
//
//

#include "XXXLayer.h"
#include "GameMaster.h"
#include "Config.h"
#include "KSVisibleRect.h"

Scene* XXXLayer::scene()
{
    Scene *pScene = Scene::create();
    pScene -> addChild(XXXLayer::create());
    return pScene;
    
}


bool XXXLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    return true;
}

void XXXLayer::nextBtnClicked()
{
    BaseLayer::nextBtnClicked();
    
    GameMaster::getInstance() -> goToHomeLayer();
}


void XXXLayer::onEnter()
{
    BaseLayer::onEnter();
}

void XXXLayer::onExit()
{
    BaseLayer::onExit();

}
