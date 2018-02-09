
#ifndef CornflakeDisplayIngredientScene_h
#define CornflakeDisplayIngredientScene_h

#include "ExtensionScene.h"

class CornflakeDisplayIngredientScene : public ExtensionScene
{
public:
    CornflakeDisplayIngredientScene();
    ~CornflakeDisplayIngredientScene();
    
    MY_SCENE(CornflakeDisplayIngredientScene);
    CREATE_FUNC(CornflakeDisplayIngredientScene);
    
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
#endif /* CornflakeDisplayIngredientScene_h */
