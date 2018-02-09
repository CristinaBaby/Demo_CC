
#ifndef DonutBakeScene_h
#define DonutBakeScene_h

#include "ExtensionScene.h"
#include "OvenNode.h"

class DonutBakeScene : public ExtensionScene
{
public:
    DonutBakeScene();
    ~DonutBakeScene();
    
    MY_SCENE(DonutBakeScene);
    CREATE_FUNC(DonutBakeScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _showModel();
    void _doBake();
    void _showGlove();
    void _addBatter(int flavorIndex,int maskIndex,bool baked = false);
    void _finishAll();
    
    Sprite* m_pBg;
    
    DragNode* m_pModel;
    Node* m_pFood;
    DragNode* m_pGlove;
    
    OvenNode* m_pOven;
    int m_nShapeIndex;
};
#endif /* DonutBakeScene_h */
