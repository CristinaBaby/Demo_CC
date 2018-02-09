
#ifndef __ChooseCupScene__
#define __ChooseCupScene__


#include "ExtensionScene.h"
#include "BlenderCup.h"
#include "JuiceMachine.h"
#include "CupLayer.h"

class ChooseCupScene : public ExtensionScene
{
public:
    ChooseCupScene();
    ~ChooseCupScene();
    
    MY_SCENE(ChooseCupScene);
    CREATE_FUNC(ChooseCupScene);
    virtual bool init();
    virtual void onEnter();
    
    virtual void onButtonCallback(Button* btn);
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    ExpandScrollView* m_pCupScrollView;
    void _showCups();
    void _onItemCallback(LockItem* item);
    
    std::vector<LockItem*> m_Cups;
    CupLayer* m_pCupLayer;
    
    Sprite* m_pOK;
    bool m_bItemTouchEnable;
};

#endif