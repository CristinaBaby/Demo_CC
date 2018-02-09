
#ifndef EggFryScene_h
#define EggFryScene_h

#include "ExtensionScene.h"
#include "ScribbleNode.h"
#include "EggModel.h"
#include "FriedUnit.h"

class EggFryScene : public ExtensionScene
{
public:
    EggFryScene();
    ~EggFryScene();
    
    MY_SCENE(EggFryScene);
    CREATE_FUNC(EggFryScene);
    
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
    
    void _showStove();
    void _showPan();
    void _showModel();
    void _showShovel();
    void _showPlate();
    void _showIngredient(int index);
    void _addIngredient(DragNode* pNode);
    void _ingredientOut(DragNode* pNode);
    void _showBacons();
    
    void _paintButter();
    void _doFryEgg();
    void _finishFry();
    void _doFryBacon(FriedUnit* pBacon);
    void _finishFryBacon();
    void _finish();
    
    Sprite* m_pStove;
    Sprite* m_pFire;
    Sprite* m_pPan;
    Sprite* m_pPanFront;
    Sprite* m_pPlate;
    
    Sprite* m_pBaconPlate;
    std::vector<Sprite*> m_pBaconVector;
    std::vector<FriedUnit*> m_pBaconFriedVector;
    DragNode* m_pClip;
    
    Node* m_pPanIn;
    DragNode* m_pButter;
    DragNode* m_pShovel;
    
    ScribbleNode* m_pButterScribble;
    
    std::vector<DragNode*> m_pModelVector;
    std::vector<EggModel*> m_pEggVector;
    std::vector<Sprite*> m_pEggFriedVector;
    int m_nShapeIndex;
    int m_nStep;
    int m_nEggCount;
    int m_nMoveCount;
    bool m_bEffectTurn;
    bool m_bEffectBaconOK;
};
#endif /* EggFryScene_h */
