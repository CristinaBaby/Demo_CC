
#ifndef __ShareScene__
#define __ShareScene__


#include "ExtensionScene.h"
#include "EatLayer.h"
#include "RuntimePermissionManager.h"

class ShareScene : public ExtensionScene,public RuntimePermissionDelegate
{
public:
    enum{
        eRequesetTagPhoto,
        eRequesetTagEmail,
        eRequesetTagAudio,
    };
    ShareScene();
    ~ShareScene();
    
    MY_SCENE(ShareScene);
    CREATE_FUNC(ShareScene);
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(ShareScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(ShareScene));}
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void onPermissionGrantedResult(int requestCode,bool bGranted);
    virtual void onNotificationCallback(Ref* ref){};
    virtual void onButtonCallback(Button* btn);
    virtual void onKeyBackClicked();
    
    virtual void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    virtual void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    virtual void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    virtual void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    
    virtual void onEatBeinCallback(Vec2 location,bool drinkable);
    virtual void onEatEndCallback(Vec2 location);
    virtual void onEatMoveCallback(Vec2 location);
    
    void onBgCallback(int index);
    
    virtual void update(float dt);
protected:
    virtual void _initData();
    virtual void _reset();
    virtual void _showEatScene();
    virtual void _savePhoto();
    virtual void _sendEmail();
    
    void _showDonut(bool animate);
    RenderTexture* getResultRender();
    
    int m_nPackage;
    EatLayer* m_pEatLayer;
    Layer* m_pDecoLayer;
    DecorateManager* m_pDecManager;
    Sprite* m_pBg;
    Sprite* m_pTable;
    Sprite* m_pShed;
    Sprite* m_pTip;
    
    Sprite* m_pPlate;
    
    MakeEatLayer* m_pEatLayerTop;
    MakeEatLayer* m_pEatLayerDown;
    
    Node* m_pDrink;
    bool m_bCanEat;
    
    bool m_bBack;
    int m_nBgIndex;
};

#endif