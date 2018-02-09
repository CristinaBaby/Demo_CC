
#ifndef __FryPancakeScene__
#define __FryPancakeScene__


#include "ExtensionScene.h"
#include "ScribbleNode.h"
#include "ItemScrollView.h"
#include "DecTypeScrollView.h"
#include "TouchLayer.h"
#include "KettleNode.h"

class FryPancakeScene : public ExtensionScene
{
public:
    FryPancakeScene();
    ~FryPancakeScene();
    
    MY_SCENE(FryPancakeScene);
    CREATE_FUNC(FryPancakeScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(FryPancakeScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(FryPancakeScene));}
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    virtual void onButtonCallback(Button* btn);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
protected:
    virtual void _initData();
    
    void _createFryUI();
    void _showBowl();
    void _showSpoon();
    void _removeSpoon();
    
    void _showSpatula();
    void _removeSpatula();
    
    void _showPlate();
    
//    deel
    void _back(DragNode* pIngredient);
    void _addOil();
    void _doFry();
    void _finishAll();
    
    
// ****** Mix Unit
    Sprite* m_pBowl;
    Sprite* m_pMixture;
    
    Sprite* m_pStove;
    Sprite* m_pFire;
    Sprite* m_pFireInner;
    Sprite* m_pPan;
    Sprite* m_pPanOil;
    DragNode* m_pOil;
    
    DragNode* m_pSpoon;
    DragNode* m_pSpatula;
    
    Sprite* m_pFryPan;
    Sprite* m_pPlate;
    
//    int m_nStep;
    int m_nFryCount;
//    int m_nMixCount;
    bool m_bSpoonIsReady;
    bool m_bSpatulaIsReady;
//    bool m_bReadyAdd;
//    
//    int m_nIndex;
//    bool m_bCanTouch;
//    bool m_bGuide[3];
    };

#endif