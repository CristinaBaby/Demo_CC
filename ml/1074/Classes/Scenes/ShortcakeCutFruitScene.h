
#ifndef ShortcakeCutFruitScene_h
#define ShortcakeCutFruitScene_h

#include "ExtensionScene.h"

class ShortcakeCutFruitScene : public ExtensionScene
{
public:
    ShortcakeCutFruitScene();
    ~ShortcakeCutFruitScene();
    
    MY_PHYSIC_SCENE(ShortcakeCutFruitScene);
    CREATE_FUNC(ShortcakeCutFruitScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    void updateStart(float delta);
    void update(float delta);
protected:
    virtual void _initData();
    
    void _showBoard();
    void _showKnife();
    void _showFruitScrollView();
    void showFruit(int index);
    void _showBowl();
    
    void _onFruitTypeClicked(LockItem* item);
    
    void _finishCut();
    void _finish();
    
    Sprite* m_pBg;
    
    Sprite* m_pBoard;
//    Node* m_pFruit;
    Node* m_pSliceNode;
    
    DragNode* m_pKnife;
    
    ExpandScrollView* m_pScrollView;
    Sprite* m_pBanner;
    
    Vector<LockItem*> m_typeItems;
    Vector<Sprite*> m_Fruits;
    Vector<Sprite*> m_FruitsBoard;
    
    Sprite* m_pBowl;
    Sprite* m_pBowlFront;
    ClippingNode* m_pBatterClipping;
    
    bool m_bItemEnable;
    int m_nFlavorIndex;
    int m_nMoveCount;
    int m_nCutCount;
    bool m_bMogoFlag;
};
#endif /* ShortcakeCutFruitScene_h */
