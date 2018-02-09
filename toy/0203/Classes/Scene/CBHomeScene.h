//
//  CBHomeScene.h
//  ColorBook
//
//  Created by maxiang on 4/21/15.
//
//

#ifndef __ColorBook__CBHomeScene__
#define __ColorBook__CBHomeScene__

#include "cocos2d.h"
#include "../Model/CBAppGlobal.h"
#include "../Module/STInAppBilling.h"
#include "../Layer/CBParentalGateLayer.h"
#include "KDKeyboardDispatcher.h"

CB_BEGIN_NAMESPACE

class HomeScene : public cocos2d::Layer, public STIABDelegate, public ParentalGateProtocol,KDKeyboardDispatcherDelegate
{
public:
    //create scene
    CREATE_SCENE_FUNC(HomeScene);
    
    //create func
    CREATE_FUNC(HomeScene);
    
    virtual ~HomeScene();
    
    virtual bool init();
    
    void playAction();
    void settingAction();
    void moreAction();
    void policyAction();
    void restoreAction();

    //STIABDelegate
    virtual void purchaseSuccessful(const char* pid);
    virtual void purchaseFailed(const char *pid, int errorCode);
    virtual void restoreSuccessful(const char* pid);
    virtual void restoreFailed(const char* pid, int errorCode);
    
    void onExit() override;
    
    void onEnter() override;
    
    //parental gate
    virtual void parentalGatePass(int tag);
    
    virtual bool onKeyReleased();
    

protected:
    void quitGame();
    void cancelQuitGame();
    
protected:
    bool _isPurchasing;
    STInAppBilling _billing;
};



CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBHomeScene__) */
