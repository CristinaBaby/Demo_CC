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

USING_NS_CC;
CB_BEGIN_NAMESPACE

#define RES_SETTING_BG       "ui02_settings_box.png"
#define RES_SETTING_BGM      "ui02_settings_bgm.png"
#define RES_SETTING_SFX      "ui02_settings_sfx.png"

#define RES_BACK_BUTTON      "ui05_popup_btn_back.png"

SettingLayer::~SettingLayer(){
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}
bool SettingLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 200))) {
        return false;
    }

    bool rcode = false;
    do {
        log("come on ");
        //back
        auto back = cocos2d::ui::Button::create(RES_BACK_BUTTON);
        back->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
        back->setPosition(Vec2(back->getContentSize().width/4, Utils::SCREEN_HEIGHT - back->getContentSize().width/4));
        back->addTouchEventListener(CC_CALLBACK_2(SettingLayer::backAction, this));
        addChild(back);
        
        //background
        Sprite *background = Sprite::create(RES_SETTING_BG);
        background->setPosition(Utils::getInstance()->getScreenCenter());
        addChild(background);
        
        //bgm icon
        Sprite *bgmIcon = Sprite::create(RES_SETTING_BGM);
        bgmIcon->setPosition(Vec2(background->getPosition().x - bgmIcon->getContentSize().width - 20, background->getPosition().y + bgmIcon->getContentSize().height/2 + 20));
        addChild(bgmIcon);
        
        //bgm on/off
        Switch *bgmSwitch = Switch::create();
        bgmSwitch->setPosition(Vec2(background->getPositionX() + bgmSwitch->getContentSize().width - 40, bgmIcon->getPositionY()));
        if (AppManager::getInstance()->isOpenBgm())
        {
            bgmSwitch->on(false);
        }
        else
        {
            bgmSwitch->off(false);
        }
        bgmSwitch->setSwitchAction(CC_CALLBACK_1(SettingLayer::bgmOn, this));
        addChild(bgmSwitch);
        
        //sfx icon
        Sprite *sfxIcon = Sprite::create(RES_SETTING_SFX);
        sfxIcon->setPosition(Vec2(background->getPosition().x - sfxIcon->getContentSize().width - 20, background->getPosition().y - sfxIcon->getContentSize().height*3/2 + 40));
        addChild(sfxIcon);
        
        //sfx on/off
        Switch *sfxSwitch = Switch::create();
        sfxSwitch->setPosition(Vec2(background->getPositionX() + sfxSwitch->getContentSize().width - 40, sfxIcon->getPositionY()));
        if (AppManager::getInstance()->isOpenEffect())
        {
            sfxSwitch->on(false);
        }
        else
        {
            sfxSwitch->off(false);
        }
        sfxSwitch->setSwitchAction(CC_CALLBACK_1(SettingLayer::sfxOn, this));
        addChild(sfxSwitch);
        
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [back](Touch* touch, Event* event) {
            return true;
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        AppManager::getInstance()->showBannerAd();
        
        rcode = true;
    } while (0);
 
    return rcode;
}

void SettingLayer::bgmOn(bool isOn)
{
    AppManager::getInstance()->setOpenBgm(isOn);
    AppManager::getInstance()->playButtonEffect();
    
    UserDefault::getInstance()->setBoolForKey(UserDefaultKey_isOpenBgm, isOn);
    UserDefault::getInstance()->flush();
}

void SettingLayer::sfxOn(bool isOn)
{
    AppManager::getInstance()->setOpenEffect(isOn);
    AppManager::getInstance()->playButtonEffect();

    UserDefault::getInstance()->setBoolForKey(UserDefaultKey_isOpenSfx, isOn);
    UserDefault::getInstance()->flush();
}

void SettingLayer::backAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if(cocos2d::ui::Widget::TouchEventType::ENDED != type) return;
    AppManager::getInstance()->playButtonEffect();

    AppManager::getInstance()->hideBannerAd();

    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    removeFromParentAndCleanup(true);
}

CB_END_NAMESPACE