
#ifndef __JuiceMachine__
#define __JuiceMachine__


#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class JuiceMachine : public Node
{
public:
    JuiceMachine();
    ~JuiceMachine();
    
    CREATE_FUNC(JuiceMachine);
    
    virtual bool init();
    void setFlavor(int index);
    
    void onExit();
    void setSwitchEnable(bool enable = true){
        m_pSwitch->setEnabled(enable);
    }
    Vec2 getButtonWorldPosition(){
        return m_pSwitch->getParent()->convertToWorldSpace(m_pSwitch->getPosition());
    }
    Vec2 getOutletWorldPosition(){
        return m_pOutlet->convertToWorldSpace(Vec2(28, 0));
    }
    void onButtonCallback(Ref*,Widget::TouchEventType);
    
    std::function<void(JuiceMachine*,bool)> onSwitchStateCallback;
    
    void turnOn(){
        m_bIsOn = true;
        _turnOn(true);
    }
    void turnOff(){
        m_bIsOn = false;
        _turnOn(false);
    }
protected:
    void _turnOn(bool on);
    
    ClippingNode* m_pClippingNode;
    
    Sprite* m_pBodyBack;
    Sprite* m_pBodyFront;
    Button* m_pSwitch;
    NodeGrid* m_pWaterGrid;
    Sprite* m_pJuice;
    Sprite* m_pOutlet;
    Sprite* m_pFlavorTag;
    
    Vec2 m_PosSwitch;
    bool m_bIsOn;
    
    int m_nFlavor;
};

#endif