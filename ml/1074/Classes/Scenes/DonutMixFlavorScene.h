
#ifndef DonutMixFlavorScene_h
#define DonutMixFlavorScene_h

#include "ExtensionScene.h"

class DonutMixFlavorScene : public ExtensionScene
{
public:
    DonutMixFlavorScene();
    ~DonutMixFlavorScene();
    
    MY_SCENE(DonutMixFlavorScene);
    CREATE_FUNC(DonutMixFlavorScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    void onAcceleration(Acceleration *, Event *);
protected:
    virtual void _initData();
    
    void _showBowls();
    void _showBowl(int index);
    void _showMixTool();
    void _showFlavorScrollView();
    void _showBottle(int index);
    void _setMix();
    void _showProgressStar();
    
    void _addFlavor();
    void _onTypeClicked(LockItem* item);
    
    void _finishMix();
    void _finish();
    
    Sprite* m_pBg;
    
    Sprite* m_pBowl;
    Sprite* m_pBowlFront;
    Node* m_pBatter;
    ExpandScrollView* m_pScrollView;
    Sprite* m_pBanner;
    ProgressStar*m_pProgressStar;
    
    Vector<LockItem*> m_typeItems;
    
    DragNode* m_pBottle;
    
    DragNode* m_pMixTool;
    
    std::vector<Node*> m_smallBowls;
    
    int m_nFlavorIndex;
    int m_nFinishCount;
    bool m_bItemEnable;
};
#endif /* DonutMixFlavorScene_h */
