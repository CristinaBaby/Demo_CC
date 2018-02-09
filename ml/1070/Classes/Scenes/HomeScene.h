
#ifndef __HomeScene__
#define __HomeScene__


#include "ExtensionScene.h"
#include "SSCMoreGameButton.h"
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
    virtual void onButtonCallback(Button* btn);
    virtual void onKeyBackClicked();
    void clickedMoreGameButton();
protected:
    virtual void _initData();
    void _showParticle();
    void _onStart();
    void _showLogo();
    void _showDecLogoLayer();
    SSCMoreGameButton* m_moreGameButton;
    Sprite* m_pPlaySprite;
    ClippingNode* m_pLogoClipping;
    
    Node* m_pCart;
    Node* m_pGirl;
    Node* m_pCartDec;
    bool m_bCanTouch;
    bool m_bShowMoreGame;
};

#endif