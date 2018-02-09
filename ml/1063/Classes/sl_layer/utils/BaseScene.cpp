//
//  BaseScene.cpp
//  OreoMaker
//
//  Created by wangbosheng1 on 14-10-23.
//
//

#include "BaseScene.h"

BaseScene::BaseScene()
{
}

BaseScene::~BaseScene()
{

}
bool BaseScene::init()
{
    bool isInit=false;
    
    do{
        CC_BREAK_IF(!Scene::init());
        isInit=true;
    }while(0);
    
    return isInit;
}
void BaseScene::onEnter()
{
    Scene::onEnter();
}
void BaseScene::onExit()
{
    Scene::onExit();
}