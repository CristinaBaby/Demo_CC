//
//  CBSwitch.h
//  ColorBook
//
//  Created by maxiang on 4/22/15.
//
//

#ifndef __ColorBook__CBSwitch__
#define __ColorBook__CBSwitch__

#include "cocos2d.h"
#include "../Model/CBAppGlobal.h"

CB_BEGIN_NAMESPACE

typedef std::function<void(bool)> SwichAction;

class Switch : public cocos2d::Layer
{
public:
    CREATE_FUNC(Switch);
    
    virtual bool init();
    
    void setOffBackground(const std::string& name);
    void setOnBackground(const std::string& name);
    void setSlider(const std::string& name);
    
    void on(bool animation);
    void off(bool animation);
    
    void setSwitchAction(const SwichAction& action);
    
protected:
    void callback();
protected:
    cocos2d::Sprite *_offBackground;
    cocos2d::Sprite *_onBackground;
    cocos2d::Sprite *_slider;
    bool    _isOn;
    SwichAction _switchAction;
};



CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBSwitch__) */
