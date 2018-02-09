
#ifndef HotdogDisplayIngredientScene_h
#define HotdogDisplayIngredientScene_h

#include "ExtensionScene.h"

class HotdogDisplayIngredientScene : public ExtensionScene
{
public:
    HotdogDisplayIngredientScene();
    ~HotdogDisplayIngredientScene();
    
    MY_SCENE(HotdogDisplayIngredientScene);
    CREATE_FUNC(HotdogDisplayIngredientScene);
    
    virtual bool init();
    virtual void onEnter();
protected:
    virtual void _initData();
    Sprite* m_pBg;
};
#endif /* HotdogDisplayIngredientScene_h */
