
#ifndef __AddJuiceScene__
#define __AddJuiceScene__


#include "ExtensionScene.h"
#include "BlenderCup.h"
#include "JuiceMachine.h"

class AddJuiceScene : public ExtensionScene
{
public:
    AddJuiceScene();
    ~AddJuiceScene();
    
    MY_PHYSIC_SCENE(AddJuiceScene);
    CREATE_FUNC(AddJuiceScene);
    enum{
        eDragTagFlavorBottle,
        eDragTagStick,
    };
    virtual bool init();
    virtual void onEnter();
    void update(float dt = 0.0f);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    void onSwitchStateCallback(JuiceMachine* ,bool on);
protected:
    virtual void _initData();
    void _showMachines();
    void _showBlenderCup();
    void _pourIce();
    
    void _onScrollCallback(ExpandScrollView* ,int);
    BlenderCup* m_pCup;
    ExpandScrollView* m_pMachineScrollView;
    std::vector<JuiceMachine*> m_MachineVector;
    int m_nFlavorIndex;
    int m_nShapeIndex;
    int m_nIceCount;
    bool m_bReady;
    bool m_bFull;
};

#endif