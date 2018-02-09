//
// HomeScene.cpp
// ANIM3203
//
// Created by dengcheng on 15/9/10.
//
//

#include "HomeScene.h"
#include "AnimalChooseScene.h"
#include "STSystemFunction.h"
#include "ShopLayer.h"
#include "LoadingLayer.h"
#include "IAPManager.h"

HomeScene::HomeScene()
:m_bCross(false)
{
    
}

void HomeScene::onEnter()
{
    BaseScene::onEnter();
    if (m_bCross && !IAPManager::getInstance()->isAdRemoved()) {
        m_bCross = false;
        LoadingLayer::showLoading<LoadingLayer>(true, this, 99999);
    }
}

HomeScene* HomeScene::create()
{
    auto pRet = new HomeScene();
    if(pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    pRet = nullptr;
    return nullptr;
}

bool HomeScene::init()
{
    if ( !BaseScene::init("bg/home_bg.jpg") )
    {
        return false;
    }
    
    setName("HomeScene");
    
    auto *pUI = GUIReader::getInstance()->widgetFromJsonFile("cocoStudio/home.json");
    m_pUILayer->addChild(pUI);
    
    Button *pPlay = dynamic_cast<Button *>(Helper::seekWidgetByName(pUI, "ui01_home_btn_play"));
    pPlay->setTag(kNextScene);
    pPlay->addTouchEventListener(CC_CALLBACK_2(HomeScene::onButtonCallback, this));
    
    Button *pShop = dynamic_cast<Button *>(Helper::seekWidgetByName(pUI, "ui01_home_btn_shop"));
    pShop->setTag(kOtherTypeOne);
    pShop->addTouchEventListener(CC_CALLBACK_2(HomeScene::onButtonCallback, this));
    
    Button *pMoreGame = dynamic_cast<Button *>(Helper::seekWidgetByName(pUI, "ui01_home_btn_more"));
    pMoreGame->setTag(kOtherTypeTwo);
    pMoreGame->addTouchEventListener(CC_CALLBACK_2(HomeScene::onButtonCallback, this));
    
    m_pSound = SwitchButton::create("cocoStudio/ui01_home/ui01_home_music_on.png", "cocoStudio/ui01_home/ui01_home_music_off.png");
    m_pSound->setPosition(XCVisibleRect::getPosition(m_pSound->getButtonSize().width * 0.5 + 35, m_pSound->getButtonSize().height * 0.5 + 35, kLeft, kBottom));
    m_pUILayer->addChild(m_pSound);
    
    if (!UserDataManager::isOpenSound()) {
        m_pSound->changeStatus(SwitchButton::ButtonStatus::STATE_SELECTED);
    }
    
    m_pSound->setStatusChangeCallback([](SwitchButton::ButtonStatus aStatus){
        if (aStatus == SwitchButton::ButtonStatus::STATE_SELECTED) {
            UserDataManager::closeMusic();
            UserDataManager::closeSound();
        } else {
            UserDataManager::openMusic();
            UserDataManager::openSound("sound/menu.mp3");
        }
    });
    
    if (Director::getInstance()->getRunningScene() && Director::getInstance()->getRunningScene()->getName() == "ShowScene") {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/menu.mp3", true);
    }
    
    return true;
}

void HomeScene::onButtonCallback(Ref *aButton, Widget::TouchEventType aType)
{
    if (aType != Widget::TouchEventType::ENDED || m_bBackKeyEvent) {
        return;
    }
    
    switch (dynamic_cast<Button *>(aButton)->getTag()) {
        case kNextScene:
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/button_play.mp3");
            m_bBackKeyEvent = true;
            AnimalChooseScene *pScene = AnimalChooseScene::create();
            pScene->showAds();
            Director::getInstance()->replaceScene(pScene);
        }
            break;
        case kOtherTypeOne://shop
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/popup.mp3");
            m_pUILayer->addChild(ShopLayer::create(), kPopZorder);
        }
            break;
        case kOtherTypeTwo://more game
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/button.mp3");
            STSystemFunction func;
            func.showMoreGame();
        }
            break;
        default:
            break;
    }
}

void HomeScene::onExit()
{

    BaseScene::onExit();
}
HomeScene::~HomeScene()
{
    
}