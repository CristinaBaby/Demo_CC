
#ifndef __OvenNode__
#define __OvenNode__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <stdio.h>

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
    };
    void openDoor();
    void closeDoor();
    void reset(){
        Node* pSmoke = this->getChildByName("smoke");
        if(pSmoke){
            pSmoke->removeFromParent();
        }
        m_pSwitch->setTouchEnabled(false);
        m_pSwitch->setPosition(Vec2(20, 58));
        m_pSwitch->loadTextures("content/make/bake/off.png", "content/make/bake/off.png","content/make/bake/off.png");
        if (m_pDoorOpen) {
            m_pDoorOpen->setVisible(false);
        }
        m_pDoor->setTexture("content/make/bake/door0.png");
        setDoorTouchEnable();
        m_eState = State::eStateDoorClose;
        m_bIsWork = false;
        m_bIsOpen = false;
        m_bCanTouch = true;
        m_bBaked = false;
    }
    void setSwitchEnable(bool enable = true){
        m_pSwitch->setTouchEnabled(enable);
    }
    void setDoorTouchEnable(bool enable = true){
        m_bCanTouch = enable;
    }
    
    OvenNode::State state(){
        return m_eState;
    }
    
    Rect getDoorWorldRect(){
        Rect rect = Rect(75, 140, 400, 330);
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
    Sprite* m_pBody;
    Sprite* m_pDoor;
    Sprite* m_pDoorOpen;
    Sprite* m_pPotLeft;
    Sprite* m_pPotRight;

    Button* m_pSwitch;
    OvenNode::State m_eState;
    bool m_bBaked; //已经烘焙
    bool m_bIsOpen; //door是否被打开
    bool m_bIsWork;  //oven正在工作中
    bool m_bCanTouch; //是否可以点击烤箱 开关烤箱门（烤箱在开／关是不允许进行 关／开操作）
};

#endif