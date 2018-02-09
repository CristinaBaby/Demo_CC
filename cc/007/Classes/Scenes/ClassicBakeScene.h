
#ifndef ClassicBakeScene_h
#define ClassicBakeScene_h

#include "ExtensionScene.h"

class ClassicBakeScene : public ExtensionScene
{
public:
    ClassicBakeScene();
    ~ClassicBakeScene();
    
    MY_SCENE(ClassicBakeScene);
    CREATE_FUNC(ClassicBakeScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showProgressBar();
    void _showBar(bool show = true);
    void _showCookie();
    void _finish();
    
    Sprite* m_pBg;
    DragNode* m_pTray;
    
    Vector<Sprite*> m_CookieVector;
    OvenNode* m_pOven;
    ProgressBar* m_pBar;
    
    int m_nFlavor;
    int m_nShape;
};
#endif /* ClassicBakeScene_h */
