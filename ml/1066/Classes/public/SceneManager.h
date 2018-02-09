
#ifndef __SceneManager__
#define __SceneManager__

#include <iostream>
#include "SingleTon.h"
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
    void enterHomeScene();
    void enterEatScene();
    void enterShopScene();

    void enterChooseScene();
    void enterMakeConeStep01Scene();
    void enterMakeConeStep02Scene();
    void enterDecorationConeScene();
    void enterMakeIceCreamStep01Scene();
    void enterMakeIceCreamStep02Scene();
    void enterDecorationIceCreamScene();
    
    void popScene();
    void popToRootScene();
    
    void showShopInView(Node* view, int zOrder);
private:
    void showScene(Scene *scene);
    void replaceScene(Scene *scene);
    
};
#endif /* defined(__SceneManager__) */
