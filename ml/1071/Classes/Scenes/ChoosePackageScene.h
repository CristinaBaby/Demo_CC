
#ifndef __ChoosePackageScene__
#define __ChoosePackageScene__


#include "ExtensionScene.h"
#include "RoleModel.h"

class ChoosePackageScene : public ExtensionScene
{
public:
    ChoosePackageScene();
    ~ChoosePackageScene();
    
    MY_SCENE(ChoosePackageScene);
    CREATE_FUNC(ChoosePackageScene);
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(ChoosePackageScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(ChoosePackageScene));}
    
    virtual bool init();
    virtual void onExit();
protected:
    virtual void _initData();
};

#endif