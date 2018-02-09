
#ifndef CaramelMeltSugarScene_h
#define CaramelMeltSugarScene_h

#include "ExtensionScene.h"
#include "MixPoint.h"

class CaramelMeltSugarScene : public ExtensionScene
{
public:
    CaramelMeltSugarScene();
    ~CaramelMeltSugarScene();
    
    MY_SCENE(CaramelMeltSugarScene);
    CREATE_FUNC(CaramelMeltSugarScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showShadow(int index,bool show = true);
    void _frozeIngredients(int index,bool froze = true);
    
    void _showStove();
    void _showBowl();
    void _showMixTool();
    
    DragNode* _createContainer(int index,bool water = false);
    void _showIngredient(int index);
    void _addIngredient(DragNode* pNode);
    
    void _mixBatter();
    
    void _finishMix();
    void _ingredientOut(DragNode* pNode);
    
    Sprite* m_pBg;
    Sprite* m_pBowl;
    MixPoint* m_pMixture;
    
    DragNode* m_pSyrup;
    DragNode* m_pBrownSugar;
    DragNode* m_pButter;
    
    DragNode* m_pMixTool;
    
    Vector<DragNode*> m_IngredientVector;
    
    int m_nStep;
    int m_nMixCount;
    int m_nRotateCount;
};
#endif /* CaramelMeltSugarScene_h */
