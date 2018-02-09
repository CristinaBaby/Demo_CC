
#ifndef CupcakeDisplayIngredientScene_h
#define CupcakeDisplayIngredientScene_h

#include "ExtensionScene.h"

class CupcakeDisplayIngredientScene : public ExtensionScene
{
public:
    CupcakeDisplayIngredientScene();
    ~CupcakeDisplayIngredientScene();
    
    MY_SCENE(CupcakeDisplayIngredientScene);
    CREATE_FUNC(CupcakeDisplayIngredientScene);
    
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
#endif /* CupcakeDisplayIngredientScene_h */
