//
//  CBSwitch.cpp
//  ColorBook
//
//  Created by huangwen on 4/22/15.
//
//

#include "Switch.hpp"
#include "UICpp.h"
#include "UITools.h"

USING_NS_CC;
#define PINK            Color3B(150, 97, 59)
#define GRAY            Color3B(187, 187, 187)
#define DEFUALT_VALUE   0.2

bool Switch::init()
{
    if (!Layer::init()) {
        return false;
    }
    _bgmValue = 0;
    _effValue = 0;
    
    bool rcode = false;
    
    do {
        
        //set content size to background
        setAnchorPoint(Point::ZERO);
        ignoreAnchorPointForPosition(false);
        
        
        rcode = true;
    } while (0);
    
    return rcode;
}

void Switch::setSwitchAction(const SwichAction& action)
{
    _switchAction = action;
}

void Switch::bgmAction(){
    
    log("_bgmValue = %d",_bgmValue);
    for (int i = 0; i < b_image.size(); ++i) {
        if (i < _bgmValue) {
            b_image.at(i)->setColor(PINK);
        }
        else
        {
            b_image.at(i)->setColor(GRAY);
        }
    }
}

void Switch::effectAction(){
    for (int i = 0; i < e_image.size(); ++i) {
        if (i < _effValue) {
            e_image.at(i)->setColor(PINK);
        }
        else
        {
            e_image.at(i)->setColor(GRAY);
        }
    }
}

void Switch::Bgm(bool tag)
{
    xAudio->playDefaultSound();
    if (tag) {
        if (_bgmValue < 3 )
            _bgmValue++;
    }
    else
    {
        if (_bgmValue > 0 )
            _bgmValue--;
    }
    
    xAudio->changeMusicVolume((float)_bgmValue/3);
    bgmAction();
}

void Switch::Effect(bool tag)
{
    if (tag) {
        if (_effValue < 3 )
            _effValue++;
    }
    else
    {
        if (_effValue > 0 )
            _effValue--;
    }
    
    xAudio->changeSoundVolume((float)_effValue/3);
    effectAction();
    
    xAudio->playDefaultSound();
}

void Switch::setBgmBackground(std::vector<ImageView*> image)
{
    b_image = image;
}

void Switch::setEffetBackground(std::vector<ImageView*> image)
{
    e_image = image;

}
void Switch::initData(){
    _effValue = xAudio->getSoundVolume() * 3;
    _bgmValue = xAudio->getMusicVolume() * 3;
    bgmAction();
    effectAction();
}


