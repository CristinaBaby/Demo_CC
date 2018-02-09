
#ifndef ShortcakePourBatterScene_h
#define ShortcakePourBatterScene_h

#include "ExtensionScene.h"
#include "ScribbleNode.h"

class ShortcakePourBatterScene : public ExtensionScene
{
public:
    ShortcakePourBatterScene();
    ~ShortcakePourBatterScene();
    
    MY_SCENE(ShortcakePourBatterScene);
    CREATE_FUNC(ShortcakePourBatterScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _showPan();
    void _showButters();
    void _showBowl();
    
    void _paintButter();
    void _finishPaintButter();
    void _finish();
    
    Sprite* m_pBg;
    
    Sprite* m_pPan;
    Sprite* m_pPanFront;
    Node* m_pMixture;
//    Sprite* m_pButterPan;
    ScribbleNode* m_pButterScribble;
    
    Sprite* m_pButterPlate;
    DragNode* m_pButter;
    DragNode* m_pButterBottle;
    DragNode* m_pCurButter;
    DragNode* m_pBowl;
    int m_nFlavorIndex;
    int m_nMoveCount;
};
#endif /* ShortcakePourBatterScene_h */
