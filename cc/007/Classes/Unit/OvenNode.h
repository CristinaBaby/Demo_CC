
#ifndef __OvenNode__
#define __OvenNode__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <stdio.h>
#include "AudioHelp.h"

USING_NS_CC;
using namespace cocos2d::ui;

class OvenNode : public Node
{
public:
    OvenNode();
    ~OvenNode();
    
    CREATE_FUNC(OvenNode);
    
    virtual bool init();
    
    enum class State{
        eStateDoorOpen,
        eStateDoorClose,
        eStateDoorOpenBaked,
        eStateDoorCloseBaked,
        eStateTurnOn,
        eStateTurnOff,
        eStateOK,
    };
    void openDoor();
    void closeDoor();
    void reset(){
        m_pSwitch->setTouchEnabled(false);
        m_pSwitch->loadTextures("res/oven/off.png", "res/oven/off.png","res/oven/off.png");
        
        m_pDoorOpen->setVisible(true);
        m_pDoor->setVisible(false);
        
        m_pDoor->setTexture("res/oven/oven_door_close.png");
        setDoorTouchEnable(false);
        m_eState = State::eStateDoorOpen;
        m_bIsWork = false;
        m_bIsOpen = true;
        m_bCanTouch = false;
        m_bBaked = false;
    }
    void setSwitchEnable(bool enable = true){
        m_pSwitch->setTouchEnabled(enable);
    }
    void setDoorTouchEnable(bool enable = true){
        m_bCanTouch = enable;
    }
    
    void putinFood(Node* pNode);
    
    void bakeOK(){
        if (onOvenStateCallback) {
            onOvenStateCallback(State::eStateOK);
        }
        AudioHelp::getInstance()->playEffect("baking_ok.mp3");
        m_pDoor->stopAllActions();
        AudioHelp::getInstance()->stopLoopEffect();
        m_pDoor->setTexture("res/oven/oven_door_close.png");
        
    }
    OvenNode::State state(){
        return m_eState;
    }
    
    Rect getDoorWorldRect(){
        Rect rect = Rect(120, 285, 470, 330);
        rect.origin = m_pBody->convertToWorldSpace(rect.origin);
        return rect;
    }
    
    Vec2 getSwitchWorldPosition()
    {
        Vec2 location = m_pSwitch->getPosition();
        location = this->convertToWorldSpace(location);
        return location;
    }
    std::function<void(OvenNode::State)> onOvenStateCallback;
    
    void onSwitchCallback(Ref*,Widget::TouchEventType);
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
protected:
    Node* m_pOven;
    Sprite* m_pBody;
    Sprite* m_pDoor;
    Sprite* m_pDoorOpen;
    Sprite* m_pPoint;

    Button* m_pSwitch;
    OvenNode::State m_eState;
    bool m_bBaked; //已经烘焙
    bool m_bIsOpen; //door是否被打开
    bool m_bIsWork;  //oven正在工作中
    bool m_bCanTouch; //是否可以点击烤箱 开关烤箱门（烤箱在开／关是不允许进行 关／开操作）
};

#endif