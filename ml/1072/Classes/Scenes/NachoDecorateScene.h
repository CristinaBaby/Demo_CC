
#ifndef NachoDecorateScene_h
#define NachoDecorateScene_h

#include "ExtensionScene.h"

class NachoDecorateScene : public ExtensionScene
{
public:
    NachoDecorateScene();
    ~NachoDecorateScene();
    
    MY_SCENE(NachoDecorateScene);
    CREATE_FUNC(NachoDecorateScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void onButtonCallback(Button* btn);
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _showPan();
    void _showSauce();
    void _saveNachos();
    void _reset();
    Sprite* m_pBg;
    
    Sprite* m_pPan;
    Sprite* m_pPanFront;
    Node* m_pFood;
    
    Sprite* m_pSalsaBowl;
    Sprite* m_pCreamBowl;
    DragNode* m_pSalsaSpoon;
    DragNode* m_pCreamSpoon;
    
    int m_nAddCount;
};
#endif /* NachoDecorateScene_h */
