//
//  ShareScene.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-24.
//
//

#include "ShareScene.h"

ShareScene::ShareScene()
{
    
}
ShareScene::~ShareScene()
{
    
}

bool ShareScene::init()
{
    bool isInit=false;
    
    do{
        
        CC_BREAK_IF(!BaseScene::init());
        isInit=true;
        
    }while(0);
    
    return isInit;
}