
#ifndef AnimationScene_h
#define AnimationScene_h

#include "ExtensionScene.h"

class AnimationScene : public ExtensionScene
{
public:
    AnimationScene();
    ~AnimationScene();
    
    MY_SCENE(AnimationScene);
    CREATE_FUNC(AnimationScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onButtonCallback(Button* btn);
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    Sprite* m_pBg;
    Armature* m_pRole;
};
#endif /* AnimationScene_h */
