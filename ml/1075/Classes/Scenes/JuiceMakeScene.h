
#ifndef JuiceMakeScene_h
#define JuiceMakeScene_h

#include "ExtensionScene.h"
#include "BlenderNode.h"

class JuiceMakeScene : public ExtensionScene
{
public:
    JuiceMakeScene();
    ~JuiceMakeScene();
    
    MY_PHYSIC_SCENE(JuiceMakeScene);
    CREATE_FUNC(JuiceMakeScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    void updateStart(float delta);
    void update(float delta);
protected:
    virtual void _initData();
    
    void _showBowl();
    void _showBlender();
    void _showWater();
    void _pourFruits();
    void _pourWater();
    void _beginCrush();
    void _blenderLidOpen();
    void _finish();
    
    Sprite* m_pBg;
    
    DragNode* m_pBowl;
    Sprite* m_pBowlFront;
    DragNode* m_pWater;
    
    BlenderNode* m_pBlender;
    BlenderCup* m_pCup;
    int m_nFlavorIndex;
    
    Vector<Sprite*> m_Fruits;
};
#endif /* JuiceMakeScene_h */
