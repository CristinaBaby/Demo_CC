//
//  HomeLayer.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-22.
//
//

#include "HomeLayer.h"
#include "SettingLayer.h"
#include "../Depends/modules/CFSystemFunction.h"

#include "../Depends/managers/IAPManager.h"
#include "../Depends/utils/AudioHelp.h"

#include "ActionUtils.h"
#include "Global.h"
#include "STAds.h"
#include "MoregameBtn.h"
#include "SelectScene.h"

HomeLayer::HomeLayer()
{
    
}
HomeLayer::~HomeLayer()
{

}
bool HomeLayer::init()
{
    bool isInit=false;
    do{
        
        CC_BREAK_IF(!BaseLayer::init());
        initUI();
        isInit=true;
        
    }while(0);
    
    return true;
    
}

void HomeLayer::initUI()
{
    
    
    
    m_pBgSprite=Sprite::create("images/home/bg.jpg");
    m_pBgSprite->setAnchorPoint(Vec2(0.5,0.5));
    kAdapterScreen->setExactPosition(m_pBgSprite, 320, 480);
    this->addToBackGroundLayer(m_pBgSprite, 1);
    
   
    
    m_pLogo = Sprite::create("images/home/logo.png");
    kAdapterScreen->setExactPosition(m_pLogo, 320, -1000,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addToContentLayer(m_pLogo,1);
    
    m_pCrab = Sprite::create("images/home/crab.png");
    m_pCrab->setPosition(Vec2(545,192));
    m_pBgSprite->addChild(m_pCrab,1);

    
    m_pUIMenu = CCMenu::create();
    m_pUIMenu->setPosition(Vec2::ZERO);
    m_pUIMenu->setZOrder(100);
   
    m_pShopItemSprite = Utils::getMenuItemSprite("images/home/home_btn_shop.png", "images/home/home_btn_shop.png", m_pUIMenu, this, menu_selector(HomeLayer::onBtnCallBack));
    m_pShopItemSprite->setAnchorPoint(Vec2(0,0));
    
    m_pFavItemSprite = Utils::getMenuItemSprite("images/button/btn_fav@2x.png", "images/button/btn_fav@2x.png", m_pUIMenu, this,menu_selector(HomeLayer::onBtnCallBack));
    m_pFavItemSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
   
    m_pSettingItemSprite = Utils::getMenuItemSprite("images/button/btn_setting@2x.png", "images/button/btn_setting@2x.png", m_pUIMenu, this, menu_selector(HomeLayer::onBtnCallBack));
    m_pSettingItemSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    m_pStartItemSprite = Utils::getMenuItemSprite("images/home/home_btn_play.png", "images/home/home_btn_play.png", m_pUIMenu, this, menu_selector(HomeLayer::onBtnCallBack));
    m_pStartItemSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    m_pPrivacyItemSprite = Utils::getMenuItemSprite("images/home/home_btn_rate.png", "images/home/home_btn_rate.png", m_pUIMenu, this, menu_selector(HomeLayer::onBtnCallBack));
    m_pPrivacyItemSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    
    m_pStartItemSprite  ->setTag(eTagStart);
    m_pShopItemSprite   ->setTag(eTagShop);
    m_pFavItemSprite    ->setTag(eTagFavorite);
    m_pSettingItemSprite->setTag(eTagSetting);
    m_pPrivacyItemSprite->setTag(eTagPrivacy);

    kAdapterScreen->setExactPosition(m_pStartItemSprite, 320,480 - 50 - 50);
    kAdapterScreen->setExactPosition(m_pShopItemSprite, 20 + 1000 , 20,Vec2(0,0),kBorderLeft,kBorderBottom);
    kAdapterScreen->setExactPosition(m_pFavItemSprite, 170 + 1000, 60,kAnchorDF,kBorderRight,kBorderTop);
    kAdapterScreen->setExactPosition(m_pSettingItemSprite, 280 + 1000, 60,kAnchorDF,kBorderRight,kBorderTop);
    kAdapterScreen->setExactPosition(m_pPrivacyItemSprite, 20, 20,kAnchorRT,kBorderRight,kBorderTop);
    
    m_pStartItemSprite->setScale(0);
    
    AudioHelp::getInstance()->playBackgroundMusic("startbg.mp3");
    
    this->addToUILayer(m_pUIMenu, 1);
    m_pUIMenu->setEnabled(false);
    
    MoregameBtn* pBtn = MoregameBtn::create();
    pBtn->addtoParentLayer(this);
    
    pBtn->startLoading();
  
    m_pBear = Sprite::create("images/home/home_bear.png");
    m_pBear->setPosition(Vec2(m_pShopItemSprite->getContentSize().width/2,0));
    m_pBear->setAnchorPoint(Vec2(0.5,0));
    m_pShopItemSprite->addChild(m_pBear);
    
    auto func = CallFunc::create([&](){
        
     
        ShowLogo();
        
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.5),func, NULL));
    
}
void HomeLayer::ShowLogo(){
    
    
    auto func = CallFunc::create([&](){
        
        
//        ShowLogo();
        ShowUIPlay();
        
        
        
    });
    
    ActionUtils::dropOut(m_pLogo, Vec2(0,-1000),1.0,func);

    auto pOvenAnimation = Animation::create();
    
    
        
    pOvenAnimation->addSpriteFrameWithFile("images/home/crab.png");
    pOvenAnimation->addSpriteFrameWithFile("images/home/crab1.png");

    
    pOvenAnimation->setDelayPerUnit(0.5f);
    pOvenAnimation->setLoops(-1);
    
    auto pOvenAnimat = Animate::create(pOvenAnimation);

    m_pCrab->runAction(Sequence::create(pOvenAnimat,NULL));


}
//显示开始按钮
void HomeLayer::ShowUIPlay()
{
    auto func = CallFunc::create([&](){
        
        m_pUIMenu->setEnabled(true);
    
        ScaleBy* scale = ScaleBy::create(0.8, 1.1);
        m_pStartItemSprite->runAction(RepeatForever::create(Sequence::create(scale,scale->reverse(), NULL)));
        
        RotateTo* rote = RotateTo::create(1.0, 20);
        RotateTo* rote1 = RotateTo::create(1.0, -20);
        RepeatForever* repeat = RepeatForever::create(Sequence::create(rote,rote1, NULL));
        
        m_pBear->runAction(repeat);
    });
    
    
    RotateBy* rote = RotateBy::create(1.5, 360*2);
    ScaleTo* scale = ScaleTo::create(1.5, 0.8);
    Spawn* spawn = Spawn::create(rote,scale, NULL);
    
    m_pStartItemSprite->runAction(Sequence::create(spawn,func, NULL));
    
    ActionUtils::moveBy(m_pShopItemSprite, Vec2(-1000,0));
    
    
    MoveBy* move = MoveBy::create(0.5, Vec2(0,20));
    m_pLogo->runAction(RepeatForever::create(Sequence::create(move,move->reverse(), NULL)));

    MoveBy* move1 = MoveBy::create(5.0, Vec2(-50,0));
    m_pCrab->runAction(RepeatForever::create(Sequence::create(move1,move1->reverse(), NULL)));


}
//现实UI
void HomeLayer::ShowUIMenu()
{
    
    auto func = CallFunc::create([&](){
        
        ShowUIPlay();
        
    });
    
    this->runAction(Sequence::create(DelayTime::create(2.0),func, NULL));
    

}

