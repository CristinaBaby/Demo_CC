
#ifndef OmeletteDisplayIngredientScene_h
#define OmeletteDisplayIngredientScene_h

#include "ExtensionScene.h"

class OmeletteDisplayIngredientScene : public ExtensionScene
{
public:
    OmeletteDisplayIngredientScene();
    ~OmeletteDisplayIngredientScene();
    
    MY_SCENE(OmeletteDisplayIngredientScene);
    CREATE_FUNC(OmeletteDisplayIngredientScene);
    
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
#endif /* OmeletteDisplayIngredientScene_h */
