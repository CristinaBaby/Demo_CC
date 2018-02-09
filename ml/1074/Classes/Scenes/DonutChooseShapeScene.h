
#ifndef DonutChooseShapeScene_h
#define DonutChooseShapeScene_h

#include "ExtensionScene.h"

class DonutChooseShapeScene : public ExtensionScene
{
public:
    DonutChooseShapeScene();
    ~DonutChooseShapeScene();
    
    MY_SCENE(DonutChooseShapeScene);
    CREATE_FUNC(DonutChooseShapeScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _onItemCallback(LockItem*);
    
    Sprite* m_pBg;
    Vector<LockItem*> m_Items;
    bool m_bSelected;
};
#endif /* DonutChooseShapeScene_h */
