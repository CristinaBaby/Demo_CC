
#ifndef CornflakeCookScene_h
#define CornflakeCookScene_h

#include "ExtensionScene.h"

class CornflakeCookScene : public ExtensionScene
{
public:
    CornflakeCookScene();
    ~CornflakeCookScene();
    
    MY_SCENE(CornflakeCookScene);
    CREATE_FUNC(CornflakeCookScene);
    
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
    void _showSieve();
    void _showShovel();
    void _showPlate();
    void _showIngredient(int index);
    void _addIngredient(DragNode* pNode);
    void _ingredientOut(DragNode* pNode);
    
    void _doFryCornmeal(Sprite* pFood);
    void _finishMix();
    void _finish();
    
    Sprite* m_pBg;
    
    Sprite* m_pStove;
    Sprite* m_pFire;
    Sprite* m_pPan;
    Sprite* m_pPanFront;
    Node* m_pPanIn;
    
    DragNode* m_pSievel;
    DragNode* m_pShovel;
    Sprite* m_pCornmeat;
    Sprite* m_pPlate;
    
    MakeEatLayer* m_pCornLayer;
    
    int m_nStep;
    int m_nMoveCount;
};
#endif /* CornflakeCookScene_h */
