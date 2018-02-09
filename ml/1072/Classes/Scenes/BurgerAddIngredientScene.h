
#ifndef BurgerAddIngredientScene_h
#define BurgerAddIngredientScene_h

#include "ExtensionScene.h"

class BurgerAddIngredientScene : public ExtensionScene
{
public:
    BurgerAddIngredientScene();
    ~BurgerAddIngredientScene();
    
    MY_SCENE(BurgerAddIngredientScene);
    CREATE_FUNC(BurgerAddIngredientScene);
    
    enum{
        tagBeaf = 0,
        tagMilk,
        tagOnion,
        tagSauce,
        tagSalt,
        tagGarlic,
        tagPepper,
        tagEgg,
    };
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _hideShadow(DragNode* pNode);
    
    void _showBowl();
//    void _showBeaf();
//    void _showMilk();
//    
//    void _showOnion();
//    void _showSauce();
//    
//    void _showPepper();
//    void _showEgg();
//    
//    void _showGarlic();
//    void _showSalt();
    
    void _showIngredient(int index);
    void _getIngredient(DragNode* pNode,int index);
    void _pourIngredient(DragNode* pNode,int index);
    
    void _finishAdd(int index);
    void _finish();
    
    Sprite* m_pBg;
    
    Sprite* m_pBowl;
    Node* m_pMixture;
    
    Sprite* m_pBeaf;
    DragNode* m_pBeafPlate;
    
    Sprite* m_pMilkBowl;
    DragNode* m_pMilkSpoon;
    
    Sprite* m_pOnion;
    DragNode* m_pOnionBowl;
    
    Sprite* m_pSauceBowl;
    DragNode* m_pSauceSpoon;
    
    Sprite* m_pPepperBowl;
    DragNode* m_pPepperSpoon;
    
    DragNode* m_pEgg;
    
    Sprite* m_pGarlic;
    DragNode* m_pGarlicBowl;
    
    Sprite* m_pSaltBowl;
    DragNode* m_pSaltSpoon;
    
    int m_nAddCount;
    int m_nMilkAddCount;
};
#endif /* BurgerAddIngredientScene_h */
