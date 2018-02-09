
#ifndef __HomeScene__
#define __HomeScene__


#include "ExtensionScene.h"
#include "SSCMoreGameButton.h"

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
    void clickedMoreGameButton();
protected:
    virtual void _initData();
    void _showParticle();
    void _showLogo();
    void _showDecLogoLayer();
    SSCMoreGameButton* m_moreGameButton;
    
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