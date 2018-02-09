
#ifndef CottonChooseShapeScene_h
#define CottonChooseShapeScene_h

#include "ExtensionScene.h"

class CottonChooseShapeScene : public ExtensionScene
{
public:
    CottonChooseShapeScene();
    ~CottonChooseShapeScene();
    
    MY_SCENE(CottonChooseShapeScene);
    CREATE_FUNC(CottonChooseShapeScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showShapeUI();
    void _showCottonCandy();
    void _onShapeClicked(LockItem* item);
    
    Sprite* m_pBg;
    
    Vector<LockItem*> m_ShapeItems;
    ExpandScrollView* m_pScrollView;
    Sprite* m_pRope;
    Sprite* m_pStick;
    Sprite* m_pCotton;
    
    int m_nFlavor;
    bool m_bItemEnable;
};
#endif /* CottonChooseShapeScene_h */
