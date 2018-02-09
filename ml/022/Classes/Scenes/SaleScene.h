
#ifndef __SaleScene__
#define __SaleScene__


#include "ExtensionScene.h"
#include "BlenderCup.h"
#include "JuiceMachine.h"
#include "RuntimePermissionManager.h"

class SaleScene : public ExtensionScene,public RuntimePermissionDelegate
{
public:
    SaleScene();
    ~SaleScene();
    
    MY_SCENE(SaleScene);
    CREATE_FUNC(SaleScene);
    virtual bool init();
    virtual void onEnter();
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    void onPermissionGrantedResult(int requestCode,bool bGranted);
    virtual void onNotificationCallback(Ref* ref){};
    virtual void onButtonCallback(Button* btn);
    virtual void onKeyBackClicked();
    RenderTexture* getResultRender();
protected:
    virtual void _initData();
    void _savePhoto();
    void _sendEmail();
    void _showGuest();
    void _pickDrink();
    void _giveCoint();
    
    DragNode* m_pDrink;
    bool m_bReadySale;
    bool m_bBack;
    bool m_bRequestEmail;
};

#endif