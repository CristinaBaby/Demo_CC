
#ifndef PizzaEatScene_h
#define PizzaEatScene_h

#include "ShareBaseScene.h"

class PizzaEatScene : public ShareBaseScene
{
public:
    PizzaEatScene();
    ~PizzaEatScene();
    
    MY_SCENE(PizzaEatScene);
    CREATE_FUNC(PizzaEatScene);
    
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
    void _showPizza(bool animation = true);
    Sprite* m_pBg;
    
    EatLayer* m_pEatLayer;
    Layer* m_pDecoLayer;
    DecorateManager* m_pDecManager;
    Sprite* m_pPlate;
};
#endif /* PizzaEatScene_h */
