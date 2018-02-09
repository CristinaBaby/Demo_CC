//
//  CBDisclaimerLayer.h
//  ColorBook
//
//  Created by maxiang on 4/30/15.
//
//

#ifndef __ColorBook__CBDisclaimerLayer__
#define __ColorBook__CBDisclaimerLayer__

#include "cocos2d.h"
#include "../Model/CBAppGlobal.h"
#include "../Layer/CBParentalGateLayer.h"

CB_BEGIN_NAMESPACE


class DisclaimerLayer : public cocos2d::LayerColor, public ParentalGateProtocol
{
public:
    CREATE_FUNC(DisclaimerLayer);
    
    virtual bool init();

    //action
    void playAction();
    void unlockAllAction();
    void termsOfUseAction();
    
    //parental gate
    virtual void parentalGatePass(int tag);
};



CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBDisclaimerLayer__) */
