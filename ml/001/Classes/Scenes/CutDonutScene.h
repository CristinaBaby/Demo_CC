
#ifndef CutDonutScene_h
#define CutDonutScene_h

#include "ExtensionScene.h"

class CutDonutScene : public ExtensionScene {
    
    
public:
    CutDonutScene();
    ~CutDonutScene();
    
    MY_SCENE(CutDonutScene);
    CREATE_FUNC(CutDonutScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(CutDonutScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(CutDonutScene));}
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    virtual void onButtonCallback(Button* btn);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
protected:
    virtual void _initData();
    void _createRollUI();
    void _pourDough();
    void _showSpreadFlour();
    void _showRollTool();
    void _removeRollTool();
    
    void _finishRoll();
    void _finishCut();
    
    void _showCutTool();
    void _produceCircleGuide();
    void _reset();
private:
    Sprite* m_pBowl;
    Sprite* m_pBowlFront;
    Sprite* m_pDoughPan;
    
    NodeGrid* m_pDoughGrid;
    Sprite* m_pDough;
    Sprite* m_pFlourSprinkle;
    DragNode* m_pRollTool;
    
    int m_nRollCount;
    int m_nRollStep;
    bool m_bRollToolReady;
    
    
    DragNode* m_pCutTool;
    std::vector<Sprite*> m_pGuideVector;
    std::vector<Sprite*> m_pCircleVector;
    std::vector<Sprite*> m_pCircleFinishVector;
    int m_nCutCount;
};
#endif /* CutDonutScene_h */
