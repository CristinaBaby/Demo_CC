
#ifndef EggFryBaconScene_h
#define EggFryBaconScene_h

#include "ExtensionScene.h"

class EggFryBaconScene : public ExtensionScene
{
public:
    EggFryBaconScene();
    ~EggFryBaconScene();
    
    MY_SCENE(EggFryBaconScene);
    CREATE_FUNC(EggFryBaconScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    Sprite* m_pBg;
};
#endif /* EggFryBaconScene_h */
