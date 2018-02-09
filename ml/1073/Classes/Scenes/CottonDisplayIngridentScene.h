
#ifndef CottonDisplayIngridentScene_h
#define CottonDisplayIngridentScene_h

#include "ExtensionScene.h"

class CottonDisplayIngridentScene : public ExtensionScene
{
public:
    CottonDisplayIngridentScene();
    ~CottonDisplayIngridentScene();
    
    MY_SCENE(CottonDisplayIngridentScene);
    CREATE_FUNC(CottonDisplayIngridentScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    Sprite* m_pBg;
};
#endif /* CottonDisplayIngridentScene_h */
