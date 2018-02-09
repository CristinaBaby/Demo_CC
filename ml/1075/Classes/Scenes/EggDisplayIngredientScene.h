
#ifndef EggDisplayIngredientScene_h
#define EggDisplayIngredientScene_h

#include "ExtensionScene.h"

class EggDisplayIngredientScene : public ExtensionScene
{
public:
    EggDisplayIngredientScene();
    ~EggDisplayIngredientScene();
    
    MY_SCENE(EggDisplayIngredientScene);
    CREATE_FUNC(EggDisplayIngredientScene);
    
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
#endif /* EggDisplayIngredientScene_h */
