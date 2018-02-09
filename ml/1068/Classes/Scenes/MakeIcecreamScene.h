
#ifndef __MakeIcecreamScene__
#define __MakeIcecreamScene__


#include "ExtensionScene.h"
#include "BarrelFreezerNode.h"

class MakeIcecreamScene : public ExtensionScene
{
public:
    MakeIcecreamScene();
    ~MakeIcecreamScene();
    
    MY_SCENE(MakeIcecreamScene);
    CREATE_FUNC(MakeIcecreamScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    typedef enum{
        eFlour,
        eMilk,
        eSugar,
        eFlavor,
        eOil,
        eSalt,
        eEgg,
        eEgg2,
    }IngredientIndex;
protected:
    virtual void _initData();
    
    void _showShadow(DragNode* pDragNode);
    void _hideShadow(DragNode* pDragNode);
//    mix UI
    void _showShelf();
    void _showShelfBoard();
    void _removeShelfBoard();
    void _showIngredient(int step);
    void _showBowl();
    void _addIngredient(IngredientIndex index); //按顺序
    void _finishAdd();
    void _finishMix();
    void _moveawayBatter();
    
//    freeze UI
    void _showFreeze();
    void _showDragBatter();
    void _showPourBatter();
    
//    flavor UI
    
//    Mix Unit
    DragNode* m_pFlavor;
    DragNode* m_pFlour;
    DragNode* m_pOil;
    DragNode* m_pEgg;
    DragNode* m_pEgg2;
    DragNode* m_pSugar;
    DragNode* m_pSalt;
    DragNode* m_pMilk;
    DragNode* m_pWhrisk;
    
    Sprite* m_pBowl;
    Sprite* m_pBowlFront;
    Node* m_pMixture;
    
    Node* m_pShelfNode;
    Sprite* m_pShelf;
    Sprite* m_pBoardBack;
    Sprite* m_pBoard;
    Sprite* m_pSounder;
    Sprite* m_pSounder2;
    
    int m_nAddCount;
    int m_nMixCount;
    bool m_bWhriskIsReady;
    bool m_bReadyAdd;
//    freeze Unit
    BarrelFreezerNode* m_pFreezer;
    
    DragNode* m_pDragBowl;
//    Sprite* m_pDragBowlFront;
//    Sprite* m_pDragMixture;
    
    Sprite* m_pPourBowl;
    Sprite* m_pPourBowlFront;
    Sprite* m_pPourMixture;
    Sprite* m_pPourStream;
};

#endif