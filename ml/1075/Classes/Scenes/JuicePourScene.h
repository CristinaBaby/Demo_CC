
#ifndef JuicePourScene_h
#define JuicePourScene_h

#include "ExtensionScene.h"
#include "BlenderNode.h"
#include "ItemScrollView.h"
#include "TouchLayer.h"
#include "DecTypeScrollView.h"

class JuicePourScene : public ExtensionScene
{
public:
    JuicePourScene();
    ~JuicePourScene();
    
    MY_SCENE(JuicePourScene);
    CREATE_FUNC(JuicePourScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onButtonCallback(Button* btn);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _showCup();
    void _showBlenderCup();
    void _pourJuice();
    
    void _decorateStaw();
    
    void _finish();
    
    Sprite* m_pBg;
    
    DragNode* m_pBlenderCup;
    Node* m_pCup;
    int m_nFlavorIndex;
    ExpandScrollView* m_pStrawScrollView;
    std::string m_sCurTypeStr;
//    std::string m_sCurTypePath;
    TouchLayer* m_pTouchLayer;
    ClippingNode* m_pClipping;
    ClippingNode* m_pClippingStraw;
};
#endif /* JuicePourScene_h */
