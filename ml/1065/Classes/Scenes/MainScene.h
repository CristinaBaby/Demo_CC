
#ifndef __MainScene__
#define __MainScene__


#include "ExtensionScene.h"

class MainScene : public ExtensionScene
{
public:
    MainScene();
    ~MainScene();
    
    MY_SCENE(MainScene);
    CREATE_FUNC(MainScene);
    
    virtual bool init();
    
protected:
    virtual void _initData();
};

#endif