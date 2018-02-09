
#ifndef ShortcakeAddFrostingScene_h
#define ShortcakeAddFrostingScene_h

#include "ExtensionScene.h"
#include "ScribbleNode.h"

class ShortcakeAddFrostingScene : public ExtensionScene
{
public:
    ShortcakeAddFrostingScene();
    ~ShortcakeAddFrostingScene();
    
    MY_SCENE(ShortcakeAddFrostingScene);
    CREATE_FUNC(ShortcakeAddFrostingScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showFood();
    void _showSpoon();
    void _showFrosting();
    
    void _finish();
    Sprite* m_pBg;
    
    Sprite* m_pBowl;
    Sprite* m_pBowlFront;
    ClippingNode* m_pCreamClipping;
    Sprite* m_pCream;
    
    DragNode* m_pSpoon;
    ScribbleNode* m_pFrostingScribble;
    
    Sprite* m_pPlate;
    Node* m_pFood;
    int m_nFrostingIndex;
    int m_nCakeCount;
    int m_nMoveCount;
//    std::vector<Sprite*> m_cake;
};
#endif /* ShortcakeAddFrostingScene_h */
