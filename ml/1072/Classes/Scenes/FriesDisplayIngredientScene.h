
#ifndef FriesDisplayIngredientScene_h
#define FriesDisplayIngredientScene_h

#include "ExtensionScene.h"

class FriesDisplayIngredientScene : public ExtensionScene
{
public:
    FriesDisplayIngredientScene();
    ~FriesDisplayIngredientScene();
    
    MY_SCENE(FriesDisplayIngredientScene);
    CREATE_FUNC(FriesDisplayIngredientScene);
    
    virtual bool init();
    virtual void onEnter();
protected:
    virtual void _initData();
    Sprite* m_pBg;
};
#endif /* FriesDisplayIngredientScene_h */
