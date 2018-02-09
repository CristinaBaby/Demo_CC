
#ifndef CupcakeAddFrostingScene_h
#define CupcakeAddFrostingScene_h

#include "ExtensionScene.h"
#include "ScribbleNode.h"

class CupcakeAddFrostingScene : public ExtensionScene
{
public:
    CupcakeAddFrostingScene();
    ~CupcakeAddFrostingScene();
    
    MY_SCENE(CupcakeAddFrostingScene);
    CREATE_FUNC(CupcakeAddFrostingScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showTray();
    void _addBatter(Node* pParent,int flavorIndex,int maskIndex);
    void _showChooseCupcakes();
    void _showCupcake();
    void _showFrostingScrollview();
    void _showFrostingBag(int index);
    void _showCandies();
    
    void _onCupcakeSelectCallback(LockItem* pItem);
    void _onFrostingClicked(LockItem* item);
    void _saveCupcake();
    void _finish();
    
    Sprite* m_pBg;
    
    Sprite* m_pTray;
    std::vector<LockItem*> m_cakeVector;
    
    ExpandScrollView* m_pScrollView;
    Vector<LockItem*> m_typeItems;
    LockItem* m_pCurCupcake;
    bool m_bItemEnable;
    DragNode* m_pFrostingBag;
    ScribbleNode* m_pFrostingScribble;
    
    Sprite* m_pCottonCandyPlate;
    Sprite* m_pRainbowCandyPlate;
    DragNode* m_pCottonCandy0;
    DragNode* m_pCottonCandy1;
    DragNode* m_pRainbowCandy;
    Node* m_pDecNode;
    
    int m_nFrostingIndex;
    int m_nMoveCount;
    int m_nAddCount;
};
#endif /* CupcakeAddFrostingScene_h */
