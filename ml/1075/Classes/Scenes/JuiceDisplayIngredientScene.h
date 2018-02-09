
#ifndef JuiceDisplayIngredientScene_h
#define JuiceDisplayIngredientScene_h

#include "ExtensionScene.h"

class JuiceDisplayIngredientScene : public ExtensionScene
{
public:
    JuiceDisplayIngredientScene();
    ~JuiceDisplayIngredientScene();
    
    MY_SCENE(JuiceDisplayIngredientScene);
    CREATE_FUNC(JuiceDisplayIngredientScene);
    
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
#endif /* JuiceDisplayIngredientScene_h */
