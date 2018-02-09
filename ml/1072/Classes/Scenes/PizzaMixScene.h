
#ifndef PizzaMixScene_h
#define PizzaMixScene_h

#include "ExtensionScene.h"

class PizzaMixScene : public ExtensionScene
{
public:
    PizzaMixScene();
    ~PizzaMixScene();
    
    MY_SCENE(PizzaMixScene);
    CREATE_FUNC(PizzaMixScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    typedef enum{
        eTagFlour,
        eTagWater,
        eTagYeast,
        eTagSalt,
        eTagOil,
        eTagWhrisk,
        eTagCloth,
        eTagEnd
    }IngredientTag;
protected:
    virtual void _initData();
    
    //    Mix UI
    void _createMixUI();
    void _showMixIngredient(int index);
    void _showWhisk();
    void _removeWhisk();
    
    //    deel
    void _back(DragNode* pIngredient);
    void _addIngredient(DragNode* pIngredient);
    void _mixBatter();
    void _ferment();
    
    void _finishAdd(int index);
    void _finishMix();
    
    void _finishAll();
    
    Sprite* m_pBg;
    
    // ****** Mix Unit
    Sprite* m_pBowl;
    Node* m_pMixture;
    Sprite* m_pBowlFront;
    
    Sprite* m_pFlourBag;
    Sprite* m_pFlourBagFront;
    Sprite* m_pOilBowl;
    Sprite* m_pSaltBowl;
    Sprite* m_pYeastBowl;
    Sprite* m_pCloth;
    
    DragNode* m_pYeastSpoon; //发酵粉
    DragNode* m_pFlour;
    DragNode* m_pOilSpoon;
    DragNode* m_pSaltSpoon;
    DragNode* m_pWater;
    DragNode* m_pWhrisk;
    
    int m_nAddCount;
    int m_nMixCount;
    bool m_bWhriskIsReady;
};
#endif /* PizzaMixScene_h */
