
#ifndef __MakeSnowconeScene__
#define __MakeSnowconeScene__


#include "ExtensionScene.h"
#include "FreezerNode.h"
#include "MachineNode.h"
#include "ScribbleNode.h"

class MakeSnowconeScene : public ExtensionScene
{
public:
    MakeSnowconeScene();
    ~MakeSnowconeScene();
    
    MY_SCENE(MakeSnowconeScene);
    CREATE_FUNC(MakeSnowconeScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    RenderTexture* getResultRender();
    void onButtonCallback(Button* btn);
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    
    void onItemClicked(LockItem* item);
    void onShapeItemClicked(LockItem* item);

    void onFreezerCallback(FreezerNode*);
    void onMachineStateCallback(int state);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    enum{
        eStepMakeIce,
        eStepCrushIce,
        eStepShape,
    };
protected:
    virtual void _initData();
    
    void _showShadow(DragNode* pDragNode);
    void _hideShadow(DragNode* pDragNode);
    
    void _showIconIce();
    void _removeIconIce(int index);
    
    void _showIconShape();
    void _removeIconShape(int index);
    
    void _showColorBottle();
    
    void _showTray();
    void _showFreezer();
    void _showMachine();
    void _showShelf(float dt = 0,bool isCenter = true);
    void _moveawayShelf();
    void _showCone(Vec2 posDelt = Vec2::ZERO);
    void _moveawayCone();
    
    void _pourWater();
    
    void _putTrayInFreezer();
    void _addIceMachine();
    void _curshIceFinished();
    
    void _resetColor();
    
    void _playEffectYummy(float);
    
    Vector<LockItem*> m_IconIces;
    
    ExpandScrollView* m_pScrollView;
    
    ExpandScrollView* m_pBottleList;
    
    
    DragNode* m_pTray;
    DragNode* m_pSpoon;
    DragNode* m_pKnife;
    
    Sprite* m_pTrayMask;
    ClippingNode* m_pTrayClipping;
    
    FreezerNode* m_pFreezer;
    MachineNode* m_pMachine;
    
    DragNode* m_pWaterBottle;
    
    Sprite* m_pTrayInFreezer;
    
    Node* m_pSnowConeNode;
    Sprite* m_pCone;
    Sprite* m_pConeFront;
    Sprite* m_pConeIce;
    
    Sprite* m_pBrush;
    ScribbleNode* m_pSnowSCribble;
    
    ScribbleNode* m_pColorSCribble;
    
    Vector<Sprite*> m_WaterVector;
    Vector<Sprite*> m_IceVector;
    int m_nShapeIndex;
    bool m_bShapTouchEnable;
    
    int m_nUpCount;
    int m_nDownCount;
    int m_nPourCount;
    
    bool m_bFreezed;
    bool m_bSpoonFull;
    int m_nStep;
    int m_nSnowPutCount;
    int m_nKnifeCount;
    int m_nShapePercent;
    int m_nFlavorIndex;
    int m_bResetShown;
};

#endif