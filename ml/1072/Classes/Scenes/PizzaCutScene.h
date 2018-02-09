
#ifndef PizzaCutScene_h
#define PizzaCutScene_h

#include "ExtensionScene.h"

class PizzaCutScene : public ExtensionScene
{
public:
    PizzaCutScene();
    ~PizzaCutScene();
    
    MY_SCENE(PizzaCutScene);
    CREATE_FUNC(PizzaCutScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _showPan();
    void _showKnifeTool();
    void _removeKnifeTool();
    void _savePizza();
    void _finishAll();
    
    Sprite* m_pBg;
    
    Sprite* m_pPan;
    Sprite* m_pPanFront;
    DragNode* m_pPizza;
    
    Sprite* m_pPlate;
    Node* m_pFood;
    DragNode* m_pKnife;
    
    int m_nCutStep;
    int m_nMoveCount;
};
#endif /* PizzaCutScene_h */