void HomeLayer::onEnter()
{
    BaseLayer::onEnter();
    
    STAds ads;
    ads.removeAds();
    
//    ShowIcon::show();
    
//    schedule(schedule_selector(HomeLayer::createQiqiu), 2.0);

    AudioHelp::getInstance()->StopBackgroundMusic();
    AudioHelp::getInstance()->playBackgroundMusic("startbg.mp3");
}

void HomeLayer::onEnterTransitionDidFinish()
{
    
    BaseLayer::onEnterTransitionDidFinish();
//    ShowIcon::show();
}

void HomeLayer::onExit()
{
    BaseLayer::onExit();
    //ShowIcon::hide();
}
//展示全屏广告的回调
void HomeLayer::showAdsLaod(Ref*)
{
//    ShowIcon::show();
    Director::getInstance()->stopAnimation();
}
//关闭全屏广告的回调
void HomeLayer::closeAdsLaod(Ref*)
{
    //ShowIcon::show();
    Director::getInstance()->startAnimation();
    
}
void HomeLayer::onBtnCallBack(Ref* sender)
{
    Node* node = dynamic_cast<Node*>(sender);
    switch (node->getTag()) {
        case eTagStart:
        {
            
            m_pUIMenu->setEnabled(false);
  
             auto scene = SceneChangeManager->enterSelectScene();
            //cross
            if (kIAPManager->isShowAds()) {
                
                AdLoadingLayerBase::showLoading<AdsLoadingLayer>(false,scene);
                
            }
            
             CCLOG("startGame");
            
            AudioHelp::getInstance()->playStartEffect();
            
        }
            break;
        case eTagFavorite:
        {
                CCLOG("Favorite");
//            ShowIcon::hide();
            m_pUIMenu->setEnabled(false);
            SceneChangeManager->enterFavScene();
            AudioHelp::getInstance()->playSelectEffect();
            auto func = CallFunc::create([&](){
                
                m_pUIMenu->setEnabled(true);
                
            });
            ActionUtils::delayTimeAction(this,0.5,func);
        }
            break;
        case eTagMoreGame:
        {
            CCLOG("MoreGame");
            AudioHelp::getInstance()->playSelectEffect();

        }
            break;
        case eTagSetting:
        {
               CCLOG("Setting");
            m_pUIMenu->setEnabled(false);
            AudioHelp::getInstance()->playSelectEffect();
            SettingLayer* layer = SettingLayer::create();
            layer->showInParent(this);
            
        }
            break;
        case eTagShop:
        {
            CCLOG("shop");
//            ShowIcon::hide();
            m_pUIMenu->setEnabled(false);
            AudioHelp::getInstance()->playSelectEffect();
            SceneChangeManager->enterShopScene();
            
            auto func = CallFunc::create([&](){
                
                m_pUIMenu->setEnabled(true);
                
            });
            ActionUtils::delayTimeAction(this,0.5,func);
        }
            break;
        case eTagPrivacy:
        {

            CCLOG("eTagPrivacy");
            CFSystemFunction sys;
            sys.rateUs();
          
        }
            break;
            
        default:
            break;
    }

}



// 监听Android返回键事件
void HomeLayer::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
{
    if(keyCode==EventKeyboard::KeyCode::KEY_BACK)
    {
        if(!isReturn)
        {
            isReturn = true;
            Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogExit, Dialog::DIALOG_TYPE_NEGATIVE);
            //                    dialog -> setAnchorPoint(Vec2(0,0));
            //                    STVisibleRect::setPosition(dialog, size.width/2, size.height/2);
            dialog -> setAnchorPoint(Vec2(0,0));
            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
            dialog->setContentText("Are you sure you want to start over with a new Italian Food?");
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            dialog->setCallback(this);
            this->addToUILayer(dialog, 9999);
        }
    }
}

void HomeLayer::onNegativeClick(void* type)
{
    isReturn = false;
}
void HomeLayer::onPositiveClick(void* type)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    CFSystemFunction sys;
    sys.endSession();
    Director::getInstance()->end();
#endif
    
}
