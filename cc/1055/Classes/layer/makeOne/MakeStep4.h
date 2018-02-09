//
//  MakeStep44.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/3/31.
//
//

#ifndef MakeStep44_hpp
#define MakeStep44_hpp

#include <stdio.h>
#include "BaseLayer.h"
#include "MainScene.h"
#include "PercentChangeComponent.h"
#include "DeviceInfo.h"
#include "InertiaComponent.h"
//make步骤的模板
class MakeStep4 : public BaseLayer  {
public:
    MakeStep4();
    ~MakeStep4();
public:
    bool init();
    static MakeStep4* create();
//    CREATE_FUNC(MakeStep4);
//    CREATE_SCENE(MakeStep4);
    
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
    Sprite* m_pSpoon;
    Sprite* m_pBowlCover;
    Vector<Sprite*> m_vSpoon;
    bool m_bIsChooseSpoon;
    bool m_bIsAction;
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
    bool m_bIsFirst;
};

#endif /* MakeStep44_hpp */
