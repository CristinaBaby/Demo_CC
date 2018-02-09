//
//  HomeScene.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-22.
//
//

#include "HomeScene.h"
HomeScene::HomeScene()
{
}
HomeScene::~HomeScene()
{
}
bool HomeScene::init()
{
    bool isInit=false;
    
    do{
        
        CC_BREAK_IF(!BaseScene::init());
        HomeLayer* m_pHomeLayer = HomeLayer::create();
        CC_BREAK_IF(!m_pHomeLayer);
        this->addChild(m_pHomeLayer);
        isInit=true;
        
    }while(0);
    
    return isInit;
}
