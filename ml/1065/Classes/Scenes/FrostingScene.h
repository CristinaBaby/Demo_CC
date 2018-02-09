
#ifndef __FrostingScene__
#define __FrostingScene__


#include "ExtensionScene.h"
#include "ScribbleNode.h"
#include "CircularScrollView.h"
#include "ChooseLayer.h"

class FrostingScene : public ExtensionScene
{
public:
    FrostingScene();
    ~FrostingScene();
    
    MY_SCENE(FrostingScene);
    CREATE_FUNC(FrostingScene);
    
    virtual bool init();
    
    virtual void onButtonCallback(Button* btn);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);

protected:
    virtual void _initData();
    void _reset();
    
    void _showIngrident(Node*);
    void _showFrosting(int type,int index);
    
    void _showChooseFrosting();
    void _showColorScrollView();
    
    void _onFrosingCallback(int index);
    void _onColorItemCallback(Node*);
    
    Sprite* m_pStand;
    Sprite* m_pCakeDown;
    Sprite* m_pCakeUp;
    DragNode* m_pKnife;
    DragNode* m_pButterKnife;
    
    MyRenderTexture* m_pTempTexture;
    
    Sprite* m_pFrosting;
    ScribbleNode* m_pScriblleNode;
    
    GuideLayer *m_pGuideLayer2;
    //UI
    CircularScrollView* m_pColorScrollview;
    
    int m_nCurType;
    int m_nCurIndex;
    
};

#endif