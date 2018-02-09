//
//  CBSwitch.cpp
//  ColorBook
//
//  Created by maxiang on 4/22/15.
//
//

#include "CBSwitch.h"
USING_NS_CC;
CB_BEGIN_NAMESPACE

bool Switch::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    bool rcode = false;
    
    do {
        setOffBackground("ui02_settings_off.png");
        setOnBackground("ui02_settings_on.png");
        setSlider("ui02_settings_button.png");
  
        //default on
        on(false);
        
        //set content size to background
        setAnchorPoint(Point::ZERO);
        ignoreAnchorPointForPosition(false);
        setContentSize(_onBackground->getContentSize());
        
        //touch
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(false);
        touchEventListener->onTouchBegan = [this](Touch* touch, Event* event) {
      
            return true;
        };
        
        touchEventListener->onTouchEnded = [this](Touch* touch, Event* event) {
            
            Point point = this->convertToNodeSpace(touch->getLocation());
            if (_onBackground->getBoundingBox().containsPoint(point))
            {
                if (_isOn)
                {
                    off(true);
                }
                else
                {
                    on(true);
                }
            }
        };

        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        rcode = true;
    } while (0);
    
    return rcode;
}

void Switch::setSwitchAction(const SwichAction& action)
{
    _switchAction = action;
}

void Switch::callback()
{
    if (_isOn)
    {
        _offBackground->setVisible(false);
        _onBackground->setVisible(true);
    }
    else
    {
        _offBackground->setVisible(true);
        _onBackground->setVisible(false);
    }
    
    if (_switchAction != nullptr) {
        _switchAction(_isOn);
    }
}

void Switch::on(bool animation)
{
    CCASSERT(_offBackground, "Switch off background can not be null");
    CCASSERT(_onBackground, "Switch on background can not be null");
    CCASSERT(_slider, "Switch slider background can not be null");

    _isOn = true;
    Vec2 sliderPosition = Vec2(_onBackground->getPosition().x + _onBackground->getContentSize().width/3, _onBackground->getPosition().y);
    if (animation)
    {
        auto action = MoveTo::create(0.2, sliderPosition);
        _slider->runAction(Sequence::create(EaseInOut::create(action, 2), CallFunc::create(CC_CALLBACK_0(Switch::callback, this)), NULL));
    }
    else
    {
        _slider->setPosition(sliderPosition);
        callback();
    }
}

void Switch::off(bool animation)
{
    CCASSERT(_offBackground, "Switch off background can not be null");
    CCASSERT(_onBackground, "Switch on background can not be null");
    CCASSERT(_slider, "Switch slider background can not be null");
    
    _isOn = false;
    Vec2 sliderPosition = Vec2(_onBackground->getPosition().x - _onBackground->getContentSize().width/3, _onBackground->getPosition().y);
    if (animation)
    {
        auto action = MoveTo::create(0.2, sliderPosition);
        _slider->runAction(Sequence::create(EaseInOut::create(action, 2), CallFunc::create(CC_CALLBACK_0(Switch::callback, this)), NULL));
    }
    else
    {
        _slider->setPosition(sliderPosition);
        callback();
    }
}

void Switch::setOffBackground(const std::string& name)
{
    if (_offBackground != nullptr)
    {
        _offBackground->removeFromParent();
        _offBackground = nullptr;
    }
    
    _offBackground = Sprite::create(name);
    if (!_offBackground) {
        log("WARNING: Switch off background create failed!");
        return;
    }
    
    addChild(_offBackground);
}

void Switch::setOnBackground(const std::string& name)
{
    if (_onBackground != nullptr)
    {
        _onBackground->removeFromParent();
        _onBackground = nullptr;
    }
    
    _onBackground = Sprite::create(name);
    if (!_onBackground) {
        log("WARNING: Switch on background create failed!");
        return;
    }
    
    addChild(_onBackground);
}

void Switch::setSlider(const std::string& name)
{
    if (_slider != nullptr)
    {
        _slider->removeFromParent();
        _slider = nullptr;
    }
    
    _slider = Sprite::create(name);
    if (!_slider) {
        log("WARNING: Switch slider background create failed!");
        return;
    }
    
    addChild(_slider);
}


CB_END_NAMESPACE