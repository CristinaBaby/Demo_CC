
#ifndef TwistRubDoughScene_h
#define TwistRubDoughScene_h

#include "ExtensionScene.h"
#include "TouchLayer.h"

class TwistRubDoughScene : public ExtensionScene {
    
    
public:
    TwistRubDoughScene();
    ~TwistRubDoughScene();
    
    MY_SCENE(TwistRubDoughScene);
    CREATE_FUNC(TwistRubDoughScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(TwistRubDoughScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(TwistRubDoughScene));}
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    virtual void onButtonCallback(Button* btn);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    void TouchBegan(Touch *pTouch, Event *pEvent);
    void TouchMove(Touch *pTouch, Event *pEvent);
    void TouchEnded(Touch *pTouch, Event *pEvent);
protected:
    virtual void _initData();
    void _createRollUI();
    void _pourDough();
    void _showSpreadFlour();
    void _showRollTool();
    void _removeRollTool();
    
    void _produceCircleGuide();
    
    void _finishRoll();
    void _finishCut();
    void _finishRub();
    
    void _showCutTool();
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
    std::vector<DragNode*> m_CircleFinishVector;
    std::vector<Sprite*> m_CircleVector;
    std::vector<Sprite*> m_TwistVector;
    NodeGrid* m_pRubGrid;
    Sprite* m_pCurDough;
    TouchLayer* m_pTouchLayer;
    int m_nCutCount;
    int m_nRubCount;
    int m_nMoveCount;
    
    Vec2 m_prePos;
};
#endif /* TwistRubDoughScene_h */
