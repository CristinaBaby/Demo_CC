//
//  EatIcePopScene.cpp
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/13.
//
//

#include "EatIcePopScene.h"
#include "IcePopEatLayer.h"

EatIcePopScene::EatIcePopScene()
{
    
}
EatIcePopScene::~EatIcePopScene()
{
    
}

bool EatIcePopScene::init()
{
    bool isInit=false;
    
    do{
        
        CC_BREAK_IF(!BaseScene::init());
        IcePopEatLayer* eatLayer = IcePopEatLayer::create();
        CC_BREAK_IF(!eatLayer);
        this->addChild(eatLayer, 0);
        
        isInit=true;
        
    }while(0);
    
    return isInit;
}