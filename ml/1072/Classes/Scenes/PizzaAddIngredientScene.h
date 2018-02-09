
#ifndef PizzaAddIngredientScene_h
#define PizzaAddIngredientScene_h

#include "ExtensionScene.h"
#include "ScribbleNode.h"
#include "ItemScrollView.h"
#include "DecTypeScrollView.h"
#include "TouchLayer.h"

class PizzaAddIngredientScene : public ExtensionScene
{
public:
    PizzaAddIngredientScene();
    ~PizzaAddIngredientScene();
    
    MY_SCENE(PizzaAddIngredientScene);
    CREATE_FUNC(PizzaAddIngredientScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void onButtonCallback(Button* btn);
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    void TouchBegan(Touch *pTouch, Event *pEvent);
    void TouchEnded(Touch *pTouch, Event *pEvent);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _createRollUI();
    void _pourDough();
    void _showSpreadFlour();
    void _showRollTool();
    void _removeRollTool();
    
    void _showPan();
    
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
    
//    deel
    
    void _brushOil();
    void _brushSauce();
    void _sliceCheese();
    void _onTypeCallback(int index,DecorateTypeConfigData,bool selected);
    void _onDecorationCallback(int index,int type,DecorateConfigData Data);
    
    void _savePizza();
    void _savePizzaDecoration();
    void _savePizzaDecorationRipe();
//    finish
    void _finishRoll();
    void _finishBrushOil();
    void _finishBrushSauce();
    void _finishCheeseSlice();
    void _finishDecorate();
    
    Sprite* m_pBg;
    Sprite* m_pTable;
    
    Sprite* m_pBowl;
    Sprite* m_pBowlFront;
    Sprite* m_pDoughBowl;
    
    Sprite* m_pPan;
    Sprite* m_pDoughPan;
    
    NodeGrid* m_pDoughGrid;
    Sprite* m_pDough;
    Sprite* m_pFlourSprinkle;
    DragNode* m_pRollTool;
    
    
    //    ****** Sauce Unit
    Sprite* m_pOilPlate;
    Sprite* m_pSaucePlate;
//    NodeGrid* m_pOilGrid;
//    NodeGrid* m_pSauceGrid;
//    Sprite* m_pOil;
//    Sprite* m_pSauce;
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
    Sprite* m_pBanner;
    
    //    ClippingNode* m_pDecoration;
    Node* m_pDecoration;
    Node* m_pDecorationRipe;
    int m_nIndex;
    bool m_bCanTouch;
    bool m_bGuide[3];
    
    int m_nRollCount;
    int m_nRollStep;
    bool m_bRollToolReady;
};
#endif /* PizzaAddIngredientScene_h */
