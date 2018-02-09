//
//  CBSettingLayer.cpp
//  ColorBook
//
//  Created by maxiang on 4/21/15.
//
//

#include "CBSettingLayer.h"
#include "../Model/CBUtils.h"
#include "../Widget/Switch.hpp"
#include "../Model/CBAppManager.h"
#include "UICpp.h"
#include "UITools.h"
#include "AudioHelper.h"

USING_NS_CC;

#define RES_BACK_BUTTON      "back.png"

SettingLayer::~SettingLayer(){
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
        
        quickLayout("m_jia", _root, CC_CALLBACK_2(SettingLayer::bgmOn, this));
        quickLayout("m_jian", _root, CC_CALLBACK_2(SettingLayer::bgmOn, this));
        quickLayout("b_jia", _root, CC_CALLBACK_2(SettingLayer::bgmOn, this));
        quickLayout("b_jian", _root, CC_CALLBACK_2(SettingLayer::bgmOn, this));
        
        m_image.push_back(quickImageView("m_1", _root));
        m_image.push_back(quickImageView("m_2", _root));
        m_image.push_back(quickImageView("m_3", _root));
        b_image.push_back(quickImageView("b_1", _root));
        b_image.push_back(quickImageView("b_2", _root));
        b_image.push_back(quickImageView("b_3", _root));
        
        swith = Switch::create();
        addChild(swith);
        swith->setBgmBackground(m_image);
        swith->setEffetBackground(b_image);
        swith->initData();
        
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [](Touch* touch, Event* event) {
            return true;
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        xAds->setVisiable(kTypeBannerAds, true);
        rcode = true;
    } while (0);
 
    return rcode;
}

void SettingLayer::bgmOn(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type) {
    if(Widget::TouchEventType::ENDED != type) return;
    
    string name = ((Widget*)pSender)->getName();
    if (name.compare("m_jia") == 0) {
        swith->Bgm(true);
    }
    else if(name.compare("m_jian") == 0){
        swith->Bgm(false);
    }
    else if (name.compare("b_jia") == 0){
        swith->Effect(true);
    }
    else if(name.compare("b_jian") == 0){
        swith->Effect(false);
    }
}

void SettingLayer::backAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if(cocos2d::ui::Widget::TouchEventType::ENDED != type) return;
    
    xAudio->playDefaultSound();
    xAudio->playMusic("bg/bg_01.mp3");

    xAds->setVisiable(kTypeBannerAds, false);

    removeFromParentAndCleanup(true);
}

