
#ifndef JuiceChooseFlavorScene_h
#define JuiceChooseFlavorScene_h

#include "ExtensionScene.h"

class JuiceChooseFlavorScene : public ExtensionScene
{
public:
    JuiceChooseFlavorScene();
    ~JuiceChooseFlavorScene();
    
    MY_SCENE(JuiceChooseFlavorScene);
    CREATE_FUNC(JuiceChooseFlavorScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _showFlavors();
    void _onItemCallback(LockItem* pItem);
    
    Sprite* m_pBg;
    
    Vector<LockItem*> m_Items;
    bool m_bSelected;
};
#endif /* JuiceChooseFlavorScene_h */
