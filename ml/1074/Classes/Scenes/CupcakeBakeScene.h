
#ifndef CupcakeBakeScene_h
#define CupcakeBakeScene_h

#include "ExtensionScene.h"
#include "OvenNode.h"

class CupcakeBakeScene : public ExtensionScene
{
public:
    CupcakeBakeScene();
    ~CupcakeBakeScene();
    
    MY_SCENE(CupcakeBakeScene);
    CREATE_FUNC(CupcakeBakeScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _showTray();
    void _doBake();
    void _showGlove();
    void _finishAll();
    
    Sprite* m_pBg;
    DragNode* m_pTray;
    DragNode* m_pGlove;
    OvenNode* m_pOven;
    
    Vector<ClippingNode*> m_batterItems;
    int m_CupFlavor[18]; //6*3
};
#endif /* CupcakeBakeScene_h */
