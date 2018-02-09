
#ifndef __CottonMachine__
#define __CottonMachine__


#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class CottonMachine : public Node
{
public:
    CottonMachine();
    ~CottonMachine();
    
    CREATE_FUNC(CottonMachine);
    
    virtual bool init();
    
    void onExit();
    void setSwitchEnable(bool enable = true){
        m_pSwitch->setEnabled(enable);
    }
    Vec2 getButtonWorldPosition(){
        return m_pSwitch->getParent()->convertToWorldSpace(m_pSwitch->getPosition());
    }
    void onButtonCallback(Ref*,Widget::TouchEventType);
    
    void addSugar(int index,bool animate = true);
    std::function<void(bool)> onSwitchTurnOn;
    bool makeCotton();
    ClippingNode* m_pOutClp;
    
    void turnOn(){
        m_bIsOn = true;
        _turnOn(true);
    }
protected:
    void _turnOn(bool on);
    
    void _makeSilk();
    ClippingNode* m_pMiddleClp;
    
    Sprite* m_pMaskMiddle;
    Sprite* m_pMaskOut;
//    Sprite* m_pShadow;
    Sprite* m_pBodyBack;
    Sprite* m_pBodyMiddle;
    Sprite* m_pBodyFront;
    Button* m_pSwitch;
    Sprite* m_pSilkFront;
    Sprite* m_pSilkBack;
    bool m_bIsOn;
    int m_nCottonStep;
    
    int m_nFlavor;
};

#endif