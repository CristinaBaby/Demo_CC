
#ifndef OmeletteMixScene_h
#define OmeletteMixScene_h

#include "ExtensionScene.h"

class OmeletteMixScene : public ExtensionScene
{
public:
    OmeletteMixScene();
    ~OmeletteMixScene();
    
    MY_SCENE(OmeletteMixScene);
    CREATE_FUNC(OmeletteMixScene);
    
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
    void _showMixTool();
    void _showIngredient(int index);
    void _addIngredient(DragNode* pNode);
    void _ingredientOut(DragNode* pNode);
    void _finishMix();
    void _finish();
    
    Sprite* m_pBg;
    
    Sprite* m_pBowl;
    Sprite* m_pBowlFront;
    ClippingNode* m_pMixtureClipping;
    Node* m_pMixture;
    DragNode* m_pMixTool;
    
    
    int m_nStep;
    int m_nEggCount;
};
#endif /* OmeletteMixScene_h */
