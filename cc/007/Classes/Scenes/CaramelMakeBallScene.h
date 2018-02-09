
#ifndef CaramelMakeBallScene_h
#define CaramelMakeBallScene_h

#include "ExtensionScene.h"
#include "TouchLayer.h"

class CaramelMakeBallScene : public ExtensionScene
{
public:
    CaramelMakeBallScene();
    ~CaramelMakeBallScene();
    
    MY_SCENE(CaramelMakeBallScene);
    CREATE_FUNC(CaramelMakeBallScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    void TouchBegan(Touch *pTouch, Event *pEvent);
    void TouchMoved(Touch *pTouch, Event *pEvent);
    void TouchEnded(Touch *pTouch, Event *pEvent);
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showAll();
    void _finishRoll();
    void _finishPress();
    
    Sprite* m_pBg;
    
    Sprite* m_pMixture;
    Sprite* m_pBowl;
    Sprite* m_pFlour;
    Sprite* m_pTray;
    Sprite* m_pPlate;
    
    DragNode* m_pBall;
    DragNode* m_pSugar;
    
    Sprite* m_pCurCookie;
    
    std::vector<Vec2> m_CookiePosVector;
    std::vector<Sprite*> m_CookieVector;
    
    TouchLayer* m_pTouchLayer;
    bool m_bCanTouch;
    int m_nMoveCount;
    int m_nStep;
};
#endif /* CaramelMakeBallScene_h */
