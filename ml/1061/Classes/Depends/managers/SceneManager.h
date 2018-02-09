
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
    Scene* randScene(Scene* scene);
    
    //make中间的步骤
    void enterMakeMoldScene();
    void enterCookMakeScene();
    void enterPopFruitScene();
    void enterCookMixScene();
private:
    void showScene(Scene *scene);
    void replaceScene(Scene *scene);
    
    
    Vector<TransitionScene*> m_vScene;
};

#define  SceneChangeManager  SceneManager::getInstance()
#endif
