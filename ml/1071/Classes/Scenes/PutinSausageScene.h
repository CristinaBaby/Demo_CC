

#ifndef PutinSausageScene_h
#define PutinSausageScene_h

#include "ExtensionScene.h"
#include "ProgressBarNew.h"

class PutinSausageScene : public ExtensionScene {
    
public:
    PutinSausageScene();
    ~PutinSausageScene();
    
    MY_SCENE(PutinSausageScene);
    CREATE_FUNC(PutinSausageScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(PutinSausageScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(PutinSausageScene));}
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
protected:
    virtual void _initData();
    
    void _showPlate();
    void _showPlateRight();
    void _showPlateDown();
    
    void _finishPut();
    void _showFinishDisplay();
    
    void _onProgressCallback(ProgressBarNew*,int);
    
    Sprite* m_pPlateLeft;
    Sprite* m_pPlateRight;
    Sprite* m_pPlateDown;
    
    std::vector<DragNode*> m_BreadLeftVector;
    std::vector<DragNode*> m_BreadRightVector;
    std::vector<DragNode*> m_PlateDownVector;
    int m_nAddCount;
};
#endif /* PutinSausageScene_h */
