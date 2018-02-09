
#ifndef __ChooseIceShapeScene__
#define __ChooseIceShapeScene__


#include "ExtensionScene.h"
#include "BlenderCup.h"
#include "JuiceMachine.h"

class ChooseIceShapeScene : public ExtensionScene
{
public:
    ChooseIceShapeScene();
    ~ChooseIceShapeScene();
    
    MY_SCENE(ChooseIceShapeScene);
    CREATE_FUNC(ChooseIceShapeScene);
    virtual bool init();
    virtual void onEnter();
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showModule();
    void _onItemCallback(LockItem* item);
    
    std::vector<LockItem*> m_modules;
    bool m_bItemTouchEnable;
};

#endif