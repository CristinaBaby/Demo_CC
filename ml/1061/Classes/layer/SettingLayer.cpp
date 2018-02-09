//
//  SettingLayer.cpp
//  FMAKER033
//
//  Created by wuguiling on 9/11/14.
//
//

#include "SettingLayer.h"
#include "../Depends/utils/Utils.h"
#include "../Depends/utils/AudioHelp.h"
#include "../Depends/managers/IAPManager.h"

#include "../Depends/utils/AdapterScreen.h"
#include "HomeLayer.h"
bool SettingLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    LayerColor* colorLayer = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(colorLayer);
  
    m_pBgSprite = Sprite::create("images/button/setting_bg.png");
    m_pBgSprite->ignoreAnchorPointForPosition(false);
    m_pBgSprite->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(m_pBgSprite);
    kAdapterScreen->setExactPosition(m_pBgSprite, 480, 640+m_pBgSprite->getContentSize().height);
    Sprite* closeSprite = Utils::getUISprite("images/button/btn_exit.png");
   
    ControlButton* btnClose = ControlButton::create(Scale9Sprite::create("images/button/btn_exit.png"));
    btnClose->setPreferredSize(closeSprite->getContentSize());
    btnClose->addTargetWithActionForControlEvents(this, cccontrol_selector(SettingLayer::_onCloseClickedCallBack), Control::EventType::TOUCH_UP_INSIDE);

    btnClose->setAnchorPoint(Vec2(0.5,0.5));
    btnClose->setPosition(Vec2(m_pBgSprite->getContentSize().width-30, m_pBgSprite->getContentSize().height-30));
    m_pBgSprite->addChild(btnClose);

   m_pSoundCheckBox = CheckBox::create("images/button/setting_off.png", "images/button/setting_off.png", "images/button/setting_on.png", "images/button/setting_off.png", "images/button/setting_on.png");
    m_pMusicCheckBox= CheckBox::create("images/button/setting_off.png", "images/button/setting_off.png", "images/button/setting_on.png", "images/button/setting_off.png", "images/button/setting_on.png");

    m_pSoundCheckBox->addEventListener(CC_CALLBACK_2(SettingLayer::soundCheckBoxCallback, this) );
    m_pMusicCheckBox->addEventListener(CC_CALLBACK_2(SettingLayer::musicCheckBoxCallback, this) );
    if(AudioHelp::getInstance()->getSoundVolume()>0)
        m_pMusicCheckBox->setSelectedState(true);
    else
        m_pMusicCheckBox->setSelectedState(false);
    
    if(AudioHelp::getInstance()->getEffectVolume()>0)
        m_pSoundCheckBox->setSelectedState(true);
    else
        m_pSoundCheckBox->setSelectedState(false);

    m_pSoundCheckBox->setPosition(Vec2(360,75));
    m_pBgSprite->addChild(m_pSoundCheckBox);

    m_pMusicCheckBox->setPosition(Vec2(360,155));
    m_pBgSprite->addChild(m_pMusicCheckBox);
    
      return true;
}

void SettingLayer::onEnter()
{
    CCLayer::onEnter();
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
//    this->setTouchEnabled(true);
    this->setZOrder(201);
}

void SettingLayer::onExit()
{
    CCLayer::onExit();
   
}
void SettingLayer::showInParent(HomeLayer *parentNode)
{
    m_Parent = parentNode;
    parentNode->addToUILayer(this, 200);
    _easeIn();
}

bool SettingLayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void SettingLayer::_onCloseClickedCallBack(Ref* ref, Control::EventType controlEvent)
{
    _easeOut();
     AudioHelp::getInstance()->playDeleteEffect();
}
void SettingLayer::_onRestoreClickedCallBack(Ref* ref, Control::EventType controlEvent)
{
    IAPManager::getInstance()->restore();
    
}

void SettingLayer::_onSettingCallback(Ref* ref, Control::EventType controlEvent)
{
    ControlSlider* slide = dynamic_cast<ControlSlider*>(ref);
    if (slide==m_pSoundSlider) {
        AudioHelp::getInstance()->setEffectVolume(slide->getValue());
    }else if (slide == m_pMusicSlider) {
        AudioHelp::getInstance()->setSoundVolume(slide->getValue());
    }
}

void SettingLayer::_easeIn()
{
//    AdvertiseAdapter::pauseBannerAd();
    if (kIAPManager->isShowAds()) {
      
        AdsManager::getInstance()->preloadAds(ADS_TYPE::kTypeBannerAds);
    }
    m_pBgSprite->runAction(MoveTo::create(0.5, kAdapterScreen->getExactPostion(Vec2(480, 320))));
}

void SettingLayer::_easeOut(){
    
    m_pBgSprite->runAction(CCSequence::create(CCMoveBy::create(0.3, kAdapterScreen->getExactPostion(Vec2(0, -480-m_pBgSprite->getContentSize().height))),CallFunc::create(CC_CALLBACK_0(SettingLayer::_easeOutDone, this)),NULL));
}

void SettingLayer::_easeOutDone()
{
    this->removeFromParent();
}
void SettingLayer::musicCheckBoxCallback(Ref* ref,CheckBox::EventType checkEvent)
{
    if(checkEvent==CheckBox::EventType::SELECTED)
    {
        AudioHelp::getInstance()->setSoundVolume(1.0f);
    }
    else
    {
        AudioHelp::getInstance()->setSoundVolume(0);
    }
}

void SettingLayer::soundCheckBoxCallback(Ref* ref,CheckBox::EventType checkEvent)
{
    if(checkEvent==CheckBox::EventType::SELECTED)
    {
        AudioHelp::getInstance()->setEffectVolume(1.0f);
    }
    else
    {
        AudioHelp::getInstance()->setEffectVolume(0);
    }
}

