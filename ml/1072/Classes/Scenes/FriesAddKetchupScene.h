
#ifndef FriesAddKetchupScene_h
#define FriesAddKetchupScene_h

#include "ExtensionScene.h"

class FriesAddKetchupScene : public ExtensionScene
{
public:
    FriesAddKetchupScene();
    ~FriesAddKetchupScene();
    
    MY_SCENE(FriesAddKetchupScene);
    CREATE_FUNC(FriesAddKetchupScene);
    
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
    
    void _resetKetchup();
    
    void _showFries();
    void _showKetchup();
    
    Sprite* m_pBg;
    
    Sprite* m_pBox;
    Sprite* m_pBoxFront;
    ClippingNode* m_pFriesNode;
    std::vector<DragNode*> m_FriesVector;
    DragNode* m_pKetchup;
    bool m_bAddKetchup;
};
#endif /* FriesAddKetchupScene_h */
