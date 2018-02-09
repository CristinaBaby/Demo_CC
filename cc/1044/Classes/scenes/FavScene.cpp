//
//  FavScene.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-24.
//
//

#include "FavScene.h"
#include "../layers/FavLayer.h"
FavScene::FavScene()
{
}
FavScene::~FavScene()
{
}
bool FavScene::init()
{
    bool isInit=false;
    
    do{
        
        CC_BREAK_IF(!BaseScene::init());
        FavLayer* favLayer = FavLayer::create();
        CC_BREAK_IF(!favLayer);
        this->addChild(favLayer, 0);
        isInit=true;
        
    }while(0);
    
    return isInit;
}