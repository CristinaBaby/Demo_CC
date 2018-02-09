
#ifndef GetGiftScene_h
#define GetGiftScene_h

#include "ExtensionScene.h"
#include "CandlePosition.h"
#include "CandleNode.h"

class GetGiftScene : public ExtensionScene
{
public:
    GetGiftScene();
    ~GetGiftScene();
    
    MY_SCENE(GetGiftScene);
    CREATE_FUNC(GetGiftScene);
    virtual bool init();
    
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
protected:
    virtual void _initData();
    
    void _lightCandle();
    void _dieoutCandle();
    
    Layer* m_pCandleLayer;
    Sprite* m_pDecoratedCake;
    DragNode* m_pTool;
    Vector<CandleNode*> m_VectorCandle;
    bool m_bFired;
    bool m_bDark;
};
#endif /* GetGiftScene_h */
