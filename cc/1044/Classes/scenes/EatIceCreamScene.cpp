//
//  EatIceCreamScene.cpp
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/13.
//
//

#include "EatIceCreamScene.h"
#include "IceCreamEatLayer.h"

EatIceCreamScene::EatIceCreamScene()
{
    
}
EatIceCreamScene::~EatIceCreamScene()
{
    
}

bool EatIceCreamScene::init()
{
    bool isInit=false;
    
    do{
        
        CC_BREAK_IF(!BaseScene::init());
        IceCreamEatLayer* eatLayer = IceCreamEatLayer::create();
        CC_BREAK_IF(!eatLayer);
        this->addChild(eatLayer, 0);
        
        isInit=true;
        
    }while(0);
    
    return isInit;
}