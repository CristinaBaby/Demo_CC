
#ifndef ShortcakeMixFlourScene_h
#define ShortcakeMixFlourScene_h

#include "ExtensionScene.h"
#include "SieveNode.h"
#include "TouchLayer.h"

class ShortcakeMixFlourScene : public ExtensionScene
{
public:
    ShortcakeMixFlourScene();
    ~ShortcakeMixFlourScene();
    
    MY_SCENE(ShortcakeMixFlourScene);
    CREATE_FUNC(ShortcakeMixFlourScene);
    
    virtual bool init();
    virtual void onEnter();
    
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
    void _showTypeItemScroll();
    void _showBottle(int index);
    void _showProgressStar();
    
    void _setContainer(KettleNode* pContainer,int flavorIndex){};
    void _addIngredient(DragNode* pNode);
    void _ingredientOut(DragNode* pNode);
    void _toolElectricWork(bool working = true);
    void _separatePowder();
    void _onTypeClicked(LockItem* item);
    void _pourSauce();
    
    void _finishMix();
    
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
    
    ExpandScrollView* m_pScrollView;
    Vector<LockItem*> m_typeItems;
    bool m_bItemEnable;
    DragNode* m_pSauceBottle;
    ProgressStar*m_pProgressStar;
    
    int m_nStep;
    int m_nFlourAdd;
    int m_nShakeCount;
    int m_nShakeAdd;
    int m_nFlavorIndex;
    int m_nEggCount;
};
#endif /* ShortcakeMixFlourScene_h */
