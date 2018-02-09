
#ifndef ChooseScene_h
#define ChooseScene_h

#include "ExtensionScene.h"
#include "HomeCart.h"

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
    void _onItemButtonCallback(Ref* ref,Widget::TouchEventType type);
    void _onCartCallback(HomeCart* cart);
    
    void _onItemCallback(int tag);
    
    Sprite* m_pBg;
    bool m_bChoosen;
    
    std::vector<Node*> m_pCarVector;
};
#endif /* ChooseScene_h */
