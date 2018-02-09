

#ifndef GrillBreadScene_h
#define GrillBreadScene_h

#include "ExtensionScene.h"
#include "ProgressBarNew.h"

class GrillBreadScene : public ExtensionScene {
    
public:
    GrillBreadScene();
    ~GrillBreadScene();
    
    MY_SCENE(GrillBreadScene);
    CREATE_FUNC(GrillBreadScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(GrillBreadScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(GrillBreadScene));}
    
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
    Sprite* m_pGrill;
    Sprite* m_pGrill2;
    Sprite* m_pPlateLeft;
    Sprite* m_pPlateRight;
    
    std::vector<DragNode*> m_BreadLeftVector;
    std::vector<DragNode*> m_BreadRightVector;
    std::map<int,DragNode*> m_BreadPotMap;
};
#endif /* GrillBreadScene_h */
