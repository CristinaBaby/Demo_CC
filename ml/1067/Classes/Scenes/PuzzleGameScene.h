
#ifndef __PuzzleGameScene__
#define __PuzzleGameScene__


#include "ExtensionScene.h"
#include "PuzzleLayerManager.h"

class PuzzleGameScene : public ExtensionScene
{
public:
    PuzzleGameScene();
    ~PuzzleGameScene();
    
    MY_SCENE(PuzzleGameScene);
    CREATE_FUNC(PuzzleGameScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void dragNodeTouchBegin(DragNode* dragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* dragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* dragNode,Point worldPoint);
    void dragNodeClicked(DragNode* dragNode,Point worldPoint);
    
    void onBallcallback(Ref*,Widget::TouchEventType);
    void onPuzzleFinished();
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showPuzzleLayer(int index);
    void _showPuzzleUnits(int index);
    void _ballBomb();
    Node* m_pPuzzleLayer;
    Node* m_pBalls;
    PuzzleLayerManager m_PuzzleManager;
    Vector<DragNode*> m_Units;
    
    
    int m_nBallIndex;
};

#endif