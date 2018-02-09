
#ifndef FriesCutPotatoScene_h
#define FriesCutPotatoScene_h

#include "ExtensionScene.h"

class FriesCutPotatoScene : public ExtensionScene
{
public:
    FriesCutPotatoScene();
    ~FriesCutPotatoScene();
    
    MY_SCENE(FriesCutPotatoScene);
    CREATE_FUNC(FriesCutPotatoScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _showBoard();
    void _showPotatoPlate();
    void _showLeftPlate();
    void _showKnife();
    void _finish();
    
    Sprite* m_pBg;
    
    Sprite* m_pBoard;
    Sprite* m_pPlateRight;
    Sprite* m_pPlateLeft;
    Sprite* m_pPotatoRight;
    DragNode* m_pPotato;
    DragNode* m_pKnife;
    Sprite* m_pChipLeft;
    Node* m_pChipNode;
    Sprite* m_pChipRight;
    int m_nCutCount;
    int m_nCutStep;
};
#endif /* FriesCutPotatoScene_h */
