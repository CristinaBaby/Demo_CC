
#ifndef DrinkJuiceScene_h
#define DrinkJuiceScene_h

#include "ShareScene.h"

class DrinkJuiceScene : public ShareScene
{
public:
    DrinkJuiceScene();
    ~DrinkJuiceScene();
    
    MY_SCENE(DrinkJuiceScene);
    CREATE_FUNC(DrinkJuiceScene);
    
    virtual bool init();
    virtual void onEnter();
        
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    virtual void onEatBeinCallback(Vec2 location,bool drinkable);
    virtual void onEatEndCallback(Vec2 location);
//    virtual void onEatMoveCallback(Vec2 location);
protected:
    virtual void _initData();
    
    virtual void _showEatScene();
    
    virtual void _reset();
};
#endif /* DrinkJuiceScene_h */
