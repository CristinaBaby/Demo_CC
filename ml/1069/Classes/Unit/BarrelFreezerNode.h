
#ifndef __BarrelFreezerNode__
#define __BarrelFreezerNode__


#include "cocos2d.h"
#include <stdio.h>
#include "cocostudio/CCArmature.h"
#include "ui/CocosGUI.h"
#include "CocoStudioHelper.h"

using namespace cocos2d::ui;

using namespace cocostudio;

USING_NS_CC;

class BarrelFreezerNode : public Node
{
public:
    BarrelFreezerNode();
    ~BarrelFreezerNode();
    
    CREATE_FUNC(BarrelFreezerNode);
    
    virtual bool init();
    
    enum{
        eStateOpen,
        eStateAddBatter,
        eStateCovor,
        eStateFinish,
    };
    
    Rect getBarrelWorldRect(){
        Rect rect = m_pBodyFront->getBoundingBox();
        rect.origin = m_pBodyFront->getParent()->convertToWorldSpace(rect.origin);
        return rect;
    }

    Rect getBatterHandleWorldRect(){
        Vec2 point = CocoStudioHelper::getBoneWorlPoint(m_pHandleArmature, "Layer18");
        Rect rect = Rect(point.x, point.y-50, 100, 100);
        return rect;
    }
    CC_SYNTHESIZE(bool, m_bTouchEnable, TouchEnable);
    std::function<void()> onFinishedCallback;
    std::function<void()> onHandleCallback;
    
    void setState(int state);
protected:
    void _freeze(float dt= 0.0f);
    
    Sprite* m_pBody;
    Sprite* m_pBodyFront;
    Node* m_pInnerNode;
    Node* m_pLidNode;
    
    Sprite* m_pRing;
    Sprite* m_pRingUp;
    
    Sprite* m_pCover;
    Sprite* m_pCoverFront;
    
    Sprite* m_pBatter;
    Sprite* m_pBatterFront;
    
    Button* m_pButton;
    
    Armature* m_pHandleArmature;
    
    Node* m_pIceBack;
    Node* m_pIceFront;
    
    bool m_bTouched;
    bool m_bFinished;
    int m_nFreezeIndex;
};

#endif