//
//  HomeLayer.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-22.
//
//

#include "HomeLayer.h"
#include "SettingLayer.h"

#include "../Depends/managers/IAPManager.h"
#include "../Depends/utils/AudioHelp.h"

#include "ActionUtils.h"
#include "Global.h"

#include "SSCMoreGameButton.h"
#include "SelectScene.h"
#include "STSystemFunction.h"
#include "Analytics.h"

HomeLayer::HomeLayer()
{
    m_bFirst = true;
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
    m_pBgSprite=Sprite::create("images/home/start_bg0.jpg");
    m_pBgSprite->setAnchorPoint(Vec2(0.5,0.5));
    kAdapterScreen->setExactPosition(m_pBgSprite, 480, 320);
    this->addToBackGroundLayer(m_pBgSprite, 1);
    
    m_pBgSpriteFront =Sprite::create("images/home/start_bg1.png");
    m_pBgSpriteFront->setAnchorPoint(Vec2(0.5,0.5));
    kAdapterScreen->setExactPosition(m_pBgSpriteFront, 480, 320);
    this->addToBackGroundLayer(m_pBgSpriteFront, 2);
    
    
    
    
    m_pLogo = ToolSprite::create("images/home/logo.png");
    
    m_pLogo->setDelegate(this);
    m_pLogo->setIsMove(false);
    kAdapterScreen->setExactPosition(m_pLogo, m_pLogo->getContentSize().width/2+50,m_pLogo->getContentSize().height/2+ 50 - 1000,Vec2(0.5,0.5),kBorderRight,kBorderTop);
    this->addToContentLayer(m_pLogo,5);
    
    
    m_pCar = ToolSprite::create("images/home/start_car.png");
    m_pCar->setPosition(Vec2(952,369 + 70));
    m_pCar->setAnchorPoint(Vec2(0.5,0.5));
    m_pCar->setScale(0.3);
    m_pCar->setDelegate(this);
    m_pCar->setIsMove(false);
    m_pBgSprite->addChild(m_pCar);
    m_pCar->setFlippedX(true);
    
    auto wheel = Sprite::create("images/home/start_tyre0.png");
    wheel->setPosition(Vec2(226 - 125,90));
    wheel->setScale(0.9);
    m_pCar->addChild(wheel);
    
    auto wheel1 = Sprite::create("images/home/start_tyre0.png");
    wheel1->setPosition(Vec2(579 - 130,93));
    wheel1->setScale(0.9);
    m_pCar->addChild(wheel1);
    
    
    wheel->runAction(RepeatForever::create(RotateBy::create(0.25, -360)));
    wheel1->runAction(RepeatForever::create(RotateBy::create(0.25, -360)));
    
    
    m_pUIMenu = CCMenu::create();
    m_pUIMenu->setPosition(Vec2::ZERO);
    m_pUIMenu->setZOrder(100);
   
    m_pShopItemSprite = Utils::getMenuItemSprite("images/button/btn_shop@2x.png", "images/button/btn_shop@2x.png", m_pUIMenu, this, menu_selector(HomeLayer::onBtnCallBack));
    m_pShopItemSprite->setAnchorPoint(Vec2(0,0));
    
    m_pFavItemSprite = Utils::getMenuItemSprite("images/button/btn_fav@2x.png", "images/button/btn_fav@2x.png", m_pUIMenu, this,menu_selector(HomeLayer::onBtnCallBack));
    m_pFavItemSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
   
    m_pStartItemSprite = Utils::getMenuItemSprite("images/button/btn_start@2x.png", "images/button/btn_start@2x.png", m_pUIMenu, this, menu_selector(HomeLayer::onBtnCallBack));
    m_pStartItemSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    m_pRateusItemSprite = Utils::getMenuItemSprite("images/home/home_btn_rate.png", "images/home/home_btn_rate.png", m_pUIMenu, this, menu_selector(HomeLayer::onBtnCallBack));
    m_pRateusItemSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    m_pPrivacyItemSprite = Utils::getMenuItemSprite("images/home/btn_pp.png", "images/home/btn_pp.png", m_pUIMenu, this, menu_selector(HomeLayer::onBtnCallBack));
    m_pPrivacyItemSprite->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    
//    m_pMiniGameItemSprite = Utils::getMenuItemSprite("images/home/mini_game.png", "images/home/mini_game.png", m_pUIMenu, this, menu_selector(HomeLayer::onBtnCallBack));
    m_pSettingItemSprite = Utils::getMenuItemSprite("images/button/btn_setting@2x.png", "images/button/btn_setting@2x.png", m_pUIMenu, this, menu_selector(HomeLayer::onBtnCallBack));
    m_pSettingItemSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

//    m_pMiniGameItemSprite->setTag(eMiniGame);
    m_pStartItemSprite  ->setTag(eTagStart);
    m_pShopItemSprite   ->setTag(eTagShop);
    m_pFavItemSprite    ->setTag(eTagFavorite);
    m_pRateusItemSprite->setTag(eTagRateus);
    m_pSettingItemSprite->setTag(eTagSetting);
    m_pPrivacyItemSprite->setTag(eTagPrivacy);

    kAdapterScreen->setExactPosition(m_pStartItemSprite, 480,320 - 50 - 50);
    kAdapterScreen->setExactPosition(m_pShopItemSprite, 10+ m_pShopItemSprite->getContentSize().width*2 + 20 , 10 - 200,Vec2(0,1),kBorderLeft,kBorderTop);
    kAdapterScreen->setExactPosition(m_pFavItemSprite, 10 + m_pShopItemSprite->getContentSize().width + 10, 10 - 200,Vec2(0,1),kBorderLeft,kBorderTop);
    kAdapterScreen->setExactPosition(m_pRateusItemSprite, 20, 20,kAnchorLT,kBorderLeft,kBorderTop);
    kAdapterScreen->setExactPosition(m_pSettingItemSprite, 10, 10 - 200,Vec2(0,1),kBorderLeft,kBorderTop);
    kAdapterScreen->setExactPosition(m_pPrivacyItemSprite, 10, 10 - 200,Vec2(1,1),kBorderRight,kBorderTop);
    
    
    m_pStartItemSprite->setScale(0);
    
    AudioHelp::getInstance()->playBackgroundMusic("startbg.mp3");
    
    this->addToUILayer(m_pUIMenu, 1);
    m_pUIMenu->setEnabled(false);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    m_pShopItemSprite->setVisible(false);
#endif
    m_pRateusItemSprite->setVisible(false);
    
    SSCMoreGameButton* m_moreGameButton = SSCMoreGameButton::create();
    //    //当首页有banner广告时调用该方法，第二个参数传true，来显示button并设置位置
    m_moreGameButton->showButton(m_pContentLayer,true);
    m_moreGameButton->setClickCall([=](SSCMoreGameButton*){
        SSCMoreGameManager::getInstance()->show(MGAT_EXPAND);
    });
    kAdapterScreen->setExactPosition(m_moreGameButton, 10, 100,Vec2(1,0),kBorderRight,kBorderBottom);
}

