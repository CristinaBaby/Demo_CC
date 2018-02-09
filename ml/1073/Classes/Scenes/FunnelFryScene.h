
#ifndef FunnelFryScene_h
#define FunnelFryScene_h

#include "ExtensionScene.h"
#include "ScribbleSauce.h"

class FunnelFryScene : public ExtensionScene
{
public:
    FunnelFryScene();
    ~FunnelFryScene();
    
    MY_SCENE(FunnelFryScene);
    CREATE_FUNC(FunnelFryScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    void update(float);
protected:
    virtual void _initData();
    
    void _update(float);
    
    void _showPan();
    void _showOilCup();
    void _showFunnel();
    void _showThermomter();
    void _showFunnelStream();
    void _showClip();
    
    void _pourOil();
    void _pourCake();
    void _doFry();
    void _onProgressCallback(ProgressBarNew2* pBar,int segment);
    void _finish();
    
    void _reset();
    
    Sprite* m_pBg;
    
    ProgressBarNew2* m_pBar;
    Sprite* m_pTimeBg;
    ProgressTimer* m_pProgressTime;
    
    DragNode* m_pClip;
    
    Sprite* m_pFire;
    Sprite* m_pPan;
    Sprite* m_pPanFront;
    Sprite* m_pOilInPan;
    DragNode* m_pOilCup;
    
    DragNode* m_pFunnel;
    Sprite* m_pBatter;
    Sprite* m_pStream;
    DragNode* m_pHand;
    
    Sprite* m_pCake;
    
    ScribbleSauce* m_pSauce;
    
    bool m_bFunnelMoved;
    int m_nPercent;
};
#endif /* FunnelFryScene_h */
