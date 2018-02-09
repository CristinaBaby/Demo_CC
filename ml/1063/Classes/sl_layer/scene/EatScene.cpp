//
//  EatScene.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-11-10.
//
//

#include "EatScene.h"
#include "SL_EatLayer.h"

EatScene::EatScene()
{
    
}
EatScene::~EatScene()
{
    
}

bool EatScene::init()
{
    bool isInit=false;
    
    do{
        
        CC_BREAK_IF(!BaseScene::init());
        SL_EatLayer* eatLayer = SL_EatLayer::create();
        CC_BREAK_IF(!eatLayer);
        this->addChild(eatLayer, 0);

        isInit=true;
        
    }while(0);
    
    return isInit;
}