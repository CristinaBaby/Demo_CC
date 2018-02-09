
#ifndef SnowconeSculptScene_h
#define SnowconeSculptScene_h

#include "ExtensionScene.h"

class SnowconeSculptScene : public ExtensionScene
{
public:
    SnowconeSculptScene();
    ~SnowconeSculptScene();
    
    MY_SCENE(SnowconeSculptScene);
    CREATE_FUNC(SnowconeSculptScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showCone();
    void _showKnife();
    void _finish();
    
    Sprite* m_pBg;
    
    DragNode* m_pKnife;
    
    Node* m_pSnowConeNode;
    Sprite* m_pCone;
    Sprite* m_pConeFront;
    Sprite* m_pConeIceShape;
    Sprite* m_pConeIce;
    Sprite* m_pConeIceTemp;
    Sprite* m_pLine;
    
    int m_nShapeIndex;
    bool m_bBeginLeft;
    int m_nMoveCount;
};
#endif /* SnowconeSculptScene_h */
