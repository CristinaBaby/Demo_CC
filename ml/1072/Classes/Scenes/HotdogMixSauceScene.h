
#ifndef HotdogMixSauceScene_h
#define HotdogMixSauceScene_h

#include "ExtensionScene.h"

class HotdogMixSauceScene : public ExtensionScene
{
public:
    HotdogMixSauceScene();
    ~HotdogMixSauceScene();
    
    MY_SCENE(HotdogMixSauceScene);
    CREATE_FUNC(HotdogMixSauceScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showBowl();
    void _showOil();
    void _showMustard();
    void _showChilli();
    void _showPaprika();
    void _showWhisk();
    
    void _addOil();
    void _addMustard();
    void _addChilli();
    void _addPaprika();
    void _mixBatter();
    
    void _finishAddOil();
    void _finishAddMustard();
    void _finishAddChilli();
    void _finishAddPaprika();
    void _finishMix();
    
    
    Sprite* m_pBg;
    
    Sprite* m_pBowl;
    Node* m_pMixture;
    
    Sprite* m_pOilBowl;
    DragNode* m_pOilSpoon;
    
    Sprite* m_pMustard;
    DragNode* m_pMustardPlate;
    
    Sprite* m_pChilliBowl;
    DragNode* m_pChilliSpoon;
    
    Sprite* m_pPaprikaBowl;
    DragNode* m_pPaprikaSpoon;
    
    DragNode* m_pWhrisk;
    
    int m_nAddOilCount;
    int m_nStep;
    int m_nMixCount;
};
#endif /* HotdogMixSauceScene_h */
