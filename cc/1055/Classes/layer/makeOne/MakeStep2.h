//
//  MakeStep22.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/3/31.
//
//

#ifndef MakeStep22_hpp
#define MakeStep22_hpp

#include <stdio.h>
#include "BaseLayer.h"
#include "MainScene.h"
#include "PercentChangeComponent.h"
#include "DeviceInfo.h"
#include "InertiaComponent.h"
//make步骤的模板
class MakeStep2 : public BaseLayer  {
public:
    MakeStep2();
    ~MakeStep2();
public:
    bool init();
    static MakeStep2* create();
//    CREATE_FUNC(MakeStep2);
//    CREATE_SCENE(MakeStep2);
    
public:
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
private:
    virtual void touchEnd(ui::Widget* widget);
    
    void onTouchDown(Node* node,MoveLisenter* lisenter);
    void onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index);
    void onTouchUp(Node* node,MoveLisenter* lisenter);
    void onTouchMix(Node* node,MoveLisenter* lisenter,Touch* touch);
    void showBowl();
    void showSpoon();
    
    Sprite* m_pBowl;
    Sprite* m_pBowlCover;
    Sprite* m_pSpoon;
    
    //
    void initData();
    void finish(LQComponent*,OperateListner* lis);
    void inertiaInvoking(LQComponent*,OperateListner* lis);
    void inertiaing(LQComponent*,OperateListner* lis);
    void shake(float distance ,float time);
    void caculateShake(float);
    void caculateFastShake(float);
    void caculateShakeBad(float);

    
public:
    std::function<void()> nextStep = nullptr;
    std::function<void()> resetLayer = nullptr;
protected:
    bool isShake,isFastShake,isFinish;
    Vec2 startPos;
//    TouchMoveComponent* move;
    static const int shakeAction;
    
    InertiaComponent* _inertia;
    PercentChangeComponent* _change;
    MoveLisenter* _lisenter;
    
    bool m_bIsSound;
    bool m_bIsVoice;
    bool m_bIsMix;
};


#endif /* MakeStep22_hpp */
