
#ifndef __MakeCottonScene__
#define __MakeCottonScene__


#include "ExtensionScene.h"
#include "CottonMachine.h"
#include "SugarBottle.h"

class MakeCottonScene : public ExtensionScene
{
public:
    MakeCottonScene();
    ~MakeCottonScene();
    
    MY_SCENE(MakeCottonScene);
    CREATE_FUNC(MakeCottonScene);
    enum{
        eDragTagFlavorBottle,
        eDragTagStick,
    };
    virtual bool init();
    virtual void onEnter();
    virtual void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    virtual void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    virtual void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    virtual void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    void onSwitchStateCallback(bool on);
protected:
    virtual void _initData();
    
//    choose flavor
    void _showFlavorUI();
    void _removeFlavorBottle();
    void _pourSugar();
    
//    choose shape
    void _showShapeUI();
    void _removeShapeBag();
    void _openShaopBag(LockItem* pItem);
    
//    make cottoncandy
    void _showMakeCandyUI();
    void _showStick();
    void _showMachine();
    void _makeCandy(DragNode* pDragNode,Point worldPoint);
    
//    callback
    
    void _onFlavorClicked(LockItem* item);
    void _onShapeClicked(LockItem* item);
    
//    finish deel
    void _finishPourSugar();
    void _finishMakeSilk();
    void _finishChooseShap();
    void _finish();
    
    void _distant(); //远景 缩小
    void _foreground(); //近景  背景放大
    
    Sprite* m_pBg;
    Sprite* m_pTable;
    Sprite* m_pShed;
    
    ExpandScrollView* m_pFlavorScroll;
    Sprite* m_pBoard;
    
    Vector<LockItem*> m_ShapeItems;
    Sprite* m_pRope;
    
    CottonMachine* m_pMachine;
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
    
    
    bool m_bItemEnable;
    bool m_bStickReady;
};

#endif