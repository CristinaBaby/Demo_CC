
#ifndef __OvenNode__
#define __OvenNode__


#include "cocos2d.h"
#include <stdio.h>
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class OvenNode : public Node
{
public:
    OvenNode();
    ~OvenNode();
    
    CREATE_FUNC(OvenNode);
    
    virtual bool init();
    
    
    void openDoor();
    void closeDoor();
    void doBake();
    void stopBake();
    
    Rect getDoorTouchWorldRect(){
        Rect rect;
        if (m_bIsOpen) {
            rect = Rect(0, 0, 480, 140);
        }else{
            rect = Rect(16, 91, 440, 310);
        }
        rect.origin = m_pBody->convertToWorldSpace(rect.origin);
        return rect;
    }
    Rect getDoorWorldRect(){
        Rect rect = Rect(16, 91, 440, 310);
        rect.origin = m_pBody->convertToWorldSpace(rect.origin);
        return rect;
    }
    Vec2 getSwitchWorldPos(){
        Vec2 pos = m_pSwitch->getPosition();
        pos = this->convertToWorldSpace(pos);
        return pos;
    }
    CC_SYNTHESIZE_READONLY(bool, m_bIsOpen, IsOpen);
    CC_SYNTHESIZE(bool, m_bTouchEnable, TouchEnable);
    CC_SYNTHESIZE(bool, m_bBaked, Baked);
    void setSwitchEnable(bool enable){
        if (enable) {
            if(!m_bIsBaking){
                m_pSwitch->setTouchEnabled(enable);
            }
        }else{
            m_pSwitch->setTouchEnabled(enable);
        }
    }
    
    void reset(){
        m_bIsOpen = false;
        m_bTouchEnable = false;
        m_bIsBaking = false;
        m_pBodyDoorOpen->setVisible(false);
        m_bBaked = false;
        m_pSwitch->setTouchEnabled(false);
    }
    std::function<void(OvenNode* )> onDoorCallback;
    std::function<void(OvenNode* ,int state)> onBakeCallback; //0 begin  1 end
protected:
    void _onButtonCallbak(Ref* ref,Widget::TouchEventType type);
    
    Sprite* m_pBody;
    Sprite* m_pBodyDoor;
    Sprite* m_pBodyDoorOpen;
    Button* m_pSwitch;
    bool m_bIsBaking;
};

#endif