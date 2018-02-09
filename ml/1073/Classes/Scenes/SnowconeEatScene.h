
#ifndef SnowconeEatScene_h
#define SnowconeEatScene_h

#include "ExtensionScene.h"
#include "EatLayer.h"
#include "RuntimePermissionManager.h"

class SnowconeEatScene : public ExtensionScene,public RuntimePermissionDelegate
{
public:
    SnowconeEatScene();
    ~SnowconeEatScene();
    
    MY_SCENE(SnowconeEatScene);
    CREATE_FUNC(SnowconeEatScene);
    
    virtual bool init();
    virtual void onEnter();
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(SnowconeEatScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(SnowconeEatScene));}
    
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
    
    void _showSnowcones();
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
    MakeEatLayer* m_pSnowcones;
    
    DragNode* m_pSpoon;
    
    bool m_bCanEat;
    
    bool m_bBack;
    int m_nBgIndex;
    
    int m_nRequestType;
    
    int m_nEatCount;
};
#endif /* SnowconeEatScene_h */
