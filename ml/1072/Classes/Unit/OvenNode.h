
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
    void showControlFrame(){
        _showControlFrame();
        this->runAction(Sequence::create(DelayTime::create(0.5),
                                         CallFunc::create([=](){
            m_bControlable = true;
        }), NULL));
    }
    void doBake();
    void stopBake();
    
    Rect getDoorTouchWorldRect(){
        Rect rect;
        if (m_bIsOpen) {
            Rect rect = Rect(140, -100, 500, 200);
        }else{
            rect = Rect(140, 65, 500, 420);
        }
        rect.origin = m_pBody->convertToWorldSpace(rect.origin);
        return rect;
    }
    Rect getDoorWorldRect(){
        Rect rect = Rect(140, 65, 500, 420);
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
    
    void setOvenSettings(int temp,int duration);
    
    virtual bool ccTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void ccTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void ccTouchEnded(Touch *pTouch, Event *pEvent);
    virtual void ccTouchCancelled(Touch *pTouch, Event *pEvent);
protected:
    void _onButtonCallbak(Ref* ref,Widget::TouchEventType type);
    
    void _showControlFrame();
    void _hideControlFrame();
    
    Sprite* m_pBody;
    Sprite* m_pBodyDoor;
    Sprite* m_pBodyDoorOpen;
    Button* m_pSwitch;
    
    Node* m_pControlFrame;
    Sprite* m_pFrame;
    Sprite* m_pTempNode;
    ClippingNode* m_pTempClippingNode;
    LayerGradient* m_pTempColor;
    LayerColor* m_pTempMask;
    Sprite* m_pTempPoint;
    
    ClippingNode* m_pLightClipping;
    
    Sprite* m_pTimeNode;
    Sprite* m_pTimePoint;
    bool m_bIsBaking;
    
    bool m_bControlable;
    int m_nTimeRotation;
    int m_nTempRotation;
    float m_fScale;
    bool m_bTempReady;
    bool m_bTimeReady;
    Vec2 m_FramePos;
    
    float m_nTempPointer;
    float m_nTimePointer;
};

#endif