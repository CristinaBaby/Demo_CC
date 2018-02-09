
#ifndef PizzaBakeScene_h
#define PizzaBakeScene_h

#include "ExtensionScene.h"
#include "OvenNode.h"

class PizzaBakeScene : public ExtensionScene
{
public:
    PizzaBakeScene();
    ~PizzaBakeScene();
    
    MY_SCENE(PizzaBakeScene);
    CREATE_FUNC(PizzaBakeScene);
    
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
    void _doBake();
    void _showGlove();
    void _finishAll();
    Sprite* m_pBg;
    
    DragNode* m_pPan;
    Node* m_pFood;
    DragNode* m_pGlove;
    
    OvenNode* m_pOven;
};
#endif /* PizzaBakeScene_h */
