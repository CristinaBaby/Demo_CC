
#ifndef DonutMixFlourScene_h
#define DonutMixFlourScene_h

#include "ExtensionScene.h"
#include "SieveNode.h"
#include "TouchLayer.h"

class DonutMixFlourScene : public ExtensionScene
{
public:
    DonutMixFlourScene();
    ~DonutMixFlourScene();
    
    MY_SCENE(DonutMixFlourScene);
    CREATE_FUNC(DonutMixFlourScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    void onAcceleration(Acceleration *, Event *);
    void TouchEnded(Touch *pTouch, Event *pEvent);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showBowl();
    void _showMixTools();
    void _showIngredientsStep(int step);
    void _showIngredient(int index);
    void _showSieve();
    void _showProgressStar();
    
    void _addIngredient(DragNode* pNode);
    void _ingredientOut(DragNode* pNode);
    void _toolElectricWork(bool working = true);
    void _separatePowder();
    
    void _finishAdd();
    void _finishMix();
    
    Sprite* m_pBg;
    TouchLayer* m_pTouchLayer;
    
    Sprite* m_pBowl;
    Sprite* m_pBowlFront;
    Node* m_pBatter;
    Node* m_pFlourBag;
    
    DragNode* m_pMixTool_normal;
    DragNode* m_pMixTool_electric;
    DragNode* m_pCurTool;
    
    SieveNode* m_pSieve;
    ProgressStar*m_pProgressStar;
    
    int m_nStep;
    int m_nFlourAdd;
    int m_nShakeCount;
    int m_nShakeAdd;
    int m_nEggCount;
};
#endif /* DonutMixFlourScene_h */
