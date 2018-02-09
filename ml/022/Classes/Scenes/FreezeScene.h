
#ifndef __FreezeScene__
#define __FreezeScene__


#include "ExtensionScene.h"
#include "FreezerNode.h"

class FreezeScene : public ExtensionScene
{
public:
    FreezeScene();
    ~FreezeScene();
    
    MY_SCENE(FreezeScene);
    CREATE_FUNC(FreezeScene);
    virtual bool init();
    virtual void onEnter();
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    void onFreezerCallback(FreezerNode*);
protected:
    virtual void _initData();
    void _showShadow(DragNode* pDragNode);
    void _hideShadow(DragNode* pDragNode);
    
    void _showTray();
    void _showFreezer();
    void _putTrayInFreezer();
    
    FreezerNode* m_pFreezer;
    Sprite* m_pTrayInFreezer;
    
    DragNode* m_pTray;
    Sprite* m_pTrayMask;
    ClippingNode* m_pTrayClipping;
    
    Vector<Sprite*> m_WaterVector;
    Vector<Sprite*> m_IceVector;
    int m_nPourCount;
    int m_nShapeIndex;
    int m_nIceCount;
    bool m_bFreezed;
};

#endif