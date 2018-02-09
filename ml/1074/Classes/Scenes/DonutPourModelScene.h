
#ifndef DonutPourModelScene_h
#define DonutPourModelScene_h

#include "ExtensionScene.h"

class DonutPourModelScene : public ExtensionScene
{
public:
    DonutPourModelScene();
    ~DonutPourModelScene();
    
    MY_SCENE(DonutPourModelScene);
    CREATE_FUNC(DonutPourModelScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showBowls();
    void _showModel();
    void _showSpoon();
    void _lightBowl();
    void _addBatter(int flavorIndex,int maskIndex);
    void _finish();
    Sprite* m_pBg;
    
    Node* m_pModel;
    DragNode* m_pSpoon;
    
    int m_nShapeIndex;
    std::vector<Node*> m_smallBowls;
//    std::vector<ClippingNode*> m_BatterClippings;
    std::vector<MyRenderTexture*> m_Batters;
    std::vector<MyRenderTexture*> m_renderTextures;
    
    Node* m_pDonutGuide;
    Node* m_pDonutGuideHighlight;
    
    int m_nPourCount;
    int m_nFlavors[5];
};
#endif /* DonutPourModelScene_h */
