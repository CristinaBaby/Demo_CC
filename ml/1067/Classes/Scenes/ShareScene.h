
#ifndef __ShareScene__
#define __ShareScene__


#include "ExtensionScene.h"
#include "EatLayer.h"
#include "CandlePosition.h"
#include "CandleNode.h"
#include "ChooseBackgroudLayer.h"
#include "RuntimePermissionManager.h"

class ShareScene : public ExtensionScene,public RuntimePermissionDelegate
{
public:
    ShareScene();
    ~ShareScene();
    
    MY_SCENE(ShareScene);
    CREATE_FUNC(ShareScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onNotificationCallback(Ref* ref){};
    virtual void onButtonCallback(Button* btn);
    virtual void onKeyBackClicked();
    
    void onEatBeinCallback(Vec2 location,bool drinkable);
    void onEatEndCallback(Vec2 location);
    
    void onBgCallback(int index);
    void onPermissionGrantedResult(int requestCode,bool bGranted);
protected:
    virtual void _initData();
    void _showEatScene();
    
    void _savePhoto();
    void _sendEmail();
    
    RenderTexture* getResultRender();
    
    int m_nPackage;
    EatLayer* m_pEatLayer;
    Layer* m_pDecoLayer;
    DecorateManager* m_pDecManager;
    Sprite* m_pBg;
    Sprite* m_pTip;
    
    
    bool m_bBack;
    int m_nBgIndex;
    int m_nRequestType;
};

#endif