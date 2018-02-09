

#ifndef FrySausageScene_h
#define FrySausageScene_h

#include "ExtensionScene.h"
#include "ProgressBarNew.h"

class FrySausageScene : public ExtensionScene {
    
public:
    FrySausageScene();
    ~FrySausageScene();
    
    MY_SCENE(FrySausageScene);
    CREATE_FUNC(FrySausageScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(FrySausageScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(FrySausageScene));}
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
protected:
    virtual void _initData();
    
    void _showPlate();
    void _showPlateRight();
    void _doFry(DragNode* pNode);
    void _finishFry(DragNode* pNode);
    
    void _onProgressCallback(ProgressBarNew*,int);
    
    Sprite* m_pStove;
    Sprite* m_pPan;
    Sprite* m_pPanOil;
    
    Sprite* m_pPlateLeft;
    Sprite* m_pPlateRight;
    
    std::vector<DragNode*> m_SausageLeftVector;
    std::vector<DragNode*> m_SausageRightVector;
    std::map<int,DragNode*> m_SausagePotMap;
    bool m_bFull[3];
};
#endif /* FrySausageScene_h */
