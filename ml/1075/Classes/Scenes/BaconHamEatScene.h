
#ifndef BaconHamEatScene_h
#define BaconHamEatScene_h

#include "ShareScene.h"
#include "EatLayer.h"
#include "RuntimePermissionManager.h"

class BaconHamEatScene : public ShareScene
{
public:
    BaconHamEatScene();
    ~BaconHamEatScene();
    
    MY_SCENE(BaconHamEatScene);
    CREATE_FUNC(BaconHamEatScene);
    
    virtual bool init();
    virtual void onEnter();
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(BaconHamEatScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(BaconHamEatScene));}
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    virtual void onNotificationCallback(Ref* ref){};
    
    void onEatBeinCallback(Vec2 location,bool drinkable);
    void onEatEndCallback(Vec2 location);
    
    void onBgCallback(int index);
protected:
    virtual void _initData();
    
    void _showFood();
    
    void _reset();
    Sprite* m_pPlate;
    
    MakeEatLayer* m_pFood;
    
    DragNode* m_pSpoon;
    
    int m_nEatCount;
    std::vector<DragNode*> m_FoodVector;
};
#endif /* BaconHamEatScene_h */
