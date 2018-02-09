
#ifndef SnowconeShapScene_h
#define SnowconeShapScene_h

#include "ExtensionScene.h"
#include "ItemScrollView.h"

class SnowconeShapScene : public ExtensionScene
{
public:
    SnowconeShapScene();
    ~SnowconeShapScene();
    
    MY_SCENE(SnowconeShapScene);
    CREATE_FUNC(SnowconeShapScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showShapeScrollView();
    void _onItemSelected(int index,int type,DecorateConfigData decData);
    
    Sprite* m_pBg;
    
    ExpandScrollView* m_pShapeScroll;
    ItemScrollView* m_pItemScrollView;
    bool m_bSelected;
};
#endif /* SnowconeShapScene_h */
