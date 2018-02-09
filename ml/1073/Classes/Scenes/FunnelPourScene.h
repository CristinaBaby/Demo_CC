
#ifndef FunnelPourScene_h
#define FunnelPourScene_h

#include "ExtensionScene.h"

class FunnelPourScene : public ExtensionScene
{
public:
    FunnelPourScene();
    ~FunnelPourScene();
    
    MY_SCENE(FunnelPourScene);
    CREATE_FUNC(FunnelPourScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showFunnel();
    void _showBowl();
    void _showHand();
    
    void _pourBatter();
    void _finish();
    
    Sprite* m_pBg;
    
    DragNode* m_pBowl;
    
    DragNode* m_pFunnel;
    Sprite* m_pFunnelFront;
    Sprite* m_pBatter;
    DragNode* m_pHand;
};
#endif /* FunnelPourScene_h */
