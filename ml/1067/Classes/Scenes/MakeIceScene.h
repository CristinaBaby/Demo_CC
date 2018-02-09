
#ifndef __MakeIceScene__
#define __MakeIceScene__


#include "ExtensionScene.h"
#include "FreezerNode.h"
class MakeIceScene : public ExtensionScene
{
public:
    MakeIceScene();
    ~MakeIceScene();
    
    MY_SCENE(MakeIceScene);
    CREATE_FUNC(MakeIceScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void onItemClicked(LockItem* item);
    void onFreezerCallback(FreezerNode*);
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
protected:
    virtual void _initData();
    
    void _showShadow(DragNode* pDragNode);
    void _hideShadow(DragNode* pDragNode);
    
    void _showShapScrollView();
    void _hidShapScrollView();
    
    void _showTray();
    void _showFreezer();
    void _pourWater();
    void _flipTray();
    void _iceJump();
    
    ExpandScrollView* m_pScrollView;
    
    DragNode* m_pTray;
    Sprite* m_pTrayMask;
    ClippingNode* m_pTrayClipping;
    
    FreezerNode* m_pFreezer;
    
    DragNode* m_pWaterBottle;
    
    Sprite* m_pTrayInFreezer;
    
    Vector<Sprite*> m_WaterVector;
    Vector<Sprite*> m_IceVector;
    int m_nShapeIndex;
    bool m_bShapTouchEnable;
    
    int m_nUpCount;
    int m_nDownCount;
    int m_nPourCount;
    
    bool m_bFreezed;
};

#endif