
#ifndef ClassicRollScene_h
#define ClassicRollScene_h

#include "ExtensionScene.h"
#include "ChooseModelLayer.h"

class ClassicRollScene : public ExtensionScene
{
public:
    ClassicRollScene();
    ~ClassicRollScene();
    
    MY_SCENE(ClassicRollScene);
    CREATE_FUNC(ClassicRollScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _createRollUI();
    
    void _showRollTool();
    void _removeRollTool();
    
    void _showChooseModelLayer();
    void _onModelCallback(int tag);
    void _showCutTool(int index);
    
    void _finishRoll();
    void _finishCut();
    void _finish();
    
    Sprite* m_pBg;
    Sprite* m_pCloth;
    
    DragNode* m_pRollTool;
    DragNode* m_pCutTool;
    Sprite* m_pDough;
    
    std::vector<Vec2> m_ModelPosVector;
    std::vector<Sprite*> m_ModelVector;
    
    int m_nFlavorIndex;
    int m_nRollCount;
    int m_nCutCount;
    int m_nModuleIndex;
    
    Button* m_pShapeBtn;
};
#endif /* ClassicRollScene_h */
