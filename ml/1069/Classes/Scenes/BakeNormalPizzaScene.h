
#ifndef __BakeNormalPizzaScene__
#define __BakeNormalPizzaScene__


#include "ExtensionScene.h"

class BakeNormalPizzaScene : public ExtensionScene
{
public:
    BakeNormalPizzaScene();
    ~BakeNormalPizzaScene();
    
    MY_SCENE(BakeNormalPizzaScene);
    CREATE_FUNC(BakeNormalPizzaScene);
    
    virtual bool init();
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(BakeNormalPizzaScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(BakeNormalPizzaScene));}
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
protected:
    virtual void _initData();
    
    void _doBake(float dt = 0);
    void _finishBake();
    void _showPlate();
    
    DragNode* m_pSholve;
    Node* m_pPizzaNode;
    Node* m_pPizzaRipeNode;
    Sprite* m_pPie;
    Sprite* m_pCheese;
    Sprite* m_pDecoration;
    Sprite* m_pPlate;
    
    ClippingNode*m_pDoorClipping;
    
    Sprite* m_pOven;
    Sprite* m_pOvenFront;
    bool m_bSholveReady;
    int m_nBakeCount;
    bool m_bBaked;
    bool m_bPlateReady;
    Size m_pieSize;
};

#endif