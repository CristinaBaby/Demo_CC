
#ifndef CornflakeEatScene_h
#define CornflakeEatScene_h

#include "ShareScene.h"
#include "EatLayer.h"
#include "RuntimePermissionManager.h"

class CornflakeEatScene : public ShareScene
{
public:
    CornflakeEatScene();
    ~CornflakeEatScene();
    
    MY_SCENE(CornflakeEatScene);
    CREATE_FUNC(CornflakeEatScene);
    
    virtual bool init();
    virtual void onEnter();
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(CornflakeEatScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(CornflakeEatScene));}
    
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
    void _showSpoon();
    
    void _reset();
    Sprite* m_pPlate;
    Sprite* m_pPanFront;
    MakeEatLayer* m_pFood;
    
    DragNode* m_pSpoon;
    
    int m_nEatCount;
};
#endif /* CornflakeEatScene_h */
