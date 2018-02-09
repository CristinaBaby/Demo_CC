
#ifndef NachoEatScene_h
#define NachoEatScene_h

#include "ExtensionScene.h"
#include "EatLayer.h"
#include "RuntimePermissionManager.h"

class NachoEatScene : public ExtensionScene,public RuntimePermissionDelegate
{
public:
    NachoEatScene();
    ~NachoEatScene();
    
    MY_SCENE(NachoEatScene);
    CREATE_FUNC(NachoEatScene);
    
    virtual bool init();
    virtual void onEnter();
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(NachoEatScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(NachoEatScene));}
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    void onPermissionGrantedResult(int requestCode,bool bGranted);
    virtual void onNotificationCallback(Ref* ref){};
    virtual void onButtonCallback(Button* btn);
    virtual void onKeyBackClicked();
    
    void onEatBeinCallback(Vec2 location,bool drinkable);
    void onEatEndCallback(Vec2 location);
    
    void onBgCallback(int index);
protected:
    virtual void _initData();
    
    void _showEatScene();
    void _savePhoto();
    void _sendEmail();
    
    void _showNachos();
    void _showSpoon();
    
    void _reset();
    
    RenderTexture* getResultRender();
    
    int m_nPackage;
    EatLayer* m_pEatLayer;
    Layer* m_pDecoLayer;
    
    Sprite* m_pBg;
    Sprite* m_pTable;
    Sprite* m_pShed;
    Sprite* m_pTip;
    
    Sprite* m_pPlate;
    Sprite* m_pPanFront;
    MakeEatLayer* m_pNachos;
    
    DragNode* m_pSpoon;
    
    bool m_bCanEat;
    
    bool m_bBack;
    int m_nBgIndex;
    
    int m_nRequestType;
    
    int m_nEatCount;
};
#endif /* NachoEatScene_h */
