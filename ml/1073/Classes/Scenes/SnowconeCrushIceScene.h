
#ifndef SnowconeCrushIceScene_h
#define SnowconeCrushIceScene_h

#include "ExtensionScene.h"
#include "MachineNode.h"

class SnowconeCrushIceScene : public ExtensionScene
{
public:
    SnowconeCrushIceScene();
    ~SnowconeCrushIceScene();
    
    MY_SCENE(SnowconeCrushIceScene);
    CREATE_FUNC(SnowconeCrushIceScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    
    void onMachineStateCallback(int state);
protected:
    virtual void _initData();
    
    void _showMachine();
    void _showIceBag();
    
    void _showCone(Vec2 posDelt = Vec2::ZERO);
    void _moveawayCone();
    
    void _pourIce();
    void _crushIce();
    
    void _finish();
    
    Sprite* m_pBg;
    
    MachineNode* m_pMachine;
    DragNode* m_pIceBag;
    
    DragNode* m_pSpoon;
    Node* m_pSnowConeNode;
    Sprite* m_pCone;
    Sprite* m_pConeFront;
    Sprite* m_pConeIce;
    int m_nSnowPutCount;
};
#endif /* SnowconeCrushIceScene_h */
