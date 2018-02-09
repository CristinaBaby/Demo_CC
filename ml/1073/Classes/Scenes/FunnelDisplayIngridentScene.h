
#ifndef FunnelDisplayIngridentScene_h
#define FunnelDisplayIngridentScene_h

#include "ExtensionScene.h"

class FunnelDisplayIngridentScene : public ExtensionScene
{
public:
    FunnelDisplayIngridentScene();
    ~FunnelDisplayIngridentScene();
    
    MY_SCENE(FunnelDisplayIngridentScene);
    CREATE_FUNC(FunnelDisplayIngridentScene);
    
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
#endif /* FunnelDisplayIngridentScene_h */
