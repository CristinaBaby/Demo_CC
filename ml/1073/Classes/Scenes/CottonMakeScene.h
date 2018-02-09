
#ifndef CottonMakeScene_h
#define CottonMakeScene_h

#include "ExtensionScene.h"
#include "CottonMachine.h"
#include "SugarBottle.h"

class CottonMakeScene : public ExtensionScene
{
public:
    CottonMakeScene();
    ~CottonMakeScene();
    
    MY_SCENE(CottonMakeScene);
    CREATE_FUNC(CottonMakeScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
//    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    void onSwitchStateCallback(bool on);
protected:
    virtual void _initData();
    
    void _distant(); //远景 缩小
    void _foreground(); //近景  背景放大
    
    void _finish();
    
    Sprite* m_pBg;
    
    CottonMachine* m_pMachine;
    DragNode* m_pSugarNode;
    SugarBottle* m_pSugarBottle;
    DragNode* m_pStick;
    Sprite* m_pSilk;
    Sprite* m_pCotton;
    int m_nFlavorIndex;
    int m_nShapeIndex;
    int m_nStickIndex;
    int m_nMoveCount;
    int m_nStepCount;
    int m_nFlag;
};
#endif /* CottonMakeScene_h */
