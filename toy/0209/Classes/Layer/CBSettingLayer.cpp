//
//  CBSettingLayer.cpp
//  ColorBook
//
//  Created by maxiang on 4/21/15.
//
//

#include "CBSettingLayer.h"
#include "../Model/CBUtils.h"
#include "../Widget/CBSwitch.h"
#include "../Model/CBAppManager.h"
#include "UICpp.h"
#include "UITools.h"
#include "AudioHelper.h"

USING_NS_CC;

#define RES_BACK_BUTTON      "back.png"

SettingLayer::~SettingLayer(){
    xAds->removeAds(kTypeBannerAds);
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}

bool SettingLayer::init()
{
    if (!Layer::init()) {
        return false;
    }

    bool rcode = false;
    do {
        setName("settingLayer");
        
        auto _root = (cocos2d::ui::Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("setting.json"));
        addChild(_root);
        //back
        quickButton("back", _root, CC_CALLBACK_2(SettingLayer::backAction, this));
        
        auto popup = Helper::seekWidgetByName(_root, "Image_5");
        //get postion
        Vec2 bgmPos = (Helper::seekWidgetByName(_root, "close"))->getPosition();
        Vec2 sfxPos = (Helper::seekWidgetByName(_root, "close-1"))->getPosition();
        
        //bgm on/off
        Switch *bgmSwitch = Switch::create();
        bgmSwitch->setPosition(bgmPos);
        if (xAudio->getMusicVolume() > FLT_EPSILON)
        {
            bgmSwitch->on(false);
        }
        else
        {
            bgmSwitch->off(false);
        }
        bgmSwitch->setSwitchAction(CC_CALLBACK_1(SettingLayer::bgmOn, this));
        popup->addChild(bgmSwitch);
        
        //sfx on/off
        Switch *sfxSwitch = Switch::create();
        sfxSwitch->setPosition(sfxPos);
        if (xAudio->getSoundVolume() > FLT_EPSILON)
        {
            sfxSwitch->on(false);
        }
        else
        {
            sfxSwitch->off(false);
        }
        sfxSwitch->setSwitchAction(CC_CALLBACK_1(SettingLayer::sfxOn, this));
        popup->addChild(sfxSwitch);
        
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [](Touch* touch, Event* event) {
            return true;
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        xAds->showAds(kTypeBannerAds);
        
        rcode = true;
    } while (0);
 
    return rcode;
}

void SettingLayer::bgmOn(bool isOn)
{
    xAudio->switchMusic(isOn);
    xAudio->playMusic("bg/bg_1.mp3");
}

void SettingLayer::sfxOn(bool isOn)
{
    xAudio->switchSound(isOn);
    xAudio->playDefaultSound();
}

void SettingLayer::backAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if(cocos2d::ui::Widget::TouchEventType::ENDED != type) return;
    
    xAudio->playDefaultSound();

    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    removeFromParentAndCleanup(true);
}

