
#ifndef OmeletteFryScene_h
#define OmeletteFryScene_h

#include "ExtensionScene.h"

class OmeletteFryScene : public ExtensionScene
{
public:
    OmeletteFryScene();
    ~OmeletteFryScene();
    
    MY_SCENE(OmeletteFryScene);
    CREATE_FUNC(OmeletteFryScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showStove();
    void _showPan();
    void _showMixture();
    void _showShovel();
    void _showPlate();
    void _showIngredient(int index);
    void _addIngredient(DragNode* pNode);
    void _ingredientOut(DragNode* pNode);
    
    void _doFryOmelette(Sprite* pFood);
    void _finish();
    
    Sprite* m_pBg;
    
    Sprite* m_pStove;
    Sprite* m_pFire;
    Sprite* m_pPan;
    Sprite* m_pPanFront;
    Node* m_pPanIn;
    
    DragNode* m_pBowl;
    DragNode* m_pShovel;
    Sprite* m_pOmelette;
    Sprite* m_pPlate;
    
    int m_nStep;
};
#endif /* OmeletteFryScene_h */
