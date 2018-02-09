
#ifndef __CookHotdogScene__
#define __CookHotdogScene__


#include "ExtensionScene.h"
#include "ScribbleNode.h"
#include "ItemScrollView.h"
#include "DecTypeScrollView.h"
#include "TouchLayer.h"
#include "KettleNode.h"

class CookHotdogScene : public ExtensionScene
{
public:
    CookHotdogScene();
    ~CookHotdogScene();
    
    MY_SCENE(CookHotdogScene);
    CREATE_FUNC(CookHotdogScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(CookHotdogScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(CookHotdogScene));}
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    virtual void onButtonCallback(Button* btn);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
protected:
    virtual void _initData();
    
    void _createFryUI();
    void _showPanLid();
    
//    deel
    void _back(DragNode* pIngredient);
    void _addSake();
    void _doCook();
    void _finishAll();
    
    
// ****** Mix Unit
    Sprite* m_pBowl;
    Sprite* m_pMixture;
    
    Sprite* m_pStove;
    Sprite* m_pPan;
    Sprite* m_pPanLid;
    Sprite* m_pPanSake;
    DragNode* m_pSakeBottle;
    Sprite* m_pSakePlatform;
    ClippingNode* m_pLidClipping;
    std::vector<Sprite*> m_HotdogVector;
    
    int m_nFryCount;
    bool m_bSpoonIsReady;
    bool m_bSpatulaIsReady;
    };

#endif