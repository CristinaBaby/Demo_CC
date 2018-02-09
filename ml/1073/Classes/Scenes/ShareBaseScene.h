
#ifndef __ShareBaseScene__
#define __ShareBaseScene__


#include "ExtensionScene.h"
#include "EatLayer.h"
#include "RuntimePermissionManager.h"

class ShareBaseScene : public ExtensionScene,public RuntimePermissionDelegate
{
public:
    ShareBaseScene();
    ~ShareBaseScene();
    
    MY_SCENE(ShareBaseScene);
    CREATE_FUNC(ShareBaseScene);
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(ShareBaseScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(ShareBaseScene));}
    virtual bool init();
    virtual void onEnter();
    void onExit();
    void onPermissionGrantedResult(int requestCode,bool bGranted);
    virtual void onNotificationCallback(Ref* ref){};
    virtual void onButtonCallback(Button* btn);
    virtual void onKeyBackClicked();
    
    void onBgCallback(int index);
protected:
    virtual void _initData();
    virtual void _reset(){}
    void _showEatScene();
    void _savePhoto();
    void _sendEmail();
    
    RenderTexture* getResultRender();
    
    int m_nPackage;
    
    Sprite* m_pBg;
    
    Sprite* m_pTip;
    
    bool m_bBack;
    int m_nRequestType;
};

#endif