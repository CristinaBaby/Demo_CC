
#ifndef JuiceCutFruitScene_h
#define JuiceCutFruitScene_h

#include "ExtensionScene.h"

class JuiceCutFruitScene : public ExtensionScene
{
public:
    JuiceCutFruitScene();
    ~JuiceCutFruitScene();
    
    MY_PHYSIC_SCENE(JuiceCutFruitScene);
    CREATE_FUNC(JuiceCutFruitScene);
    
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
    void _showFruit(int index);
    void _showBowl();
    
    void _finishCut();
    void _finish();
    
    Sprite* m_pBg;
    
    Sprite* m_pBoard;
    //    Node* m_pFruit;
    Node* m_pSliceNode;
    
    DragNode* m_pKnife;
    
    Vector<Sprite*> m_Fruits;
    Vector<Sprite*> m_FruitsBoard;
    
    Sprite* m_pBowl;
    Sprite* m_pBowlFront;
    ClippingNode* m_pBatterClipping;
    
    bool m_bItemEnable;
    int m_nFlavorIndex;
    int m_nMoveCount;
    int m_nCutCount;
};
#endif /* JuiceCutFruitScene_h */
