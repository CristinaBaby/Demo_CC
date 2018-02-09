
#ifndef PizzaDisplayIngredientScene_h
#define PizzaDisplayIngredientScene_h

#include "ExtensionScene.h"

class PizzaDisplayIngredientScene : public ExtensionScene
{
public:
    PizzaDisplayIngredientScene();
    ~PizzaDisplayIngredientScene();
    
    MY_SCENE(PizzaDisplayIngredientScene);
    CREATE_FUNC(PizzaDisplayIngredientScene);
    
    virtual bool init();
    virtual void onEnter();
protected:
    virtual void _initData();
    Sprite* m_pBg;
};
#endif /* PizzaDisplayIngredientScene_h */
