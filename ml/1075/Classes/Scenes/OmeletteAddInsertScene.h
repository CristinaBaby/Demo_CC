
#ifndef OmeletteAddInsertScene_h
#define OmeletteAddInsertScene_h

#include "ExtensionScene.h"
#include "ItemScrollView.h"
#include "TouchLayer.h"

class OmeletteAddInsertScene : public ExtensionScene
{
public:
    OmeletteAddInsertScene();
    ~OmeletteAddInsertScene();
    
    MY_SCENE(OmeletteAddInsertScene);
    CREATE_FUNC(OmeletteAddInsertScene);
    
    virtual bool init();
    virtual void onEnter();
    
    virtual void onButtonCallback(Button* btn);
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    void TouchBegan(Touch *pTouch, Event *pEvent);
    void TouchMoved(Touch *pTouch, Event *pEvent);
    void TouchEnded(Touch *pTouch, Event *pEvent);
    
    void onDecorationCallback(int index,int type,DecorateConfigData Data);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _showOmelette();
    void _showDecorations();
    void _getFoodRenderData();
    void _saveFood();
    void _reset();
    
    Sprite* m_pBg;
    Sprite* m_pPlate;
    Sprite* m_pOmelette;
    TouchLayer* m_pTouchLayer;
    ItemScrollView* m_pDecorationScrollView;
    MyRenderTexture* m_pFoodRender;
    Node* m_pDecNode;
    
    bool m_bCanTouch;
    int m_nIndex;
    bool m_bGuide;
    bool m_bButtonEable;
    Size m_FoodSize;
};
#endif /* OmeletteAddInsertScene_h */
