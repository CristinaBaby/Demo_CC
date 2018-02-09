
#ifndef BurgerRollBeafScene_h
#define BurgerRollBeafScene_h

#include "ExtensionScene.h"
#include "TouchLayer.h"

class BurgerRollBeafScene : public ExtensionScene
{
public:
    BurgerRollBeafScene();
    ~BurgerRollBeafScene();
    
    MY_SCENE(BurgerRollBeafScene);
    CREATE_FUNC(BurgerRollBeafScene);
    
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
    void _showBoard();
    void _finishRoll();
    void _finish();
    
    Sprite* m_pBg;
    
    TouchLayer* m_pTouchLayer;
    
    Sprite* m_pBowl;
    Sprite* m_pMixture;
    
    Sprite* m_pBoard;
    DragNode* m_pBeaf;
    
    NodeGrid* m_pBeafGrid;
    Sprite* m_pRollBeaf;
    
    int m_nMoveCount;
    int m_nFinishCount;
    int m_nRollCount;
};
#endif /* BurgerRollBeafScene_h */
