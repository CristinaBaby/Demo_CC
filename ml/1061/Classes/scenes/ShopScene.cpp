//
//  ShopScene.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-24.
//
//

#include "ShopScene.h"
#include "ShopLayer.h"
ShopScene::ShopScene()
{
}
ShopScene::~ShopScene()
{
}
bool ShopScene::init()
{
    bool isInit=false;
    
    do{
        
        CC_BREAK_IF(!BaseScene::init());
        ShopLayer* shopLayer = ShopLayer::create();
        CC_BREAK_IF(!shopLayer);
        this->addChild(shopLayer, 0);
        isInit=true;
        
    }while(0);
    
    return isInit;
}