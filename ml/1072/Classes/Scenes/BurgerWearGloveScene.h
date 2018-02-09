
#ifndef BurgerWearGloveScene_h
#define BurgerWearGloveScene_h

#include "ExtensionScene.h"
#include "TouchLayer.h"

class BurgerWearGloveScene : public ExtensionScene
{
public:
    BurgerWearGloveScene();
    ~BurgerWearGloveScene();
    
    MY_SCENE(BurgerWearGloveScene);
    CREATE_FUNC(BurgerWearGloveScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    void TouchBegan(Touch *pTouch, Event *pEvent);
    void TouchMove(Touch *pTouch, Event *pEvent);
    void TouchEnded(Touch *pTouch, Event *pEvent);
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _finish();
    
    Sprite* m_pBg;
    TouchLayer* m_pTouchLayer;
    
    Sprite* m_pHand;
    Sprite* m_pGloveBack;
    DragNode* m_pGlove;
    int m_nMoveCount;
    int m_nStep;
};
#endif /* BurgerWearGloveScene_h */
