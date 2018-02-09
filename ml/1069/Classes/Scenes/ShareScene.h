
#ifndef __ShareScene__
#define __ShareScene__


#include "ExtensionScene.h"
#include "EatLayer.h"
#include "ChooseBackgroudLayer.h"
#include "RuntimePermissionManager.h"

class ShareScene : public ExtensionScene,public RuntimePermissionDelegate
{
public:
    ShareScene();
    ~ShareScene();
    
    MY_SCENE(ShareScene);
    CREATE_FUNC(ShareScene);
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(ShareScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(ShareScene));}
    virtual bool init();
    virtual void onEnter();
    void onExit();
    void onPermissionGrantedResult(int requestCode,bool bGranted);
    virtual void onNotificationCallback(Ref* ref){};
    virtual void onButtonCallback(Button* btn);
    virtual void onKeyBackClicked();
    
    void onEatBeinCallback(Vec2 location,bool drinkable);
    void onEatEndCallback(Vec2 location);
    
    void onBgCallback(int index);
protected:
    virtual void _initData();
    void _savePhoto();
    void _sendEmail();
    void _showEatScene();
    
    RenderTexture* getResultRender();
    
    int m_nPackage;
    EatLayer* m_pEatLayer;
    Layer* m_pDecoLayer;
    DecorateManager* m_pDecManager;
    Sprite* m_pBg;
    Sprite* m_pTip;
    
    Sprite* m_pPlate;
    
    bool m_bCanEat;
    
    bool m_bBack;
    int m_nBgIndex;
    int m_nRequest;
};

#endif