
#ifndef SnowconeColorScene_h
#define SnowconeColorScene_h

#include "ExtensionScene.h"

class SnowconeColorScene : public ExtensionScene
{
public:
    SnowconeColorScene();
    ~SnowconeColorScene();
    
    MY_SCENE(SnowconeColorScene);
    CREATE_FUNC(SnowconeColorScene);
    
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
#endif /* SnowconeColorScene_h */
