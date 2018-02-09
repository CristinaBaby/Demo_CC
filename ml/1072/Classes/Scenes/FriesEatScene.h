
#ifndef FriesEatScene_h
#define FriesEatScene_h

#include "ShareBaseScene.h"

class FriesEatScene : public ShareBaseScene

{
public:
    FriesEatScene();
    ~FriesEatScene();
    
    MY_SCENE(FriesEatScene);
    CREATE_FUNC(FriesEatScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    void onButtonCallback(Button* btn);
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _reset();
    
    void _showFries();
    
    Sprite* m_pBg;
    Sprite* m_pBox;
    Sprite* m_pBoxFront;
    ClippingNode* m_pFriesNode;
    std::vector<DragNode*> m_FriesVector;
    
    int m_nEatCount;
    bool m_bButtonEable;
};
#endif /* FriesEatScene_h */
