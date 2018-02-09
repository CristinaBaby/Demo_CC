
#ifndef __AddCandleScene__
#define __AddCandleScene__


#include "ExtensionScene.h"
#include<cmath>
#include "CandlePosition.h"
#include "DecTypeScrollView.h"

class AddCandleScene : public ExtensionScene
{
public:
    AddCandleScene();
    ~AddCandleScene();
    
    MY_SCENE(AddCandleScene);
    CREATE_FUNC(AddCandleScene);
    
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    virtual void onButtonCallback(Button* btn);
    void onTypeCallback(int index,DecorateTypeConfigData,bool selected);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    
    virtual void update(float);
//    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
protected:
    virtual void _initData();
    void _reset();
    
    void _showGuideCandlePos();
    
    void _showItemScrollView(bool nooffset = true);
    void _hideItemScrollView();
    
    Sprite* m_pDecoratedCake;
    
    DragNode* m_pTool;
    
    Vector<Sprite*> m_VectorCandle;
    int m_nType;
    
    int m_nSegment;
    bool m_flag[50];
    std::vector<Vec3> m_candleIndex;
    
    CandlePosition m_CandlePos;
    
    DecTypeScrollView* m_pTypeScrollView;
    ExpandScrollView* m_pItemScrollView;
    Sprite* m_pCurCandle;
    
    int m_nPressTime;
    bool m_bGuideShown;
    
    Vector<Sprite*> m_GuideCandleVector;
};

#endif