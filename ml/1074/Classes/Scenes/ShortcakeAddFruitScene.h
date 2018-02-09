
#ifndef ShortcakeAddFruitScene_h
#define ShortcakeAddFruitScene_h

#include "ExtensionScene.h"

class ShortcakeAddFruitScene : public ExtensionScene
{
public:
    ShortcakeAddFruitScene();
    ~ShortcakeAddFruitScene();
    
    MY_PHYSIC_SCENE(ShortcakeAddFruitScene);
    CREATE_FUNC(ShortcakeAddFruitScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    void updateStart(float delta);
    void update(float delta);
protected:
    virtual void _initData();
    void _showFood();
    void _showBowl();
    void _pourFruit();
    void _saveFruit();
    void _saveCake();
    void _finish();
    
    Sprite* m_pBg;
    
    DragNode* m_pBowl;
    Sprite* m_pBowlFront;
    
    Sprite* m_pPlate;
    Node* m_pFood;
    Node* m_pFruit;
    
    Vector<Sprite*> m_Fruits;
    
    int m_nCakeCount;
    int m_nFlavorIndex;
    Size m_CakeSize;
};
#endif /* ShortcakeAddFruitScene_h */
