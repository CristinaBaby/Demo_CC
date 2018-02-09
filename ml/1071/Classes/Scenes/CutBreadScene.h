

#ifndef CutBreadScene_h
#define CutBreadScene_h

#include "ExtensionScene.h"

class CutBreadScene : public ExtensionScene {
    
public:
    CutBreadScene();
    ~CutBreadScene();
    
    MY_SCENE(CutBreadScene);
    CREATE_FUNC(CutBreadScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(CutBreadScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(CutBreadScene));}
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
protected:
    virtual void _initData();
    void _hideShadow(DragNode* pNode);
    void _showShadow(DragNode* pNode);
    
    void _showPlate();
    void _showPlateRight();
    void _showKnife();
    
    void _finishCut();
    
    Sprite* m_pBoard;
    Sprite* m_pRag; //抹布
    
    Sprite* m_pPlateLeft;
    Sprite* m_pPlateRight;
    Sprite* m_pCrumbs;
    
    DragNode* m_pKnife;
    DragNode* m_pCurCutBread;
    
    std::vector<DragNode*> m_BreadLeftVector;
    std::vector<DragNode*> m_BreadRightVector;
    std::map<int,DragNode*> m_BreadBoardMap;
    
    int m_nMoveCount;
};
#endif /* CutBreadScene_h */
