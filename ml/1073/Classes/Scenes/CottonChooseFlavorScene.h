
#ifndef __CottonChooseFlavorScene__
#define __CottonChooseFlavorScene__


#include "ExtensionScene.h"
#include "SugarBottle.h"



class CottonChooseFlavorScene : public ExtensionScene
{
public:
    CottonChooseFlavorScene();
    ~CottonChooseFlavorScene();
    
    MY_SCENE(CottonChooseFlavorScene);
    CREATE_FUNC(CottonChooseFlavorScene);
    
    virtual bool init();
    
    virtual void onButtonCallback(Button* btn);
    void onItemClicked(LockItem* item);
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _showScrollView();
    ExpandScrollView* m_pScrollView;
    
    bool m_bSelected;
    LockItem* m_pCurItem;
    Vector<LockItem*> m_ItemVector;
};

#endif