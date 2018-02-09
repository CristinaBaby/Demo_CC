
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
    void enterGameScene();
    void enterShareScene();
    Scene* enterSelectScene();
    void enterFavScene();
    void enterShopScene();
    Scene* enterIceCreamEatScene();
    Scene* enterIcePopEatScene();
    Scene* enterSnowConeEatScene();
    
    void popScene();
    void popToRootScene();
private:
    void showScene(Scene *scene);
    void replaceScene(Scene *scene);
    
};

#define         SceneChangeManager     SceneManager::getInstance()
#endif /* defined(__SceneManager__) */
