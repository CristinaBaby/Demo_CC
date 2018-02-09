
#ifndef __PourJuiceScene__
#define __PourJuiceScene__


#include "ExtensionScene.h"
#include "BlenderNode.h"
#include "ChooseLayer.h"
#include  "CupLayer.h"

class PourJuiceScene : public ExtensionScene
{
public:
    PourJuiceScene();
    ~PourJuiceScene();
    
    MY_PHYSIC_SCENE(PourJuiceScene);
    CREATE_FUNC(PourJuiceScene);
    enum{
        eDragTagFlavorBottle,
        eDragTagStick,
    };
    virtual bool init();
    virtual void onEnter();
    void update(float dt = 0.0f);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
protected:
    virtual void _initData();
    
    void _showBlenderCup();
    void _showSlushyCup();
    void _pourJuice();
    void _finish();
    
    DragNode* m_pBlenderCup;
    CupLayer* m_pCupLayer;
    BlenderCup* m_pCup;
};

#endif