
#ifndef __DecorateScene__
#define __DecorateScene__


#include "ExtensionScene.h"
#include "ItemScrollView.h"
#include "TouchLayer.h"
#include "DecTypeScrollView.h"
#include "ScribbleNode.h"
#include "AddSprinkle.h"

class DecorateScene : public ExtensionScene
{
public:
    DecorateScene();
    ~DecorateScene();
    
    MY_SCENE(DecorateScene);
    CREATE_FUNC(DecorateScene);
    
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
    
    void onTypeCallback(int index,int type,DecorateTypeConfigData,bool selected);
    void onDecorateOK();
    void onDecorationCallback(int index,int type,DecorateConfigData Data);
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _initDefaultDecorate();
    void _reset();
    void _saveFood();
    void _displayFood();
    void _getFoodRenderData();
    
    Sprite* m_pBg;
    
    Layer* m_pDecorateLayer;
    
    DecTypeScrollView* m_pTypeScrollView;
    ItemScrollView* m_pDecorationScrollView;
    
    TouchLayer* m_pTouchLayer;
    ScribbleNode* m_pIcingScribble;
    Node* m_pDecNode;
//    ClippingNode* m_pClipping;
    
    Sprite* m_pFood;
    Sprite* m_pBanner;
    
    int m_nFlavor;
    int m_nShape;
    std::string m_sCurTypeStr;
    std::string m_sCurTypePath;
    bool m_bCanTouch;
    int m_nIndex;
    Size m_CandySize;
    bool m_bPacked;
    
    bool m_bButtonEable;
    bool m_bGuide[3];
    bool m_bVOGuide[8];
    MyRenderTexture* m_pFoodRender;
    AddSprinkle* currentSprinkle;
    int m_nTypeIndex;
};

#endif