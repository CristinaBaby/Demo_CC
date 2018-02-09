
#ifndef BurgerDisplayIngredientScene_h
#define BurgerDisplayIngredientScene_h

#include "ExtensionScene.h"

class BurgerDisplayIngredientScene : public ExtensionScene
{
public:
    BurgerDisplayIngredientScene();
    ~BurgerDisplayIngredientScene();
    
    MY_SCENE(BurgerDisplayIngredientScene);
    CREATE_FUNC(BurgerDisplayIngredientScene);
    
    virtual bool init();
    virtual void onEnter();
protected:
    virtual void _initData();
    Sprite* m_pBg;
};
#endif /* BurgerDisplayIngredientScene_h */
