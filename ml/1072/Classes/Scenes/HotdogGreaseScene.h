
#ifndef HotdogGreaseScene_h
#define HotdogGreaseScene_h

#include "ExtensionScene.h"
#include "TouchLayer.h"

class HotdogGreaseScene : public ExtensionScene
{
public:
    HotdogGreaseScene();
    ~HotdogGreaseScene();
    
    MY_SCENE(HotdogGreaseScene);
    CREATE_FUNC(HotdogGreaseScene);
    
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
    
    void _showPan();
    void _showOil();
    
    void _showPlateLeft();
    void _showPlateRight();
    
    void _finishAddOil();
    void _finishAll();
    
    Sprite* m_pBg;
    TouchLayer* m_pTouchLayer;
    
    Sprite* m_pPan;
    Sprite* m_pOil;
    
    Sprite* m_pOilBowl;
    DragNode* m_pOilSpoon;
    
    Sprite* m_pPlateLeft;
    Sprite* m_pPlateRight;
    
    std::vector<DragNode*> m_SausageVector;
    DragNode* m_pCurSausage;
    NodeGrid* m_pSausageGrid;
    
    int m_nAddOilCount;
    int m_nMoveCount;
    int m_nFinishCount;
};
#endif /* HotdogGreaseScene_h */
