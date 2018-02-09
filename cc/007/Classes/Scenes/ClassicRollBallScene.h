
#ifndef ClassicRollBallScene_h
#define ClassicRollBallScene_h

#include "ExtensionScene.h"
#include "TouchLayer.h"

class ClassicRollBallScene : public ExtensionScene
{
public:
    ClassicRollBallScene();
    ~ClassicRollBallScene();
    
    MY_SCENE(ClassicRollBallScene);
    CREATE_FUNC(ClassicRollBallScene);
    
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
    Sprite* m_pTray;
    
    DragNode* m_pScooper;
    
    std::vector<Vec2> m_CookiePosVector;
    std::vector<Sprite*> m_CookieVector;
    
    TouchLayer* m_pTouchLayer;
    
    bool m_bCanTouch;
};
#endif /* ClassicRollBallScene_h */
