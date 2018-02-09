

#ifndef RollSausageScene_h
#define RollSausageScene_h

#include "ExtensionScene.h"
#include "ProgressBarNew.h"
#include "TouchLayer.h"

class RollSausageScene : public ExtensionScene {
    
public:
    RollSausageScene();
    ~RollSausageScene();
    
    MY_SCENE(RollSausageScene);
    CREATE_FUNC(RollSausageScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(RollSausageScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(RollSausageScene));}
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    void TouchBegan(Touch *pTouch, Event *pEvent);
    void TouchMove(Touch *pTouch, Event *pEvent);
    void TouchEnded(Touch *pTouch, Event *pEvent);
protected:
    virtual void _initData();
    
    void _showPlate();
    void _showPlateRight();
    void _showPlateDown();
    
    void _finishRoll();
    void _showFinishDisplay();
    
    void _onProgressCallback(ProgressBarNew*,int);
    
    TouchLayer* m_pTouchLayer;
    Sprite* m_pPlateLeft;
    Sprite* m_pPlateRight;
    Sprite* m_pPlateDown;
    Sprite* m_pHotdog;
    
    std::vector<DragNode*> m_SausageVector;
    std::vector<DragNode*> m_PancakeVector;
    std::vector<DragNode*> m_PlateDownVector;
    int m_nAddCount;
    int m_nMoveCount;
    bool m_bReady;
    bool m_bCanRoll;
};
#endif /* RollSausageScene_h */
