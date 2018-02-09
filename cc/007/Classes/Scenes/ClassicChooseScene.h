
#ifndef ClassicChooseScene_h
#define ClassicChooseScene_h

#include "ExtensionScene.h"

class ClassicChooseScene : public ExtensionScene
{
public:
    ClassicChooseScene();
    ~ClassicChooseScene();
    
    MY_SCENE(ClassicChooseScene);
    CREATE_FUNC(ClassicChooseScene);
    
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
#endif /* ClassicChooseScene_h */
