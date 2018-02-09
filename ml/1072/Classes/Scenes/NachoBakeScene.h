
#ifndef NachoBakeScene_h
#define NachoBakeScene_h

#include "ExtensionScene.h"
#include "OvenNode.h"

class NachoBakeScene : public ExtensionScene
{
public:
    NachoBakeScene();
    ~NachoBakeScene();
    
    MY_SCENE(NachoBakeScene);
    CREATE_FUNC(NachoBakeScene);
    
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
    
    void _saveNachos();
    void _finish();
    
    Sprite* m_pBg;
    
    DragNode* m_pPan;
    Node* m_pFood;
    DragNode* m_pGlove;
    Sprite* m_pNachos;
    
    OvenNode* m_pOven;
};
#endif /* NachoBakeScene_h */
