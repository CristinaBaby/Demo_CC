
#ifndef BurgerFryBeafScene_h
#define BurgerFryBeafScene_h

#include "ExtensionScene.h"

class BurgerFryBeafScene : public ExtensionScene
{
public:
    BurgerFryBeafScene();
    ~BurgerFryBeafScene();
    
    MY_SCENE(BurgerFryBeafScene);
    CREATE_FUNC(BurgerFryBeafScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    void onAccelerationCallback(Acceleration *, Event *);
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _showStove();
    void _showPan();
    void _showOil();
    void _showBeaf();
    void _showShovel();
    void _removeShovel();
    void _showPlateLeft();
    
    void _showBeafPlate();
    void _hideBeafPlate();
    
    
    void _doFry();
    
    void _finishAddOil();
    void _finishFry();
    void _finishAll();
    
    Sprite* m_pBg;
    
    EventListenerAcceleration* m_pAccelerListener;
    
    Sprite* m_pStove;
    Sprite* m_pFire;
    Sprite* m_pPan;
    Sprite* m_pOil;
    
    Sprite* m_pOilBowl;
    DragNode* m_pOilSpoon;
    
    DragNode* m_pShovel;
    
    Sprite* m_pPlateLeft;
    Sprite* m_pPlateRight;
    
    DragNode*  m_pCurBeaf;
    Sprite* m_pBeafInOil;
    
    std::vector<DragNode*> m_pBeafVector;
    
    
    int m_nAddOilCount;
    
    int m_nFryStep;
    int m_nFryCount;
};
#endif /* BurgerFryBeafScene_h */
