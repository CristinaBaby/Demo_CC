
#ifndef BurgerDecorateScene_h
#define BurgerDecorateScene_h

#include "ExtensionScene.h"
#include "ItemScrollView.h"
#include "DecTypeScrollView.h"

class BurgerDecorateScene : public ExtensionScene
{
public:
    BurgerDecorateScene();
    ~BurgerDecorateScene();
    
    MY_SCENE(BurgerDecorateScene);
    CREATE_FUNC(BurgerDecorateScene);
    
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
    void _showBurger();
    void _showBeaf();
    void _showDecorations();
    void _savePizza();
    Vec2 _getTopPos();
    
    void _onTypeCallback(int index,DecorateTypeConfigData,bool selected);
    void _onDecorationCallback(int index,int type,DecorateConfigData Data);
    
    
    Sprite* m_pBg;
    Sprite* m_pBanner;
    DecTypeScrollView* m_pTypeScrollView;
    ItemScrollView* m_pDecorationScrollView;
    std::string m_sCurTypeStr;
    std::string m_sCurTypePath;
    
    Sprite* m_pPlate;
    Sprite* m_pBreadDown;
    Sprite* m_pBreadOn;
    Node* m_pFood;
    Node* m_pDec;
    
    
    Sprite* m_pPlateBeaf;
    DragNode* m_pBeaf;
    Vec2 m_decPos;
};
#endif /* BurgerDecorateScene_h */
