
#ifndef ShortcakeBakeScene_h
#define ShortcakeBakeScene_h

#include "ExtensionScene.h"
#include "OvenNode.h"

class ShortcakeBakeScene : public ExtensionScene
{
public:
    ShortcakeBakeScene();
    ~ShortcakeBakeScene();
    
    MY_SCENE(ShortcakeBakeScene);
    CREATE_FUNC(ShortcakeBakeScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _showModel();
    void _doBake();
    void _showGlove();
    void _finishAll();
    
    Sprite* m_pBg;
    
    DragNode* m_pModel;
    Node* m_pFood;
    Sprite* m_pBatter;
    DragNode* m_pGlove;
    
    OvenNode* m_pOven;
    int m_nFlavorIndex;
};
#endif /* ShortcakeBakeScene_h */
