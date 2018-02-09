

#ifndef HotdogGrillSausageScene_h
#define HotdogGrillSausageScene_h

#include "ExtensionScene.h"
#include "ProgressBarNew.h"

class HotdogGrillSausageScene : public ExtensionScene {
    
public:
    HotdogGrillSausageScene();
    ~HotdogGrillSausageScene();
    
    MY_SCENE(HotdogGrillSausageScene);
    CREATE_FUNC(HotdogGrillSausageScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(HotdogGrillSausageScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(HotdogGrillSausageScene));}
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
protected:
    virtual void _initData();
    
    void _showStove();
    void _showPlate();
    void _showPlateRight();
    void _doFry(DragNode* pNode);
    void _finishFry(DragNode* pNode);
    
    void _onProgressCallback(ProgressBarNew*,int);
    
    Sprite* m_pStove;
    Sprite* m_pFire;
    
    Sprite* m_pGrill;
    Sprite* m_pGrill2;
    Sprite* m_pPlateLeft;
    Sprite* m_pPlateRight;
    
    std::vector<DragNode*> m_SausageRightVector;
    std::map<int,DragNode*> m_SausagePotMap;
    bool m_bFull[3];
};
#endif /* HotdogGrillSausageScene_h */
