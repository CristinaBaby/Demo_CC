
#ifndef __HomeScene__
#define __HomeScene__


#include "ExtensionScene.h"

class HomeScene : public ExtensionScene
{
public:
    HomeScene();
    ~HomeScene();
    
    MY_SCENE(HomeScene);
    CREATE_FUNC(HomeScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void onButtonCallback(Button* btn);
    virtual void onKeyBackClicked();
protected:
    virtual void _initData();
    void _showParticle();
    
    Sprite* m_pLight;
    Vector<Sprite*> m_VectorKid;
    bool m_bShowMoreGame;
};

#endif