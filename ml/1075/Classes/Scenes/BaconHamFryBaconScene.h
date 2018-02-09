
#ifndef BaconHamFryBaconScene_h
#define BaconHamFryBaconScene_h

#include "ExtensionScene.h"
#include "FriedUnit.h"

class BaconHamFryBaconScene : public ExtensionScene
{
public:
    BaconHamFryBaconScene();
    ~BaconHamFryBaconScene();
    
    MY_SCENE(BaconHamFryBaconScene);
    CREATE_FUNC(BaconHamFryBaconScene);
    
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
    void _showBacons();
    
    void _doFryBacon(FriedUnit* pBacon);
    void _finishFryBacon();
    void _finish();
    
    Sprite* m_pBg;
    
    Sprite* m_pStove;
    Sprite* m_pFire;
    Sprite* m_pPan;
    Sprite* m_pPanFront;
    Sprite* m_pPlate;
    Node* m_pPanIn;
    
    Sprite* m_pBaconPlate;
    std::vector<Sprite*> m_pBaconVector;
    std::vector<FriedUnit*> m_pBaconFriedVector;
    DragNode* m_pClip;
    
    bool m_bEffectTurn;
    bool m_bEffectBaconOK;
};
#endif /* BaconHamFryBaconScene_h */
