
#ifndef CupcakePourBatterScene_h
#define CupcakePourBatterScene_h

#include "ExtensionScene.h"

class CupcakePourBatterScene : public ExtensionScene
{
public:
    CupcakePourBatterScene();
    ~CupcakePourBatterScene();
    
    MY_SCENE(CupcakePourBatterScene);
    CREATE_FUNC(CupcakePourBatterScene);
    
    virtual bool init();
    virtual void onEnter();
    
    virtual void onButtonCallback(Button* btn);
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _showTray();
    
    void _showCupScrollView();
    void _showBags();
    void _onCupClicked(LockItem* item);
    void _updateCups(int index);
    void _addBatter(int index,int flavor);
    
    void _finish();
    
    Sprite* m_pBg;
    Sprite* m_pTray;
    
    
    ExpandScrollView* m_pScrollView;
    
    Vector<LockItem*> m_typeItems;
    Vector<Sprite*> m_cupItems;
    Vector<DragNode*> m_bagItems;
    Vector<ClippingNode*> m_batterItems;
    
    DragNode* m_pCurBag;
    bool m_bItemEnable;
    int m_nCupIndex;
    int m_nStep;
    int m_CupFlavor[12]; //4*3
};
#endif /* CupcakePourBatterScene_h */
