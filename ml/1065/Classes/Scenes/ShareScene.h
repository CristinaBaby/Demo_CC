
#ifndef __ShareScene__
#define __ShareScene__


#include "ExtensionScene.h"
#include "EatLayer.h"
#include "CandlePosition.h"
#include "CandleNode.h"
#include "ChooseBackgroudLayer.h"

class ShareScene : public ExtensionScene
{
public:
    ShareScene();
    ~ShareScene();
    
    MY_SCENE(ShareScene);
    CREATE_FUNC(ShareScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onNotificationCallback(Ref* ref){};
    virtual void onButtonCallback(Button* btn);
    virtual void onKeyBackClicked();
    
    void onEatBeinCallback(Vec2 location,bool drinkable);
    void onEatEndCallback(Vec2 location);
    
    void onBgCallback(int index);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
protected:
    virtual void _initData();
    void _showLightCandleScene();
    void _showEatScene();
    void _showGift();
    
    RenderTexture* getResultRender();
    
    int m_nPackage;
    EatLayer* m_pEatLayer;
    Layer* m_pDecoLayer;
    DecorateManager* m_pDecManager;
    Sprite* m_pBg;
    Sprite* m_pTip;
    
    Sprite* m_pTable;
    
    bool m_bBack;
    
//    light candle scene
    
    void _lightCandle();
    void _dieoutCandle();
    
    Layer* m_pCandleLayer;
    Sprite* m_pDecoratedCake;
    DragNode* m_pTool;
    Vector<CandleNode*> m_VectorCandle;
    bool m_bFired;
    bool m_bDark;
    bool m_bEatLayerShown;
};

#endif