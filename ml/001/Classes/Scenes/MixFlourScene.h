
#ifndef __MixFlourScene__
#define __MixFlourScene__


#include "ExtensionScene.h"
#include "ScribbleNode.h"
#include "ItemScrollView.h"
#include "DecTypeScrollView.h"
#include "TouchLayer.h"
#include "KettleNode.h"

class MixFlourScene : public ExtensionScene
{
public:
    MixFlourScene();
    ~MixFlourScene();
    
    MY_SCENE(MixFlourScene);
    CREATE_FUNC(MixFlourScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(MixFlourScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(MixFlourScene));}
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    virtual void onButtonCallback(Button* btn);
    void TouchBegan(Touch *pTouch, Event *pEvent);
    void TouchEnded(Touch *pTouch, Event *pEvent);
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    typedef enum{
        eTagBegin = 99,
        eTagEgg0,
        eTagEgg1,
        eTagSugar,
        eTagWater,
        eTagButter,
        eTagMilk,
        eTagSalt,
        eTagFlour,
        eTagYeast,
        eTagWhrisk,
        eTagEnd
    }IngredientTag;
    
    typedef enum{
        eEgg0 = 100,
        eEgg1,
        eSugar,
        eWater,
        eFlour,
        eYeast,
        eSalt,
    }IngredientIndex;
protected:
    virtual void _initData();
    
//    Mix UI
    void _createMixUI();
    void _showMixIngredient(int index);
    void _moveDownMixIngredient(int tag);
    void _showWhisk();
    void _removeWhisk();
//    Roll UI
    void _createRollUI();
    void _pourDough();
    void _showSpreadFlour();
    void _showRollTool();
    void _removeRollTool();   
    
    
//    deel
    void _back(DragNode* pIngredient);
    void _addIngredient(DragNode* pIngredient);
    void _mixBatter();
    void _rollDough();
    void _addDoughPan();
    
    void _playEffectNice(float dt = 0.0f);
    
    void _finishAdd(int index);
    void _finishMix();
    void _finishRoll();
    
    void _finishAll();
    
    
// ****** Mix Unit
    Sprite* m_pBowl;
    Sprite* m_pBowlFront;
    Node* m_pMixture;
    Sprite* m_pEgg0_0;
    Sprite* m_pEgg1_0;
    
    DragNode* m_pEgg0;
    DragNode* m_pEgg1;
    DragNode* m_pWater;
    DragNode* m_pSugar;
    DragNode* m_pButter;
    DragNode* m_pMilk;
    DragNode* m_pSalt;
    DragNode* m_pYeast; //发酵粉
    DragNode* m_pFlour;
    DragNode* m_pWhrisk;
    
    int m_nStep;
    int m_nAddCount;
    int m_nMixCount;
    bool m_bWhriskIsReady;
    bool m_bReadyAdd;
    
//    ***** Roll Unit
    NodeGrid* m_pDoughGrid;
    Sprite* m_pDough;
    Sprite* m_pDoughPan;
    Sprite* m_pFlourSprinkle;
    DragNode* m_pRollTool;
    int m_nRollCount;
    bool m_bRollToolReady;
    
//    ****** Sauce Unit
    Sprite* m_pPan;
    DragNode* m_pDragDough;
    Sprite* m_pOilPlate;
    Sprite* m_pSaucePlate;
    NodeGrid* m_pOilGrid;
    NodeGrid* m_pSauceGrid;
    Sprite* m_pOil;
    Sprite* m_pSauce;
    DragNode* m_pOilBrush;
    DragNode* m_pSauceSpoon;
    ScribbleNode* m_pOilScribble;
    ScribbleNode* m_pSauceScribble;
    
    bool m_bOilReady;
    bool m_bSauceReady;
    int m_nOilPercent;
    int m_nSaucePercent;
//    ********* Cheese unit
    DragNode* m_pGrater;
    DragNode* m_pCheese;
    Node* m_pCheeseSlice;
    bool m_bGraterReady;
    int m_nCheeseCount;
    
//    *********** dec unit
    DecTypeScrollView* m_pTypeScrollView;
    ItemScrollView* m_pDecorationScrollView;
    TouchLayer* m_pTouchLayer;
    std::string m_sCurTypeStr;
    std::string m_sCurTypePath;
//    ClippingNode* m_pDecoration;
    Node* m_pDecoration;
    Node* m_pDecorationRipe;
    
    int m_nIndex;
    bool m_bCanTouch;
    bool m_bGuide[3];
    
//    **********put bake unit
    DragNode* m_pSholve;
    Node* m_pFinishPizza;
};

#endif