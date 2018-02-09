
#ifndef __MachineNode__
#define __MachineNode__


#include "cocos2d.h"
#include <stdio.h>
#include "cocostudio/CCArmature.h"

using namespace cocostudio;

USING_NS_CC;

class MachineNode : public Node
{
public:
    MachineNode();
    ~MachineNode();
    
    CREATE_FUNC(MachineNode);
    
    virtual bool init();
    
    
    void addUnit(int index); //0 body  1 bowl  2 cone  3 stick  4 cover

    Rect getBowlWorldRect(){
        Rect rect = m_pBowl->getBoundingBox();
        rect.origin = this->convertToWorldSpace(rect.origin);
        return rect;
    }
    CC_SYNTHESIZE_READONLY(bool, m_bIsOpen, IsOpen);
    CC_SYNTHESIZE(bool, m_bTouchEnable, TouchEnable);
    std::function<void()> onFinishedCallback;
    std::function<void()> onBeginCallback;
    void updateIce(float);
protected:
    
    Sprite* m_pBody;
    Sprite* m_pBodyFront;
    Sprite* m_pBowl;
    Sprite* m_pBowlFront;
    Sprite* m_pCover;
    Sprite* m_pStick;
    
    Sprite* m_pCone;
    Sprite* m_pConeFront;
    Sprite* m_pIce;
    Armature* m_pCoverArmature;
    
    Sprite* m_pMask;
    ClippingNode* m_pIceClipping;
    
    bool m_bTouched;
    bool m_bFinished;
};

#endif