void HomeLayer::ShowLogo(){
    
    
    auto func = CallFunc::create([=](){
        ShowUIPlay();
    });
    AudioHelp::getInstance()->playEffect("swish.mp3");
    ActionUtils::dropOut(m_pLogo, Vec2(0,-1000),1.0,func);
}
//显示开始按钮
void HomeLayer::ShowUIPlay()
{
    auto func = CallFunc::create([=](){
        
        m_pUIMenu->setEnabled(true);
    
        
        ScaleBy* scale = ScaleBy::create(0.8, 1.1);
        m_pStartItemSprite->runAction(RepeatForever::create(Sequence::create(scale,scale->reverse(), NULL)));
        
      
        
        
       
    });
    
    AudioHelp::getInstance()->playEffect("swish.mp3");
    RotateBy* rote = RotateBy::create(1.5, 360*2);
    ScaleTo* scale = ScaleTo::create(1.5, 1.0);
    Spawn* spawn = Spawn::create(rote,scale, NULL);
    
    m_pStartItemSprite->runAction(Sequence::create(spawn,func, NULL));
    
    m_pShopItemSprite->runAction(MoveBy::create(1.5, Vec2(0,-200)));
    m_pFavItemSprite->runAction(MoveBy::create(1.5, Vec2(0,-200)));

    m_pSettingItemSprite->runAction(MoveBy::create(1.5, Vec2(0,-200)));
    
    m_pPrivacyItemSprite->runAction(MoveBy::create(1.5, Vec2(0,-200)));
    
    
    auto chooseParticle = ParticleSystemQuad::create("particle/bakeToolParticle.plist");
    chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
    chooseParticle->setVisible(true);
    chooseParticle->setPosition(m_pLogo->getContentSize()*.5);
    chooseParticle->setScale(1.5);
    m_pLogo->addChild(chooseParticle,4);
    
    MoveBy* move = MoveBy::create(1.0, Vec2(0,20));
    m_pLogo->runAction(RepeatForever::create(Sequence::create(move,move->reverse(), NULL)));

 
}

