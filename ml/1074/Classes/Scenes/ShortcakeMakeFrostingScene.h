
#ifndef ShortcakeMakeFrostingScene_h
#define ShortcakeMakeFrostingScene_h

#include "ExtensionScene.h"

class ShortcakeMakeFrostingScene : public ExtensionScene
{
public:
    ShortcakeMakeFrostingScene();
    ~ShortcakeMakeFrostingScene();
    
    MY_SCENE(ShortcakeMakeFrostingScene);
    CREATE_FUNC(ShortcakeMakeFrostingScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    void onAcceleration(Acceleration *, Event *);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showBowl();
    void _showMixBlender();
    void _showIngredientsStep(int step);
    void _showIngredient(int index);
    
    void _showTypeItemScroll();
    void _showBottle(int index);
    void _showProgressStar();
    
    void _addIngredient(DragNode* pNode);
    void _ingredientOut(DragNode* pNode);
    
    void _onTypeClicked(LockItem* item);
    void _pourSauce();
    
    void _doMix(int index);
    void _finishMix();
    
    Sprite* m_pBg;
    
    Sprite* m_pBowl;
    Sprite* m_pBowlFront;
    ClippingNode* m_pBatterClipping;
    Node* m_pBatter;
    
    Sprite* m_pBlender;
    DragNode* m_pBlenderSwitch;
    Sprite* m_pBlenderStick;
    
    ExpandScrollView* m_pScrollView;
    Vector<LockItem*> m_typeItems;
    bool m_bItemEnable;
    DragNode* m_pSauceBottle;
    ProgressStar*m_pProgressStar;
    
    int m_nStep;
    int m_nFlavorIndex;
};
#endif /* ShortcakeMakeFrostingScene_h */
