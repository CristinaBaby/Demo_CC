
#ifndef CornflakeAddMilkScene_h
#define CornflakeAddMilkScene_h

#include "ExtensionScene.h"

class CornflakeAddMilkScene : public ExtensionScene
{
public:
    CornflakeAddMilkScene();
    ~CornflakeAddMilkScene();
    
    MY_SCENE(CornflakeAddMilkScene);
    CREATE_FUNC(CornflakeAddMilkScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showBowl();
    void _showSievel();
    void _showMilk();
    void _pourSievel();
    void _pourMilk();
    void _finish();
    
    Sprite* m_pBg;
    
    
    Sprite* m_pBowl;
    Sprite* m_pBowlFront;
    Node* m_pMixture;
    
    DragNode* m_pSievel;
    DragNode* m_pMilk;
};
#endif /* CornflakeAddMilkScene_h */
