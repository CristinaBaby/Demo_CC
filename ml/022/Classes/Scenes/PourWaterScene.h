
#ifndef __PourWaterScene__
#define __PourWaterScene__


#include "ExtensionScene.h"
#include "BlenderCup.h"
#include "JuiceMachine.h"

class PourWaterScene : public ExtensionScene
{
public:
    PourWaterScene();
    ~PourWaterScene();
    
    MY_SCENE(PourWaterScene);
    CREATE_FUNC(PourWaterScene);
    virtual bool init();
    virtual void onEnter();
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
protected:
    virtual void _initData();
    void _showShadow(DragNode* pDragNode);
    void _hideShadow(DragNode* pDragNode);
    
    void _showTray();
    void _pourWater();
    
    DragNode* m_pTray;
    Sprite* m_pTrayMask;
    ClippingNode* m_pTrayClipping;
    DragNode* m_pWaterBottle;
    Vector<Sprite*> m_WaterVector;
    int m_nPourCount;
    int m_nShapeIndex;
    int m_nIceCount;
};

#endif