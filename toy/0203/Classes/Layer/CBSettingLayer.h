//
//  CBSettingLayer.h
//  ColorBook
//
//  Created by maxiang on 4/21/15.
//
//

#ifndef __ColorBook__CBSettingLayer__
#define __ColorBook__CBSettingLayer__

#include "cocos2d.h"
#include "../Model/CBAppGlobal.h"
#include "KDKeyboardDispatcher.h"

CB_BEGIN_NAMESPACE

class SettingLayer : public cocos2d::LayerColor,public KDKeyboardDispatcherDelegate
{
public:
    CREATE_FUNC(SettingLayer);
    
    virtual bool init() override;
    void backAction();
    
    virtual bool onKeyReleased() override;
    void onExit() override;
    void onEnter() override;

protected:
    void bgmOn(bool isOn);
    void sfxOn(bool isOn);
};



CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBSettingLayer__) */
