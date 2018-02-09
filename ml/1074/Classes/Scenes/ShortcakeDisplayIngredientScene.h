
#ifndef ShortcakeDisplayIngredientScene_h
#define ShortcakeDisplayIngredientScene_h

#include "ExtensionScene.h"

class ShortcakeDisplayIngredientScene : public ExtensionScene
{
public:
    ShortcakeDisplayIngredientScene();
    ~ShortcakeDisplayIngredientScene();
    
    MY_SCENE(ShortcakeDisplayIngredientScene);
    CREATE_FUNC(ShortcakeDisplayIngredientScene);
    
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
#endif /* ShortcakeDisplayIngredientScene_h */
