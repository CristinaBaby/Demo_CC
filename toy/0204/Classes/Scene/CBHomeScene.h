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
#include "cocosgui.h"
#include "CBQuitGameLayer.h"

CB_BEGIN_NAMESPACE

enum ButtonTag{
    playTag = 111,
    settingTag,
    moreTag,
    restoreTag
};

class HomeScene : public cocos2d::Layer, public STIABDelegate, public ParentalGateProtocol
{
    virtual void onEnterTransitionDidFinish() override;
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
    void touchDown(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);

    //STIABDelegate
    virtual void purchaseSuccessful(const char* pid);
    virtual void purchaseFailed(const char *pid, int errorCode);
    virtual void restoreSuccessful(const char* pid);
    virtual void restoreFailed(const char* pid, int errorCode);
    
    //parental gate
    virtual void parentalGatePass(int tag);
    
protected:
    void quitGame();
    void cancelQuitGame();
    
protected:
    bool _isPurchasing;
    STInAppBilling _billing;
    
    cocos2d::Layer  * _uiLayer;
    QuitGameLayer   * _quit = nullptr;
};



CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBHomeScene__) */
