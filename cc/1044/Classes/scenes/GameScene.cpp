//
//  GameScene.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-22.
//
//

#include "GameScene.h"
#include "../layers/GameLayer.h"
GameScene::GameScene()
{
}
GameScene::~GameScene()
{
    log("GameScene()");
    log("gameLayer ------- %d",this->getChildByTag(100)->getReferenceCount());
    
}
bool GameScene::init()
{
    bool isInit=false;
    
    do{
       
        CC_BREAK_IF(!BaseScene::init());
        GameLayer* gameLayer = GameLayer::create();
        log("gameLayer ------- %d",gameLayer->getReferenceCount());
        gameLayer->setTag(100);
        CC_BREAK_IF(!gameLayer);
        this->addChild(gameLayer, 0);
        log("gameLayer ------- %d",gameLayer->getReferenceCount());
//        gameLayer->release();

        isInit=true;
        
    }while(0);
    
    return isInit;
}
