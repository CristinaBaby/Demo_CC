
#ifndef BurgerEatScene_h
#define BurgerEatScene_h

#include "ShareBaseScene.h"

class BurgerEatScene : public ShareBaseScene
{
public:
    BurgerEatScene();
    ~BurgerEatScene();
    
    MY_SCENE(BurgerEatScene);
    CREATE_FUNC(BurgerEatScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    
    void onEatBeinCallback(Vec2 location,bool drinkable);
    void onEatEndCallback(Vec2 location);
protected:
    virtual void _initData();
    void _reset();
    
    void _showPlate();
    void _showHotdog(bool animation = true);
    Sprite* m_pBg;
    
    EatLayer* m_pEatLayer;
    Layer* m_pDecoLayer;
    DecorateManager* m_pDecManager;
    Sprite* m_pPlate;
};
#endif /* BurgerEatScene_h */
