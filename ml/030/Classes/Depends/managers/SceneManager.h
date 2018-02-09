
#ifndef __SceneManager__
#define __SceneManager__

#include <iostream>
#include "../utils/SingleTon.h"
#include "cocos2d.h"

USING_NS_CC;


class SceneManager :public SingleTon<SceneManager>{
private:
    SceneManager();
    friend class SingleTon<SceneManager>;
public:
    virtual ~SceneManager(){}
public:
    void enterLoadingScene();
    Scene* enterHomeScene();
    Scene* enterEatScene();
    void enterGameScene();
    void enterShareScene();
    Scene* enterSelectScene();
    void enterFavScene();
    void enterShopScene();
    void enterDecScene();
    void popScene();
    void popToRootScene();
    
    void enterMarketAction();
    void enterMarket();
    void enterMakeScene();
    void enterIceCreamScene();
private:
    void showScene(Scene *scene);
    void replaceScene(Scene *scene);
    
};

#define  SceneChangeManager  SceneManager::getInstance()
#endif
