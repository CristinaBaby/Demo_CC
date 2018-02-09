
#ifndef DonutPourBatterScene_h
#define DonutPourBatterScene_h

#include "ExtensionScene.h"

class DonutPourBatterScene : public ExtensionScene
{
public:
    DonutPourBatterScene();
    ~DonutPourBatterScene();
    
    MY_SCENE(DonutPourBatterScene);
    CREATE_FUNC(DonutPourBatterScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showBowls();
    void _finish();
    
    Sprite* m_pBg;
    
    DragNode* m_pBigBowl;
    
    std::vector<Node*> m_smallBowls;
};
#endif /* DonutPourBatterScene_h */
