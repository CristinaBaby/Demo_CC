
#ifndef __TwistRiseDoughScene__
#define __TwistRiseDoughScene__


#include "ExtensionScene.h"
#include "ScribbleNode.h"
#include "ItemScrollView.h"
#include "DecTypeScrollView.h"
#include "TouchLayer.h"
#include "KettleNode.h"

class TwistRiseDoughScene : public ExtensionScene
{
public:
    TwistRiseDoughScene();
    ~TwistRiseDoughScene();
    
    MY_SCENE(TwistRiseDoughScene);
    CREATE_FUNC(TwistRiseDoughScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(TwistRiseDoughScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(TwistRiseDoughScene));}
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    virtual void onButtonCallback(Button* btn);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    typedef enum{
        eTagBegin = 99,
        eTagSugar,
        eTagYeast,
        eTagFlour,
        eTagWater,
        eTagMilk,
        eTagEgg,
        eTagButter,
        eTagSalt,
        eTagOil,
        eTagWhrisk,
        eTagEnd
    }IngredientTag;
    
    typedef enum{
        eEgg = 100,
        eSugar,
        eYeast,
        eFlour,
        eWater,
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
    void _riseDough();
    
    void _playEffectNice(float dt = 0.0f);
    
    void _finishAdd(int index);
    void _finishMix();
    void _finishRoll();
    
    void _finishAll();
    
    
// ****** Mix Unit
    Sprite* m_pBowl;
    Sprite* m_pBowlFront;
    Node* m_pMixture;
    ClippingNode* m_pMixtureClipping;
    Sprite* m_pEgg0_0;
    
    DragNode* m_pEgg0;
    DragNode* m_pWater;
    DragNode* m_pSugar;
    DragNode* m_pButter;
    DragNode* m_pMilk;
    DragNode* m_pSalt;
    DragNode* m_pYeast; //发酵粉
    DragNode* m_pFlour;
    DragNode* m_pOil;
    DragNode* m_pWhrisk;
    
    int m_nStep;
    int m_nAddCount;
    int m_nMixCount;
    bool m_bWhriskIsReady;
    bool m_bReadyAdd;
    
    int m_nIndex;
    bool m_bCanTouch;
    bool m_bGuide[3];
    };

#endif