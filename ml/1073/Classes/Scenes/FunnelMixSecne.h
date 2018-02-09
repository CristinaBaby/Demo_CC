
#ifndef FunnelMixSecne_h
#define FunnelMixSecne_h

#include "ExtensionScene.h"

class FunnelMixSecne : public ExtensionScene
{
public:
    FunnelMixSecne();
    ~FunnelMixSecne();
    
    MY_SCENE(FunnelMixSecne);
    CREATE_FUNC(FunnelMixSecne);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    typedef enum{
        eTagBegin = 99,
        eTagSugar,
        eTagFlour,
        eTagEgg,
        eTagPowder,
        eTagMilk,
        eTagSalt,
        eTagWhrisk,
        eTagEnd
    }IngredientTag;
    
    typedef enum{
        eSugar = 100,
        eFlour,
        eEgg0,
        eEgg1,
        eWater,
        eYeast,
        eSalt,
    }IngredientIndex;
protected:
    virtual void _initData();
    
    void _createMixUI();
    void _showWhisk();
    void _removeWhisk();
    void _showMixIngredient(int index);
    
    void _back(DragNode* pIngredient);
    void _addIngredient(DragNode* pIngredient);
    void _mixBatter();
    
    void _finishAdd(int index);
    void _finishMix();
    
    Sprite* m_pBg;
    
    Sprite* m_pBowl;
    Sprite* m_pBowlFront;
    ClippingNode* m_pMixture;
    
    DragNode* m_pWhrisk;
    
    DragNode* m_pEgg0;
    DragNode* m_pEgg1;
    DragNode* m_pEgg2;
    DragNode* m_pMilk;
    
    Sprite* m_pSalt;
    DragNode* m_pSaltSpoon;
    
    DragNode* m_pSugar;
    Sprite* m_pFlourBag;
    DragNode* m_pFlour;
    Sprite* m_pPowder;
    DragNode* m_pPowderSpoon;
    
    int m_nMilkCount;
    int m_nEggCount;
    int m_nAddCount;
    bool m_bReadyAdd;
    int m_nMixCount;
};
#endif /* FunnelMixSecne_h */