void HomeLayer::onEnter()
{
    BaseLayer::onEnter();
    
    AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
    
    AudioHelp::getInstance()->StopBackgroundMusic();
    AudioHelp::getInstance()->playBackgroundMusic("startbg.mp3");

    
    
    m_pUIMenu->setEnabled(true);
    
    if (m_bFirst) {
        
        m_bFirst = false;
        this->scheduleOnce(schedule_selector(HomeLayer::showCar),0.5);
        
    }
}
void HomeLayer::showCar(float dt)
{
    auto func = CallFunc::create([=](){
        
        this->carAction();
        
    });
    
    MoveBy* move = MoveBy::create(2.5, Vec2(-1200,-160));
    RotateBy* rota = RotateBy::create(2.5, -20);
    Spawn* sp = Spawn::create(move,rota,nullptr);
    
    
    m_pCar->runAction(Sequence::create(sp,func, NULL));
}
void HomeLayer::carAction()
{
    m_pCar->removeFromParent();
    m_pCar = ToolSprite::create("images/home/start_car.png");
    m_pCar->setPosition(Vec2(430 - 1000 + m_pCar->getContentSize().width/2,123 + m_pCar->getContentSize().height/2));
    m_pCar->setAnchorPoint(Vec2(1,0.5));
    m_pBgSpriteFront->addChild(m_pCar);
    m_pCar->setDelegate(this);
    m_pCar->cancelEvent(true);
    m_pCar->setIsMove(false);
    
    auto wheel = Sprite::create("images/home/start_tyre0.png");
    wheel->setPosition(Vec2(230,86));
    wheel->setScale(0.9);
    wheel->setTag(10);
    m_pCar->addChild(wheel);
    
    auto wheel1 = Sprite::create("images/home/start_tyre0.png");
    wheel1->setPosition(Vec2(579,80));
    wheel1->setScale(0.9);
    wheel1->setTag(20);
    m_pCar->addChild(wheel1);
    
    wheel->runAction(RepeatForever::create(RotateBy::create(0.5, 120)));
    wheel1->runAction(RepeatForever::create(RotateBy::create(0.5, 120)));
    
    
    auto func = CallFunc::create([=](){
    
        logoAction();
        
    });
    AudioHelp::getInstance()->playEffect("swish.mp3");
    
    MoveBy* move = MoveBy::create(1.0, Vec2(1000,0));
    EaseSineIn* esi = EaseSineIn::create(move);
    m_pCar->runAction(Sequence::create(esi,func, NULL));
}
void HomeLayer::logoAction()
{
    auto func = CallFunc::create([=](){
        
     //showUI
        ShowUIPlay();
    });
    
    m_pCar->getChildByTag(10)->stopAllActions();
    m_pCar->getChildByTag(20)->stopAllActions();
    
    AudioHelp::getInstance()->playEffect("swish.mp3");
    ActionUtils::dropOut(m_pLogo, Vec2(0,-1000),1.0,func);
    
    
    m_pCar->runAction(Sequence::create(ScaleTo::create(0.2, 0.9,1.0), ScaleTo::create(0.2, 1.0,1.0),NULL));
    
}
void HomeLayer::onEnterTransitionDidFinish()
{
    
    BaseLayer::onEnterTransitionDidFinish();
    
}

