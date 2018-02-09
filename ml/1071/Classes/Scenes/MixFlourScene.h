
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
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    typedef enum{
        eTagBegin = 99,
        eTagFlour,
        eTagWater,
        eTagEgg,
        eTagCabbage,
        eTagWhrisk,
        eTagEnd
    }IngredientTag;
    
    typedef enum{
        eEgg = 100,
        eCabbage,
        eFlour,
        eWater,
    }IngredientIndex;
protected:
    virtual void _initData();
    
//    Mix UI
    void _createMixUI();
    void _showMixIngredient(int index);
    void _showWhisk();
    void _removeWhisk(); 
    
    
//    deel
    void _back(DragNode* pIngredient);
    void _addIngredient(DragNode* pIngredient);
    void _mixBatter();
    
    void _playEffectNice(float dt = 0.0f);
    
    void _finishAdd(int index);
    void _finishMix();
    
    void _finishAll();
    
    
// ****** Mix Unit
    Sprite* m_pBowl;
    Node* m_pMixture;
    
    DragNode* m_pEgg;
    DragNode* m_pWater;
    DragNode* m_pCabbage;
    DragNode* m_pFlour;
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