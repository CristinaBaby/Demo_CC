
#ifndef FunnelDecorateScene_h
#define FunnelDecorateScene_h

#include "ExtensionScene.h"

class FunnelDecorateScene : public ExtensionScene
{
public:
    FunnelDecorateScene();
    ~FunnelDecorateScene();
    
    MY_SCENE(FunnelDecorateScene);
    CREATE_FUNC(FunnelDecorateScene);
    
    void onButtonCallback(Button* btn);
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _initDefaultDecorate();
    void _showSugar();
    void _saveSugar();
    
    Sprite* m_pBg;
    
    Sprite* m_pPlate;
    Sprite* m_pCake;
    ClippingNode* m_pClipping;
    
    DragNode* m_pSugarBottle;
    int m_nMoveCount;
    
};
#endif /* FunnelDecorateScene_h */
