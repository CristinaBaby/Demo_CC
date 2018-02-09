//
//  HomeScene.h
//
//
//  Created by luoxp on 3/18/15.
//
//

#pragma once

#include <cocos2d.h>
#include "UITools.h"
#include "../Model/AppConfigs.h"
#include "CBParentalGateLayer.h"
#include "../Module/STInAppBilling.h"
#include "cocostudio/CCArmatureAnimation.h"


class  HomeScene : public Scene, public ParentalGateProtocol, public STIABDelegate
{
public :
    virtual ~HomeScene();
    
    CREATE_FUNC(HomeScene);
    
    virtual bool init();
    void onButton(Ref* pSender, Widget::TouchEventType type);
    void settingAction();
    void moreAction();
    virtual void onEnterTransitionDidFinish() override;
    
    virtual void parentalGatePass(int tag);

    //STIABDelegate
    virtual void purchaseSuccessful(const char* pid);
    virtual void purchaseFailed(const char *pid, int errorCode);
    virtual void restoreSuccessful(const char* pid);
    virtual void restoreFailed(const char* pid, int errorCode);
    
    ActionInterval * quickButterflyAnimation(int animationIndex, const string &name, const Vec2 &pos);
    void onMovementEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);
    void beBack(){ _beBack = true;};
    
protected:
    void quitGame();
    void cancelQuitGame();
    
private:
    STInAppBilling _billing;
    Button* _playBtn;
    bool     _dock = false;
    bool     _beBack = false;
};