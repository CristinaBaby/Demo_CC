//
//  DecorationScene.cpp
//  MLABS1047_HotDogMaker
//
//  Created by wusonglin1 on 15/8/5.
//
//

#include "DecorationScene.h"
#include "SL_DecorationLayer.h"

DecorationScene::DecorationScene()
{
}
DecorationScene::~DecorationScene()
{
}
bool DecorationScene::init()
{
    bool isInit=false;
    
    do{
        
        CC_BREAK_IF(!BaseScene::init());
        SL_DecorationLayer* decorationLayer = SL_DecorationLayer::create();
        CC_BREAK_IF(!decorationLayer);
        this->addChild(decorationLayer, 0);
        isInit=true;
        
    }while(0);
    
    return isInit;
}