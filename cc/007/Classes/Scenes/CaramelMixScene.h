
#ifndef CaramelMixScene_h
#define CaramelMixScene_h

#include "ExtensionScene.h"
#include "MixPoint.h"
#include "InertiaComponent.h"
#include "LQComponent.h"

class CaramelMixScene : public ExtensionScene
{
public:
    CaramelMixScene();
    ~CaramelMixScene();
    
    MY_SCENE(CaramelMixScene);
    CREATE_FUNC(CaramelMixScene);
    
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
    
    void _showBowl();
    void _showMixTools();
    void _showProgressBar();
    void _showBar(bool show = true);
    
    DragNode* _createContainer(int index,bool water = false);
    void _showIngredient(int index);
    void _addIngredient(DragNode* pNode);
    
    void _saveMixture();
    void _checkMoveSpeed();
    void _mixBatter();
    void _mixBad();
    void _finishMix();
    void _ingredientOut(DragNode* pNode);
    void _toolElectricWork(bool working = true);
    
    void _finishAdd();
    
    void inertiaInvoking(LQComponent*,OperateListner* lis);
    void inertiaing(LQComponent*,OperateListner* lis);
    
    void _slow();
    void _perfect();
    void _fast();
    
    void caculateShake(float);
    void caculateFastShake(float);
    void caculateShakeBad(float);
    bool isPlayFast;
    
    Sprite* m_pBg;
    
    Sprite* m_pBowl;
    MixPoint* m_pMixture;
    Node* m_pBatter;
    
    DragNode* m_pSugar;
    DragNode* m_pFlour;
    
    DragNode* m_pMixTool_normal;
    DragNode* m_pMixTool_electric;
    DragNode* m_pCurTool;
    
    ProgressBar* m_pBar;
    
    Vector<DragNode*> m_IngredientVector;
    
    int m_nStep;
    int m_nMixCount;
    int m_nRotateCount;
    int m_nMixTimeCount;
    bool m_bTemp;
    
    
    InertiaComponent* _inertia;
    float loadingPercent;
    bool isShake,isFastShake,isFinish;
};
#endif /* CaramelMixScene_h */
