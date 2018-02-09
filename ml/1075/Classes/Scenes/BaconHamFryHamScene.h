
#ifndef BaconHamFryHamScene_h
#define BaconHamFryHamScene_h

#include "ExtensionScene.h"
#include "ScribbleNode.h"
#include "FriedUnit.h"

class BaconHamFryHamScene : public ExtensionScene
{
public:
    BaconHamFryHamScene();
    ~BaconHamFryHamScene();
    
    MY_SCENE(BaconHamFryHamScene);
    CREATE_FUNC(BaconHamFryHamScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _showStove();
    void _showPan();
    void _showHams();
    void _showButter();
    
    void _addButter(DragNode* pIngredient);
    void _paintButter();
    
    void _doFryBacon(FriedUnit* pBacon);
    void _finishFryBacon();
    void _finish();
    
    Sprite* m_pBg;
    
    Sprite* m_pStove;
    Sprite* m_pFire;
    Sprite* m_pPan;
    Sprite* m_pPanFront;
    Sprite* m_pPlate;
    Sprite* m_pBaconPlate;
    
    Node* m_pPanIn;
    DragNode* m_pButter;
    DragNode* m_pClip;
    
    std::vector<Sprite*> m_pBaconVector;
    std::vector<FriedUnit*> m_pBaconFriedVector;
    
    ScribbleNode* m_pButterScribble;
    int m_nMoveCount;
    bool m_bEffectTurn;
    bool m_bEffectBaconOK;
};
#endif /* BaconHamFryHamScene_h */
