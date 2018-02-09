
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
        Rect rect = Rect(145, 34, 450, 350);
        rect.origin = m_pBody->convertToWorldSpace(rect.origin);
        return rect;
    }
    CC_SYNTHESIZE_READONLY(bool, m_bIsOpen, IsOpen);
    CC_SYNTHESIZE(bool, m_bTouchEnable, TouchEnable);
    std::function<void(FreezerNode* )> onDoorCallback;
protected:
    Rect _getDoorTouchWorldRect(){
        Rect rect;
        if (m_bIsOpen) {
            rect = Rect(0, 0, 150, 350);
        }else{
            rect = Rect(145, 34, 450, 350);
        }
        rect.origin = m_pBody->convertToWorldSpace(rect.origin);
        return rect;
    }
    
    Sprite* m_pBody;
    Sprite* m_pBodyDoor;
    Sprite* m_pBodyDoorOpen;
    
};

#endif