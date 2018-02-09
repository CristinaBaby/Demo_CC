
#ifndef BaconHamDisplayIngredientScene_h
#define BaconHamDisplayIngredientScene_h

#include "ExtensionScene.h"

class BaconHamDisplayIngredientScene : public ExtensionScene
{
public:
    BaconHamDisplayIngredientScene();
    ~BaconHamDisplayIngredientScene();
    
    MY_SCENE(BaconHamDisplayIngredientScene);
    CREATE_FUNC(BaconHamDisplayIngredientScene);
    
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
#endif /* BaconHamDisplayIngredientScene_h */
