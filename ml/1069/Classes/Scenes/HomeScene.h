
#ifndef __HomeScene__
#define __HomeScene__


#include "ExtensionScene.h"
#include "RuntimePermissionManager.h"

class HomeScene : public ExtensionScene,public RuntimePermissionDelegate
{
public:
    HomeScene();
    ~HomeScene();
    
    MY_SCENE(HomeScene);
    CREATE_FUNC(HomeScene);
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(HomeScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(HomeScene));}
    
    
    void onPermissionGrantedResult(int requestCode,bool bGranted);
    
    virtual bool init();
    virtual void onEnter();
    void onEnterTransitionDidFinish();
    virtual void onExit();
    virtual void onButtonCallback(Button* btn);
    virtual void onKeyBackClicked();
    void clickedMoreGameButton(Ref* ref,Widget::TouchEventType type);
    SSCMoreGameButton* m_moreGameButton = nullptr;
protected:
    virtual void _initData();
    void _showParticle();
    void _showLogo();
    void _showDecLogoLayer();
    
    
    ClippingNode* m_pLogoClipping;
    Vector<Sprite*> m_VectorKid;
    bool m_bShowMoreGame;
    Node* m_pLogoNode;
    Sprite* m_pPlaySprite;
    Sprite* m_pLogoSprite;
    bool m_bCanTouch;
    
    RoleModel* m_pGirl;
};

#endif