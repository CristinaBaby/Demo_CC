//
//  SceneManager.cpp
//  X_BrainTest_zlwd
//
//  Created by studio on 14-1-18.
//
//

#include "SceneManager.h"
#include "HomeScene.h"

#include "FavScene.h"
#include "ShareScene.h"
#include "ShopScene.h"
#include "EatScene.h"
#include "LoadingScene.h"
#include "SelectScene.h"
#include "DecorationScene.h"


//make中间步骤
#include "MakeMoldScene.h"
#include "CookMakeScene.h"
#include "PopFruitScene.h"
#include "MixMoldScene.h"
USING_NS_CC;

SceneManager::SceneManager(){
}

void SceneManager::enterLoadingScene()
{
    Scene* pScene = LoadingScene::create();
    this->replaceScene(pScene);
}

Scene* SceneManager::enterHomeScene()
{
    Scene* pScene = HomeScene::create();
    this->replaceScene(pScene);
    return pScene;
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

void SceneManager::popScene()
{
    Director::getInstance()->popScene();
}
void SceneManager::popToRootScene()
{
    Director::getInstance()->popToRootScene();
    SceneManager::enterHomeScene();
}

void SceneManager::enterDecScene()
{
    Scene* pScene = DecorationScene::create();
    this->replaceScene(pScene);

}

Scene* SceneManager::enterEatScene()
{
    Scene* pScene = EatScene::create();
    this->showScene(pScene);
    
    return pScene;

}
#pragma mark - 进入中间步骤
void SceneManager::enterMakeMoldScene()
{

    Scene* pScene = MakeMoldScene::createScene();
    this->replaceScene(pScene);
    

}
void SceneManager::enterCookMakeScene()
{
    
    Scene* pScene = CookMakeScene::createScene();
    this->replaceScene(pScene);
}
void SceneManager::enterPopFruitScene()
{
    Scene* pScene = PopFruitScene::createScene();
    this->replaceScene(pScene);
}
void SceneManager::enterCookMixScene()
{
    Scene* pScene = MixMoldScene::createScene();
    this->replaceScene(pScene);
}
void SceneManager::showScene(cocos2d::Scene *scene)
{
   
    
    if( Director::getInstance()->getRunningScene() )
    {
        
//        Director::getInstance()->pushScene(randScene(scene));
        Director::getInstance()->pushScene(scene);
    }
    else
    {
//        Director::getInstance()->runWithScene(randScene(scene));
        Director::getInstance()->runWithScene(scene);
        
    }
    
}

void SceneManager::replaceScene(cocos2d::Scene *scene)
{
   
    
    if( Director::getInstance()->getRunningScene() )
    {
        
//        Director::getInstance()->replaceScene(randScene(scene));
         Director::getInstance()->replaceScene(scene);
    }
    
    else
    {
        
//        Director::getInstance()->runWithScene(randScene(scene));
        Director::getInstance()->runWithScene(scene);
    }
    
    
}
Scene* SceneManager::randScene(Scene* scene)
{
    m_vScene.clear();
    
    TransitionProgressRadialCCW* progressScene= TransitionProgressRadialCCW::create(1.0, scene);
    TransitionProgressRadialCW* progressRadialCW= TransitionProgressRadialCW::create(1.0, scene);
    TransitionProgressVertical* pv= TransitionProgressVertical::create(1.0, scene);
  
//    TransitionRotoZoom* rz= TransitionRotoZoom::create(1.0, scene);
   
    TransitionSlideInR* sit = TransitionSlideInR::create(1.0, scene);
    
    m_vScene.pushBack(sit);
    m_vScene.pushBack(progressScene);
    m_vScene.pushBack(progressRadialCW);
    m_vScene.pushBack(pv);

    
//    m_vScene.pushBack(rz);
    
   
    
    int num = rand()%3;
    log("----num %d",num);
    
    return m_vScene.at(0);
}










