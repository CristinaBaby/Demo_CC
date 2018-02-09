
#ifndef BurgerMixScene_h
#define BurgerMixScene_h

#include "ExtensionScene.h"
#include "TouchLayer.h"

class BurgerMixScene : public ExtensionScene
{
public:
    BurgerMixScene();
    ~BurgerMixScene();
    
    MY_SCENE(BurgerMixScene);
    CREATE_FUNC(BurgerMixScene);
    
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
    
    void _showBowl();
    void _showHand();
    void _finish();
    
    Sprite* m_pBg;
    
    TouchLayer* m_pTouchLayer;
    
    Sprite* m_pBowl;
    Sprite* m_pMixture;
    NodeGrid* m_pBeafGrid;
    Sprite* m_pHand;
    
    int m_nMoveCount;
    int m_nCount;
};
#endif /* BurgerMixScene_h */
