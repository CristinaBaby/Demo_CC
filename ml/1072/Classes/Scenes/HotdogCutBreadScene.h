
#ifndef HotdogCutBreadScene_h
#define HotdogCutBreadScene_h

#include "ExtensionScene.h"
#include "ScribbleSauce.h"

class HotdogCutBreadScene : public ExtensionScene
{
public:
    HotdogCutBreadScene();
    ~HotdogCutBreadScene();
    
    MY_SCENE(HotdogCutBreadScene);
    CREATE_FUNC(HotdogCutBreadScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void onButtonCallback(Button* btn);
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _reset();
    
    void _showBread();
    void _showSausage();
    void _showKnife();
    void _showOnion();
    void _showSause();
    
    void _finishCutBread();
    void _finishAddSausage();
    void _finishAddOnion();
    void _saveHotdog();
    
    Sprite* m_pBg;
    
    Sprite* m_pBoard;
    Sprite* m_pSausagePlate;
    Sprite* m_pBread;
    Sprite* m_pBreadBack;
    Sprite* m_pBreadFront;
    Sprite* m_pCrumbs;
    
    DragNode* m_pSausage;
    DragNode* m_pKnife;
    Node* m_pFoodNode;
    
    DragNode* m_pOnionInPlate;
    Sprite* m_pOnionPlate;
    
    DragNode* m_pSauseYellow;
    DragNode* m_pSauseRed;
    ClippingNode* m_pSauseClipping;
    ScribbleSauce* m_pSauce;
    
    int m_nMoveCount;
};
#endif /* HotdogCutBreadScene_h */
