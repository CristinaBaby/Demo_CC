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
#include "cocostudio/CCArmatureAnimation.h"


class  HomeScene : public Scene, public ParentalGateProtocol
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
    ActionInterval * quickButterflyAnimation(int animationIndex, const string &name, const Vec2 &pos);
    void onMovementEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);
    void beBack(){ _beBack = true;};
    void onDialog(const string& name);
    
protected:
    void quitGame();
    
private:
    Button* _play;
    Armature* _butterfly;
    bool     _dock = false;
    bool     _beBack = false;
    bool     _block = false;
};