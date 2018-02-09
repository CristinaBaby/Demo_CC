
#ifndef __BakeScene__
#define __BakeScene__


#include "ExtensionScene.h"
#include "OvenNode.h"

class BakeScene : public ExtensionScene
{
public:
    BakeScene();
    ~BakeScene();
    
    MY_SCENE(BakeScene);
    CREATE_FUNC(BakeScene);
    
    virtual bool init();
    
    virtual void dragNodeTouchBegin(DragNode* dragNode,Point worldPoint);
    virtual void dragNodeTouchMoved(DragNode* dragNode,Point worldPoint);
    virtual void dragNodeTouchEnded(DragNode* dragNode,Point worldPoint);
    virtual void dragNodeClicked(DragNode* dragNode,Point worldPoint);
    void onOvenStateCallback(OvenNode::State state);
protected:
    virtual void _initData();
    
    void _resumeBake();
    void _pauseBake();
    void _beginBakeDough();
    void _doBakeDough(float dt);
    void _resetBar();
    void _showBar();
    void _hideBar();
    
    void _showBowl();
    
    ProgressBar* m_pBar;
    GuideLayer *m_pGuideLayer2;
    
    OvenNode* m_pOven;
    DragNode* m_pBowl;
    Sprite* m_pBowlInOven;
    bool m_bBowlShown;
    bool m_bCakeReady;
    int m_nBakeCount;
};

#endif