
#ifndef __MachineNode__
#define __MachineNode__


#include "cocos2d.h"
#include <stdio.h>
#include "cocostudio/CCArmature.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

using namespace cocostudio;

USING_NS_CC;

class MachineNode : public Node
{
public:
    MachineNode();
    ~MachineNode();
    
    CREATE_FUNC(MachineNode);
    
    virtual bool init();
    
    enum{
        eStateCapOpen,
        eStateCapClose,
        eStateTurnOn,
        eStateTurnOff,
        eStateFinish,
    };
    void addUnit(int index); //0 body  1 bowl  2 cone  3 stick  4 cover

    Rect getBoxWorldRect(){
        Rect rect = m_pBox->getBoundingBox();
        rect.origin = m_pBox->getParent()->convertToWorldSpace(rect.origin);
        return rect;
    }
    Rect getCapWorldRect(){
        Rect rect = m_pCap->getBoundingBox();
//        if (!m_bIsOpen) {
//            rect.origin.x += m_pCap->getContentSize().width;
//        }
        rect.origin = this->convertToWorldSpace(rect.origin);
        return rect;
    }
    void setIcePercent(int percent);
    
    CC_SYNTHESIZE_READONLY(bool, m_bIsOn, IsOn);
    CC_SYNTHESIZE_READONLY(bool, m_bIsOpen, IsOpen);
    CC_SYNTHESIZE(bool, m_bTouchEnable, TouchEnable);
    CC_SYNTHESIZE(bool, m_bButtonEnable, ButtonEnable);
    std::function<void()> onFinishedCallback;
    std::function<void()> onBeginCallback;
    std::function<void(int state)> onStateCallback;
    void updateIce(float);
    void addIce(Sprite* pIce,Vec2 wolrdPos);
    void getIce(Sprite* pSpoon,std::function<void()> callback);
    void separateBox();
protected:
    void _onButtonCallback(Ref* ref,Widget::TouchEventType type);
    void _doWork();
    void _stopWork();
    
    Sprite* m_pBody;
    Sprite* m_pBodyFront;
    Sprite* m_pBox;
    Sprite* m_pBoxFront;
    Sprite* m_pIce;
    Sprite* m_pMask;
    Node* m_pBoxNode;
    ClippingNode* m_pIceClipping;
    
    Sprite* m_pCap;
    
    Button* m_pButton;
    
    Sprite* m_pBowl;
    Sprite* m_pBowlFront;
    Sprite* m_pCover;
    Sprite* m_pStick;
    
    Sprite* m_pCone;
    Sprite* m_pConeFront;
    Armature* m_pCoverArmature;
    
    
    bool m_bTouched;
    bool m_bFinished;
};

#endif