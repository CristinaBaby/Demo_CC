
#ifndef __MakeNormalPizzaScene__
#define __MakeNormalPizzaScene__


#include "ExtensionScene.h"
#include "ScribbleNode.h"
#include "ItemScrollView.h"
#include "DecTypeScrollView.h"
#include "TouchLayer.h"

class MakeNormalPizzaScene : public ExtensionScene
{
public:
    MakeNormalPizzaScene();
    ~MakeNormalPizzaScene();
    
    MY_SCENE(MakeNormalPizzaScene);
    CREATE_FUNC(MakeNormalPizzaScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    virtual void onButtonCallback(Button* btn);
    void TouchBegan(Touch *pTouch, Event *pEvent);
    void TouchEnded(Touch *pTouch, Event *pEvent);
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    typedef enum{
        eFlour,
        eYeast,
        eSalt,
        eSugar,
        eWater,
    }IngredientIndex;
protected:
    virtual void _initData();
    
//    Mix UI
    void _createMixUI();
    void _showMixIngredient(int index);
    void _showWhisk();
    void _removeWhisk();
//    Roll UI
    void _createRollUI();
    void _pourDough();
    void _showSpreadFlour();
    void _showRollTool();
    void _removeRollTool();
//    Spread sauce UI
    void _createSauceUI();
    void _showSauceIngredient();
    void _removeSauceIngredient();
//    add cheese UI
    void _createCheeseUI();
    void _showCheeseIngredient();
    void _removeCheeseIngredient();
    
//    add fruit veg meat UI
    void _showDecorations();
    void _removeDecorations();
    
//    Sholve get pizza
    void _showSholve();
    
//    deel
    
    void _addIngredient(DragNode* pIngredient);
    void _mixBatter();
    void _rollDough();
    void _addDoughPan();
    void _brushOil();
    void _brushSauce();
    void _sliceCheese();
    void _onTypeCallback(int index,DecorateTypeConfigData,bool selected);
    void _onDecorationCallback(int index,DecorateConfigData Data);
    void _playEffectNice(float dt = 0.0f);
    RenderTexture* _getDecorationResultRender();
    void _getSholvePizza();
    void _finishAdd();
    void _finishMix();
    void _finishRoll();
    void _finishBrushOil();
    void _finishBrushSauce();
    void _finishCheeseSlice();
    void _finishDecorate();
    void _finishAll();
    void _savePizza();
    void _savePizzaDecoration();
    void _savePizzaDecorationRipe();
    
// ****** Mix Unit
    Sprite* m_pBowl;
    Node* m_pMixture;
    
    DragNode* m_pYeast; //发酵粉
    DragNode* m_pFlour;
    DragNode* m_pSugar;
    DragNode* m_pSalt;
    DragNode* m_pWater;
    DragNode* m_pWhrisk;
    
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