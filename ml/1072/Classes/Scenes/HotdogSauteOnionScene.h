
#ifndef HotdogSauteOnionScene_h
#define HotdogSauteOnionScene_h

#include "ExtensionScene.h"

class HotdogSauteOnionScene : public ExtensionScene
{
public:
    HotdogSauteOnionScene();
    ~HotdogSauteOnionScene();
    
    MY_PHYSIC_SCENE(HotdogSauteOnionScene);
    CREATE_FUNC(HotdogSauteOnionScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    void update(float);
protected:
    virtual void _initData();
    void _showStove();
    void _showPan();
    void _showOil();
    void _showOnion();
    void _showShovel();
    void _showBrownSugar();
    void _showGarlic();
    void _showPlate();
    
    void _pourOnion();
    void _pourBrownSugar();
    void _pourGarlic();
    
    void _finishAddOil();
    void _finishPourOnion();
    void _finishAll();
    
    Sprite* m_pBg;
    
    Sprite* m_pStove;
    Sprite* m_pFire;
    Sprite* m_pPan;
    Sprite* m_pOil;
    
    Sprite* m_pOilBowl;
    DragNode* m_pOilSpoon;
    
    DragNode* m_pOnionPlate;
    Sprite* m_pOnionInPlate;
    
    
    DragNode* m_pSugarSpoon;
    Sprite* m_pSugarBowl;
    Sprite* m_pSugarInPan;
    
    DragNode* m_pGarlicBowl;
    Sprite* m_pGarlicInBowl;
    Sprite* m_pGarlicInPan;
    
    Sprite* m_pPlate;
    Sprite* m_pPaper;
    
    Node* m_pOnionNode;
    
    DragNode* m_pShovel;
    int m_nMoveCount;
    
    int m_nAddOilCount;
    int m_nAddOnionCount;
};
#endif /* HotdogSauteOnionScene_h */
