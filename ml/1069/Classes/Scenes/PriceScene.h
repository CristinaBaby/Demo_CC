
#ifndef __PriceScene__
#define __PriceScene__


#include "ExtensionScene.h"
#include "SetPriceLayer.h"
#include "SaleManager.h"

class PriceScene : public ExtensionScene
{
public:
    PriceScene();
    ~PriceScene();
    
    MY_SCENE(PriceScene);
    CREATE_FUNC(PriceScene);
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(PriceScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(PriceScene));}
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void onButtonCallback(Button* btn);
    void onPiceCallback(Ref*,Widget::TouchEventType);
    void onPriceSetCallback(float price);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
protected:
    virtual void _initData();
    
    
    DragNode* _addPizza(int index);
    void _showPriceTag();
    void _updatePrice(int index,float price);
    void _showSetPriceLayer();
    void _saveData();
    void _readyToSale();
    void _beginiveCoint();
    void _giveCoint();
    void _produceGuese();
    void _pickPizza();
    
    void _showParticle();
    Layer* m_pShowCase;
    DragNode* m_pPizza;
    
    std::vector<FoodData> m_FoodDataVector;
    Vector<DragNode*> m_PizzaVector;
    Vector<Button*> m_PizzaPriceVector;
    
    std::string getStoragePath(std::string name){
        std::string path;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        path = SSCFileUtility::getStoragePath()+ name;
#else
        path = SSCFileUtility::getStoragePath()+"/"+ name;
#endif
        return path;
    }
    int m_nEmptyIndex;
    int m_nCurPriceSetIndex;
    bool m_bPriceTagEnable;
    std::string m_sPath;
    
    std::vector<int> m_PizzaPathIndex;
    
    bool m_bOnSale[8];
    
    RoleModel* m_pGirl;
    RoleModel* m_pGuest;
    Vec2 m_PriceTagPos;
    bool m_bReadySale;
    bool m_bGirlShown;
    int m_nSaleIndex;
    int m_nSaleCount;
    bool m_bExpensive;
    bool m_bFinish;
};

#endif