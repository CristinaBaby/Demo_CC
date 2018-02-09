//
//  SceneManager.cpp
//  X_BrainTest_zlwd
//
//  Created by studio on 14-1-18.
//
//

#include "SceneManager.h"
#include "HomeScene.h"
#include "GameScene.h"
#include "FavScene.h"
#include "ShareScene.h"
#include "ShopScene.h"
#include "GameScene.h"
#include "EatScene.h"
#include "LoadingScene.h"
#include "SelectScene.h"
#include "EatIceCreamScene.h"
#include "EatIcePopScene.h"
#include "EatSnowScene.h"

USING_NS_CC;

SceneManager::SceneManager(){
}

void SceneManager::enterLoadingScene()
{
    Scene* pScene = LoadingScene::create();
    this->showScene(pScene);
}

Scene* SceneManager::enterHomeScene()
{
    Scene* pScene = HomeScene::create();
    this->replaceScene(pScene);
    return pScene;
}

void SceneManager::enterGameScene()
{
    Scene* pScene = GameScene::create();
    this->showScene(pScene);
}

Scene* SceneManager::enterSelectScene()
{
    Scene* pScene = SelectScene::create();
    //this->showScene(pScene);
    this->replaceScene(pScene);
    
    return pScene;
}
void SceneManager::enterShareScene()
{
    Scene* pScene = ShareScene::create();
    this->showScene(pScene);
}

void SceneManager::enterFavScene()
{
    
    Scene* pScene = FavScene::create();
    this->showScene(pScene);
}

void SceneManager::enterShopScene()
{
    Scene* pScene = ShopScene::create();
    this->showScene(pScene);
}
Scene* SceneManager::enterIceCreamEatScene()
{
    Scene* pScene = EatIceCreamScene::create();
    this->showScene(pScene);
    
    return pScene;
}

Scene* SceneManager::enterIcePopEatScene()
{
    Scene* pScene = EatIcePopScene::create();
    this->showScene(pScene);
    
    return pScene;
}
Scene* SceneManager::enterSnowConeEatScene()
{
    Scene* pScene = EatSnowScene::create();
    this->showScene(pScene);
    
    return pScene;
}
void SceneManager::popScene()
{
    Director::getInstance()->popScene();
}



void SceneManager::popToRootScene()
{
    Director::getInstance()->popToRootScene();
    SceneManager::enterHomeScene();
}

void SceneManager::showScene(cocos2d::Scene *scene)
{
    if( Director::getInstance()->getRunningScene() )
        Director::getInstance()->pushScene(TransitionFade::create(1.0, scene, Color3B::WHITE));
    else
        Director::getInstance()->runWithScene(scene);
}

void SceneManager::replaceScene(cocos2d::Scene *scene)
{
    if( Director::getInstance()->getRunningScene() )
        Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene, Color3B::WHITE));
    else
        Director::getInstance()->runWithScene(scene);
    
}