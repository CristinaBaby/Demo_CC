
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
        Rect rect = Rect(65, 385, 375, 165);
        rect.origin = m_pBody->convertToWorldSpace(rect.origin);
        return rect;
    }
    CC_SYNTHESIZE_READONLY(bool, m_bIsOpen, IsOpen);
    CC_SYNTHESIZE(bool, m_bTouchEnable, TouchEnable);
    std::function<void(FreezerNode* )> onDoorCallback;
protected:
    
    Sprite* m_pBody;
    Sprite* m_pBodyFront;
    Sprite* m_pBodyDoor;
    Sprite* m_pBodyDoorOpen;
    
    Sprite* m_pLightIn;
    Sprite* m_pLightOut;
};

#endif