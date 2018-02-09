
#ifndef DonutMakeSauceScene_h
#define DonutMakeSauceScene_h

#include "ExtensionScene.h"
#include "SieveNode.h"
#include "TouchLayer.h"

class DonutMakeSauceScene : public ExtensionScene
{
public:
    DonutMakeSauceScene();
    ~DonutMakeSauceScene();
    
    MY_SCENE(DonutMakeSauceScene);
    CREATE_FUNC(DonutMakeSauceScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    
    void TouchEnded(Touch *pTouch, Event *pEvent);
    void onAcceleration(Acceleration *, Event *);
protected:
    virtual void _initData();
    
    void _showBowl();
    void _showTypeItemScroll();
    void _showBottle(int index);
    void _showSpoon();
    void _showSieve();
    void _showIcingSugar();
    void _showMixTool();
    void _showDonut();
    void _showProgressStar();
    
    void _onTypeClicked(LockItem* item);
    void _addBatter(int flavorIndex,int maskIndex);
    void _setContainer(KettleNode* pContainer,int flavorIndex);
    void _FullSpoonSauce(int index);
    void _pourSauce(int index);
    void _pourIcingSugar();
    void _separatePowder();
    void _addSauce2Donut();
    void _saveDonut();
    
    void _finishAddSauce();
    void _finishAddIcingSugar();
    void _finishMix();
    void _finish();
    
    Sprite* m_pBg;
    
    TouchLayer* m_pTouchLayer;
    
    Sprite* m_pBowl;
    Sprite* m_pBowlFront;
    Node* m_pBatter;
    ClippingNode* m_pBatterClipping;
    ExpandScrollView* m_pScrollView;
    Sprite* m_pBanner;
    
    DragNode* m_pSauceBottle;
    DragNode* m_pSpoon;
    ProgressStar*m_pProgressStar;
    
    SieveNode* m_pSieve;
    DragNode* m_pIcingSugar;
    DragNode* m_pMixTool;
    
    DragNode* m_pDonut;
    Sprite* m_pDonutPlate;
    
    Vector<LockItem*> m_typeItems;
    int m_nShapeIndex;
    bool m_bItemEnable;
    int m_nSauceIndex;
    int m_nAddCount;
    int m_nShakeCount;
    int m_nStep;
};
#endif /* DonutMakeSauceScene_h */
