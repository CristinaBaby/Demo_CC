//
//  EatSnowScene.cpp
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/13.
//
//

#include "EatSnowScene.h"
#include "SnowEatLayer.h"

EatSnowScene::EatSnowScene()
{
    
}
EatSnowScene::~EatSnowScene()
{
    
}

bool EatSnowScene::init()
{
    bool isInit=false;
    
    do{
        
        CC_BREAK_IF(!BaseScene::init());
        SnowEatLayer* eatLayer = SnowEatLayer::create();
        CC_BREAK_IF(!eatLayer);
        this->addChild(eatLayer, 0);
        
        isInit=true;
        
    }while(0);
    
    return isInit;
}