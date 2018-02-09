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
#include "cocosgui.h"

CB_BEGIN_NAMESPACE

class SettingLayer : public cocos2d::LayerColor
{
public:
    CREATE_FUNC(SettingLayer);
    ~SettingLayer();
    virtual bool init();
    void backAction(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);

protected:
    void bgmOn(bool isOn);
    void sfxOn(bool isOn);
};



CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBSettingLayer__) */
