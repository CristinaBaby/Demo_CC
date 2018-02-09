
#ifndef __GetIcecreamScene__
#define __GetIcecreamScene__


#include "ExtensionScene.h"

class GetIcecreamScene : public ExtensionScene
{
public:
    GetIcecreamScene();
    ~GetIcecreamScene();
    
    MY_SCENE(GetIcecreamScene);
    CREATE_FUNC(GetIcecreamScene);
    
    virtual bool init();
    void onItemTouchBegin(LockItem*);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    RenderTexture* getResultRender();
    void onButtonCallback(Button* btn);
    void resetIcecreamBall();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
protected:
    virtual void _initData();
    
    void _showShelf();
    void _showScrollView();
    void _showCone();

    ExpandScrollView* m_pScrollView;
    Sprite* m_pCone;
    Node* m_pIcecreamNode0;
    Node* m_pIcecreamNode;
    Vector<DragNode*> m_IceCreamIcons;
    Vector<Sprite*> m_Icecreams;
    
    Node* m_pShelfNode;
    Sprite* m_pShelf;
    Sprite* m_pSounder;
    Sprite* m_pSounder2;
    
};

#endif