void HomeLayer::onExit()
{
    BaseLayer::onExit();

}
void HomeLayer::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    toolSprite->cancelEvent(true);
    auto func = CallFunc::create([&](){
        
        m_pLogo->cancelEvent(false);
    });
    
    
    ScaleTo* s1 = ScaleTo::create(0.2f,1.08f,0.90f);
    ScaleTo* s2 = ScaleTo::create(0.2f,0.90,1.08f);
    ScaleTo* s3 = ScaleTo::create(0.2f,1.0f,1.0f);
    toolSprite->runAction(Sequence::create(s1,s2,s3,func,nullptr));

}
void HomeLayer::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{

}
void HomeLayer::showAdsLaod(EventCustom*)
{
    Director::getInstance()->stopAnimation();
}
//关闭全屏广告的回调
void HomeLayer::closeAdsLaod(EventCustom*)
{
    Director::getInstance()->startAnimation();
}
void HomeLayer::nextAction()
{
    ScaleTo* scale = ScaleTo::create(0.5, 1.1);
    RotateBy* rote = RotateBy::create(1.0, 360*2);
    ScaleTo* scale1 = ScaleTo::create(1.0, 0);
    Spawn* spawn = Spawn::create(rote,scale1, NULL);
    
    EaseSineOut* eso = EaseSineOut::create(spawn);
    
    m_pStartItemSprite->runAction(Sequence::create(scale,eso,RemoveSelf::create(),NULL));
    
    auto func = CallFunc::create([=](){
    
        carGoOut();
        
    });
    
    m_pLogo->runAction(Sequence::create(DelayTime::create(1.5),ScaleTo::create(0.5, 0),func,NULL));
    
    m_pShopItemSprite   ->runAction(Sequence::create(DelayTime::create(1.5),MoveBy::create(0.5,Vec2(0,500)),func,NULL));
    m_pFavItemSprite    ->runAction(Sequence::create(DelayTime::create(1.5),MoveBy::create(0.5,Vec2(0,500)),NULL));
    m_pSettingItemSprite    ->runAction(Sequence::create(DelayTime::create(1.5),MoveBy::create(0.5,Vec2(0,500)),NULL));
    
    
    
}
void HomeLayer::carGoOut()
{
    m_pCar->getChildByTag(10)->runAction(RepeatForever::create(RotateBy::create(0.5, 240)));
    m_pCar->getChildByTag(20)->runAction(RepeatForever::create(RotateBy::create(0.5, 240)));
    
    auto func = CallFunc::create([=](){
        
        //
        if(kIAPManager->isShowAds())
        {
            AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
            
        }
        SceneChangeManager->enterSelectScene();

        
    });
    
    MoveBy* move = MoveBy::create(1.0, Vec2(500,0));
    EaseSineIn* esi = EaseSineIn::create(move);
    m_pCar->runAction(Sequence::create(esi,func, NULL));
}
void HomeLayer::onBtnCallBack(Ref* sender)
{
    Node* node = dynamic_cast<Node*>(sender);
    switch (node->getTag()) {
        case eTagStart:
        {
            
            m_pUIMenu->setEnabled(false);

//            if(kIAPManager->isShowAds())
//            {
//                AdLoadingLayerBase::showLoading<AdsLoadingLayer>(false);
//                AdLoadingLayerBase::loadingDoneCallback = []()
//                {
//                    SceneChangeManager->enterSelectScene();
//                };
//            }
//            else
//            {
//                SceneChangeManager->enterSelectScene();
//            }
             CCLOG("startGame");
            nextAction();
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
        case eTagRateus:
        {

            CCLOG("eTagRateus");
            SSCInternalLibManager::getInstance()->rateUs();
          
        }
            break;
        case eTagPrivacy:
        {
            STSystemFunction sys;
            sys.openUrl("https://www.makerlabs.net/privacy/");
        }
            break;
        case eMiniGame:
        {
           
            m_pUIMenu->setEnabled(false);
           
            
            
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
        if(!m_bIsShowReturn)
        {
            m_bIsShowReturn = true;
            Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogExit, Dialog::DIALOG_TYPE_NEGATIVE);
            //                    dialog -> setAnchorPoint(Vec2(0,0));
            //                    STVisibleRect::setPosition(dialog, size.width/2, size.height/2);
            dialog -> setAnchorPoint(Vec2(0,0));
            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
            dialog->setContentText(ExitGame);
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            dialog->setCallback(this);
            this->addChild(dialog, 9999);
        }
    }
}

void HomeLayer::onNegativeClick(void* type)
{
    m_bIsShowReturn = false;
}
void HomeLayer::onPositiveClick(void* type)
{
    Analytics::getInstance()->endSession();
    Director::getInstance()->end();

}
