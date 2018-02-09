
#ifndef EggChooseModelScene_h
#define EggChooseModelScene_h

#include "ExtensionScene.h"

class EggChooseModelScene : public ExtensionScene
{
public:
    EggChooseModelScene();
    ~EggChooseModelScene();
    
    MY_SCENE(EggChooseModelScene);
    CREATE_FUNC(EggChooseModelScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _showShapes();
    void _onShapeCallback(LockItem* pItem);
    
    Sprite* m_pBg;
    bool m_bCanTouch ;
    Vector<LockItem*> m_Items;
};
#endif /* EggChooseModelScene_h */
