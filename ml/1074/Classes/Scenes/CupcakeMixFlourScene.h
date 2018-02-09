
#ifndef CupcakeMixFlourScene_h
#define CupcakeMixFlourScene_h

#include "ExtensionScene.h"
#include "SieveNode.h"
#include "TouchLayer.h"

class CupcakeMixFlourScene : public ExtensionScene
{
public:
    CupcakeMixFlourScene();
    ~CupcakeMixFlourScene();
    
    MY_SCENE(CupcakeMixFlourScene);
    CREATE_FUNC(CupcakeMixFlourScene);
    
    virtual bool init();
    virtual void onEnter();
    
    virtual void onButtonCallback(Button* btn);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    void onAcceleration(Acceleration *, Event *);
    void TouchEnded(Touch *pTouch, Event *pEvent);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showBowl();
    void _showMixTools();
    void _showIngredientsStep(int step);
    void _showIngredient(int index);
    void _showSieve();
    void _showFlavorScrollView();
    void _showBottle(int index);
    void _showProgressStar();
    
    void _addIngredient(DragNode* pNode);
    void _ingredientOut(DragNode* pNode);
    void _toolElectricWork(bool working = true);
    void _separatePowder();
    void _onTypeClicked(LockItem* item);
    void _addFlavor();
    void _saveMixture();
    
    void _finishAdd();
    void _finishMix();
    void _finish();
    
    Sprite* m_pBg;
    TouchLayer* m_pTouchLayer;
    
    Sprite* m_pBowl;
    Sprite* m_pBowlFront;
    ClippingNode* m_pBatterClipping;
    Node* m_pBatter;
    Node* m_pFlourBag;
    
    DragNode* m_pMixTool_normal;
    DragNode* m_pMixTool_electric;
    DragNode* m_pCurTool;
    
    SieveNode* m_pSieve;
//    ItemScrollView* m_pDecorationScrollView;
    ExpandScrollView* m_pScrollView;
    Sprite* m_pBanner;
    
    Vector<LockItem*> m_typeItems;
    DragNode* m_pBottle;
    
    ProgressStar*m_pProgressStar;
    
    bool m_bItemEnable;
    
    int m_nStep;
    int m_nMixStep;
    int m_nFlourAdd;
    int m_nShakeCount;
    int m_nShakeAdd;
    int m_nFlavorIndex;
    int m_nEggCount;
//    std::string m_sCurTypePath;
};
#endif /* CupcakeMixFlourScene_h */
