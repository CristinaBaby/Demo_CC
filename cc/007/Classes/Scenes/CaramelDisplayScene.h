
#ifndef CaramelDisplayScene_h
#define CaramelDisplayScene_h

#include "ExtensionScene.h"

class CaramelDisplayScene : public ExtensionScene
{
public:
    CaramelDisplayScene();
    ~CaramelDisplayScene();
    
    MY_SCENE(CaramelDisplayScene);
    CREATE_FUNC(CaramelDisplayScene);
    
    virtual bool init();
    virtual void onEnter();
    
    virtual void onButtonCallback(Button* btn);
    
protected:
    virtual void _initData();
    
    void _showCookie();
    
    Sprite* m_pBg;
    bool m_bButtonEable;
};
#endif /* CaramelDisplayScene_h */
