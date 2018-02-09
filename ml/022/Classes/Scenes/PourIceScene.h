
#ifndef __PourIceScene__
#define __PourIceScene__


#include "ExtensionScene.h"
#include "BlenderCup.h"
#include "JuiceMachine.h"

class PourIceScene : public ExtensionScene
{
public:
    PourIceScene();
    ~PourIceScene();
    
    MY_PHYSIC_SCENE(PourIceScene);
    CREATE_FUNC(PourIceScene);
    
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
    void _showTray();
    void _showBlenderCup();
    void _pourIce();
    
    DragNode* m_pTray;
    BlenderCup* m_pCup;
    Vector<Sprite*> m_IceVector;
    int m_nShapeIndex;
    int m_nIceCount;
};

#endif