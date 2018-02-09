//
//  SettingLayer.cpp
//  MexicanFood
//
//  Created by huxixiang on 16/3/29.
//
//

#include "SettingLayer.hpp"
#include "GameData.h"

enum setting_tag{
    eCloseBtnTag,
    eMusicBtnTag,
    eSoundBtnTag
};

SettingLayer::SettingLayer()
{
    
}

SettingLayer::~SettingLayer()
{
    
}

SettingLayer* SettingLayer::create(const Color4B& color)
{
    auto layer = new SettingLayer();
    if(layer && layer->initWithColor(color))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_RELEASE_NULL(layer);
    return nullptr;
}

bool SettingLayer::initWithColor(const Color4B& color)
{
    bool isInit = false;
    do{
        CC_BREAK_IF(!LayerColor::initWithColor(color));
        CC_BREAK_IF(!initData());
         CC_BREAK_IF(!initUI());
        isInit = true;
    }while(0);
    
    return isInit;
}

bool SettingLayer::initData()
{
    EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
    pListener->setSwallowTouches(true);
    pListener->onTouchBegan = [=](Touch* touch, Event* event){
        return true;
    };
    pListener->onTouchMoved = [=](Touch* touch, Event* event){};
    pListener->onTouchEnded = [=](Touch* touch, Event* event){};
    _eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

    return true;
}

bool SettingLayer::initUI()
{
    // add bg
    auto pBg = Sprite::create("images/ui/settingBg.png");
    kAdapterScreen->setExactPosition(pBg, 480, 330);
    this->addChild(pBg);
    
    // add close button
    auto pCloseBtn = ToolSprite::create("images/ui/close.png");
    pCloseBtn->setPosition(pBg->getContentSize().width - pCloseBtn->getContentSize().width / 2 - 6, pBg->getContentSize().height - pCloseBtn->getContentSize().height / 2 - 6);
    pCloseBtn->setDelegate(this);
    pCloseBtn->setTag(eCloseBtnTag);
    pCloseBtn->setShadeBtn(true);
    pBg->addChild(pCloseBtn);
    
    // add music button
    ToolSprite* pMusicBtn = nullptr;
    if (SoundPlayer::getInstance()->getMusicV() > 0) {
        pMusicBtn = ToolSprite::create("images/ui/on.png");
    }else{
        pMusicBtn = ToolSprite::create("images/ui/off.png");
    }
    pMusicBtn->setPosition(405, 158);
    pMusicBtn->setDelegate(this);
    pMusicBtn->setTag(eMusicBtnTag);
    pMusicBtn->setShadeBtn(true);
    pBg->addChild(pMusicBtn);
    
    // add sound button
    ToolSprite* pSoundBtn = nullptr;
    if (SoundPlayer::getInstance()->getSoundV() > 0) {
        pSoundBtn = ToolSprite::create("images/ui/on.png");
    }else{
        pSoundBtn = ToolSprite::create("images/ui/off.png");
    }
    pSoundBtn->setPosition(405, 76);
    pSoundBtn->setDelegate(this);
    pSoundBtn->setTag(eSoundBtnTag);
    pSoundBtn->setShadeBtn(true);
    pBg->addChild(pSoundBtn);
    
    return true;
}

void SettingLayer::onTouchUpInBoundingBox(ToolSprite* toolSprite,Touch *pTouch)
{
    switch (toolSprite->getTag()) {
        case eCloseBtnTag:
        {
            SoundPlayer::getInstance()->playEffect(buttonSound);
            this->removeFromParentAndCleanup(true);
        }
            break;
            
        case eMusicBtnTag:
        {
            SoundPlayer::getInstance()->playEffect(buttonSound);
            if (SoundPlayer::getInstance()->getMusicV() > 0) {
                SoundPlayer::getInstance()->setMusicV(0);
                SoundPlayer::getInstance()->flushVolume();
                toolSprite->setTexture("images/ui/off.png");
            }else{
                SoundPlayer::getInstance()->setMusicV(1.0f);
                SoundPlayer::getInstance()->flushVolume();
                toolSprite->setTexture("images/ui/on.png");
            }
        }
            break;
            
        case eSoundBtnTag:
        {
            if (SoundPlayer::getInstance()->getSoundV() > 0) {
                SoundPlayer::getInstance()->setSoundV(0);
                SoundPlayer::getInstance()->flushVolume();
                toolSprite->setTexture("images/ui/off.png");
            }else{
                SoundPlayer::getInstance()->setSoundV(1.0f);
                SoundPlayer::getInstance()->flushVolume();
                toolSprite->setTexture("images/ui/on.png");
            }
            SoundPlayer::getInstance()->playEffect(buttonSound);
        }
            break;
    }
}