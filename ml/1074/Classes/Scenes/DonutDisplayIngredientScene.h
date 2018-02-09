
#ifndef DonutDisplayIngredientScene_h
#define DonutDisplayIngredientScene_h

#include "ExtensionScene.h"

class DonutDisplayIngredientScene : public ExtensionScene
{
public:
    DonutDisplayIngredientScene();
    ~DonutDisplayIngredientScene();
    
    MY_SCENE(DonutDisplayIngredientScene);
    CREATE_FUNC(DonutDisplayIngredientScene);
    
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
#endif /* DonutDisplayIngredientScene_h */
