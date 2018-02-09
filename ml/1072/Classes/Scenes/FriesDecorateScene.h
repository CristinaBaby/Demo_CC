
#ifndef FriesDecorateScene_h
#define FriesDecorateScene_h

#include "ExtensionScene.h"
#include "ItemScrollView.h"

class FriesDecorateScene : public ExtensionScene
{
public:
    FriesDecorateScene();
    ~FriesDecorateScene();
    
    MY_SCENE(FriesDecorateScene);
    CREATE_FUNC(FriesDecorateScene);
    
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
    void _showFries();
    void _showBoxs();
    void _onDecorationCallback(int index,int type,DecorateConfigData Data);
    
    Sprite* m_pBg;
    ItemScrollView* m_pBoxScrollView;
    
    Sprite* m_pBox;
    Sprite* m_pBoxFront;
    ClippingNode* m_pFriesNode;
    std::vector<Sprite*> m_FriesVector;
};
#endif /* FriesDecorateScene_h */
