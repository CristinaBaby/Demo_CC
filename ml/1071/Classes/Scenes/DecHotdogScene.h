
#ifndef __DecHotdogScene__
#define __DecHotdogScene__


#include "ExtensionScene.h"
#include "ScribbleNode.h"
#include "ItemScrollView.h"
#include "DecTypeScrollView.h"
#include "TouchLayer.h"
#include "KettleNode.h"
#include "ScribbleSauce.h"

class DecHotdogScene : public ExtensionScene
{
public:
    DecHotdogScene();
    ~DecHotdogScene();
    
    MY_SCENE(DecHotdogScene);
    CREATE_FUNC(DecHotdogScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(DecHotdogScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(DecHotdogScene));}
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    virtual void onButtonCallback(Button* btn);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    void TouchBegan(Touch *pTouch, Event *pEvent);
    void TouchMove(Touch *pTouch, Event *pEvent);
    void TouchEnded(Touch *pTouch, Event *pEvent);
protected:
    virtual void _initData();
    
    void _reset();
    void _showHotdog();
    void _showSauseBottle();
    void _showMayonnaise();
    void _showSeaweed();
    void _showGinger();
    
//    deel
    void _back(DragNode* pIngredient);
    std::vector<Vec2> interpolate(Point pStartPosition, Point pEndPosition, float pDelta);
    void _pourSeaweed();
    void _pourGinger();
    
    void _finishAll();
    void _saveHotdog();
    
    TouchLayer* m_pTouchLayer;
    Sprite* m_pPlate;
    Sprite* m_pHotdog;
    
    ClippingNode* m_pClipping;
    
    //sause
    ScribbleSauce* m_pSauce;
//    Sprite* m_pSauseBottle;
    DragNode* m_pSauseBottle;
    
    //mayonnaise
    Sprite* m_pBowlMayonnaise;
    Sprite* m_pMayonnaise;
    DragNode* m_pSpoon;
    Sprite* m_pSpoonIn;
    Sprite* m_pSpoonStream;
    ScribbleNode* m_pScribble;
    
    //seaweed
    DragNode* m_pBowlSeaweed;
    Sprite* m_pSeaweed;
    
    //ginger
    DragNode* m_pBowlGiner;
    Sprite* m_pGiner;
    
    static int ms_nSausageCount;
    
    int m_nStep;
    bool m_bBtnEnable;
};
#endif