
#ifndef NachoAddIngredientScene_h
#define NachoAddIngredientScene_h

#include "ExtensionScene.h"

class NachoAddIngredientScene : public ExtensionScene
{
public:
    NachoAddIngredientScene();
    ~NachoAddIngredientScene();
    
    MY_SCENE(NachoAddIngredientScene);
    CREATE_FUNC(NachoAddIngredientScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onButtonCallback(Button* btn);
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showPan();
    void _showIngredient(int index);
    void _finish();
    
    Sprite* m_pBg;
    
    Sprite* m_pPan;
    Node* m_pMixture;
    Node* m_pBowl;
    DragNode* m_pCurFood;
    
    bool m_bBtnEnable;
    int m_nStep;
    int m_nAddCount;
};
#endif /* NachoAddIngredientScene_h */
