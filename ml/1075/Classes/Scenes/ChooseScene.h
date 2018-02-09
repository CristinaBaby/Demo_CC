
#ifndef ChooseScene_h
#define ChooseScene_h

#include "ExtensionScene.h"

class ChooseScene : public ExtensionScene
{
public:
    ChooseScene();
    ~ChooseScene();
    
    MY_SCENE(ChooseScene);
    CREATE_FUNC(ChooseScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showScrollView();
    void _onPackageClicked(LockItem* item);
    
    Sprite* m_pBg;
    
    ExpandScrollView* m_pScrollView;
    
    Vector<LockItem*> m_typeItems;
    bool m_bItemEnable;
};
#endif /* ChooseScene_h */
