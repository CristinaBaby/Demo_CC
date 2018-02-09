
#ifndef NachoDisplayIngredientScene_h
#define NachoDisplayIngredientScene_h

#include "ExtensionScene.h"

class NachoDisplayIngredientScene : public ExtensionScene
{
public:
    NachoDisplayIngredientScene();
    ~NachoDisplayIngredientScene();
    
    MY_SCENE(NachoDisplayIngredientScene);
    CREATE_FUNC(NachoDisplayIngredientScene);
    
    virtual bool init();
    virtual void onEnter();
protected:
    virtual void _initData();
    Sprite* m_pBg;
};
#endif /* NachoDisplayIngredientScene_h */
