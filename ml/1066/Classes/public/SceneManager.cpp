//
//  SceneManager.cpp
//  X_BrainTest_zlwd
//
//  Created by studio on 14-1-18.
//
//

#include "SceneManager.h"
#include "Global.h"

#include "LoadingLayer.h"
#include "HomeLayer.h"
#include "ShopLayer.h"
#include "EatLayer.h"
#include "ChooseLayer.hpp"
#include "MakeConeStep01.hpp"
#include "MakeConeStep02.hpp"
#include "MakeIceCreamStep01.hpp"
#include "MakeIceCreamStep02.hpp"
#include "DecorationConeLayer.hpp"
#include "DecorationIceCreamLayer.hpp"

USING_NS_CC;

SceneManager::SceneManager(){
}

void SceneManager::enterLoadingScene()
{
    Scene* pScene = LoadingLayer::scene();
    Director::getInstance()->runWithScene(pScene);
}

void SceneManager::enterHomeScene()
{
    Scene* pScene = HomeLayer::scene();
    
    this->replaceScene(pScene);
}

void SceneManager::enterChooseScene()
{
    Scene* pScene = ChooseLayer::scene();
    
    this->replaceScene(pScene);
}

void SceneManager::enterMakeConeStep01Scene()
{
    Scene* pScene = MakeConeStep01::scene();
    
    this->replaceScene(pScene);
}

void SceneManager::enterMakeConeStep02Scene()
{
    Scene* pScene = MakeConeStep02::scene();
    
    this->replaceScene(pScene);
}

void SceneManager::enterDecorationConeScene()
{
    Scene* pScene = DecorationConeLayer::scene();
    
    this->replaceScene(pScene);
}

void SceneManager::enterMakeIceCreamStep01Scene()
{
    Scene* pScene = MakeIceCreamStep01::scene();
    
    this->replaceScene(pScene);
}

void SceneManager::enterMakeIceCreamStep02Scene()
{
    Scene* pScene = MakeIceCreamStep02::scene();
    
    this->replaceScene(pScene);
}

void SceneManager::enterDecorationIceCreamScene()
{
    Scene* pScene = DecorationIceCreamLayer::scene();
    
    this->replaceScene(pScene);
}

void SceneManager::enterEatScene()
{
    Scene* pScene = EatLayer::scene();
    this->showScene(pScene);
}

void SceneManager::enterShopScene()
{
    Scene* pScene = ShopLayer::scene();
    this->showScene(pScene);
}

//////////////////////////////////////
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
    if( Director::getInstance()->getRunningScene())
        Director::getInstance()->pushScene(TransitionFade::create(1.0, scene, Color3B::WHITE));
    else
        Director::getInstance()->runWithScene(scene);
}

void SceneManager::replaceScene(cocos2d::Scene *scene)
{
    if( Director::getInstance()->getRunningScene())
        Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene, Color3B::WHITE));
    else
        Director::getInstance()->runWithScene(scene);
    
}

void SceneManager::showShopInView(Node* view, int zOrder)
{
    ShopLayer* shopLayer = ShopLayer::create();
    view->addChild(shopLayer, zOrder);
}