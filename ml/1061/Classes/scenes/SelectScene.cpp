//
//  SelectScene.cpp
//  MLABS1043_BrownieMaker
//
//  Created by wusonglin1 on 15-1-5.
//
//

#include "SelectScene.h"
#include "SelectLayer.h"
SelectScene::SelectScene()
{
}
SelectScene::~SelectScene()
{
}
bool SelectScene::init()
{
    bool isInit=false;
    
    do{
        
        CC_BREAK_IF(!BaseScene::init());
        SelectLayer* selectLayer = SelectLayer::create();
        CC_BREAK_IF(!selectLayer);
        this->addChild(selectLayer, 0);
        isInit=true;
        
    }while(0);
    
    return isInit;
}