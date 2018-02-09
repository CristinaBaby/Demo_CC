
#ifndef __FreezerNode__
#define __FreezerNode__


#include "cocos2d.h"
#include <stdio.h>

USING_NS_CC;

class FreezerNode : public Node
{
public:
    FreezerNode();
    ~FreezerNode();
    
    CREATE_FUNC(FreezerNode);
    
    virtual bool init();
    
    
    void openDoor();
    void closeDoor();
    void doFreeze();
    void stopFreeze();
    
    Rect getDoorWorldRect(){
        Rect rect = Rect(265, 88, 450, 330);
        rect.origin = m_pBody->convertToWorldSpace(rect.origin);
        return rect;
    }
    Rect getDoorTouchWorldRect(){
        Rect rect;
        if (m_bIsOpen) {
            rect = Rect(0, 0, 250, 400);
        }else{
            rect = Rect(265, 88, 450, 330);
        }
        rect.origin = m_pBody->convertToWorldSpace(rect.origin);
        return rect;
    }
    void reset(){
        m_bIsOpen = false;
        m_bTouchEnable = false;
        m_pBodyDoorOpen->setVisible(false);
        m_bFreezed = false;
    }
    CC_SYNTHESIZE_READONLY(bool, m_bIsOpen, IsOpen);
    CC_SYNTHESIZE(bool, m_bTouchEnable, TouchEnable);
    CC_SYNTHESIZE(bool, m_bFreezed, Freezed);
    std::function<void(FreezerNode* )> onDoorCallback;
protected:
    
    Sprite* m_pBody;
    Sprite* m_pBodyDoor;
    Sprite* m_pBodyDoorOpen;
    
};

#endif