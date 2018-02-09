//
//  HomeScene.cpp
//  BHUG1062
//
//  Created by liji on 16/5/3.
//
//

#include "HomeScene.h"
#include "depends/component/screenadapter/CMVisibleRect.h"
#include "SSCIAPManager.h"
#include "scene/loading/AdsLoadingScene.h"
#include "component/actions/ActionHelper.h"
#include "scene/shop/ShopScene.h"
#include "scene/loading/LoadingTransitionScene.h"
#include "SoundConfig.h"
#include "DialogLayer.h"
#include "ComponentTouchMove.h"
#include "Charactor.h"
#include "SceneHeader.h"
#include "SSCMoreGameButton.h"
#include "Analytics.h"
#include "SSCInternalLibManager.h"
#include "IAPAapter.h"

static bool s_isFirst = true;


HomeScene::HomeScene()
{
}
HomeScene::~HomeScene()
{
    
}



bool HomeScene::init()
{
    if (!StudioLayer::init())
        return false;
    

    
    Analytics::getInstance()->sendScreenEvent("food0.0_movienight_home");
    
    this->setBackground("res/home/home_bg.png");
    auto girl = Sprite::create("res/home/person1.png");
    girl->setPosition(CMVisibleRect::getPosition(506, 335));
    this->addToContentLayer(girl,10);
    
    auto stand = ActionHelper::createAnimate("res/home/person%d.png", 1, 3,0.3f);
    girl->runAction(Sequence::create(stand,
                                     DelayTime::create(0.5f),
                                     stand,
                                     NULL));

    auto showUIcall = [=]
    {
        auto logo = Sprite::create("res/home/logo.png");
        logo->setAnchorPoint(Vec2(0.5, 1));
        logo->setPosition(CMVisibleRect::getPosition(320, 30-300,kBorderNone,kBorderTop));
        this->addToUILayer(logo,10);
        
        auto move = EaseBackOut::create(MoveBy::create(0.5f, Vec2(0, -300)));
        auto call = CallFunc::create([=]
                                     {
                                         logo->runAction(RepeatForever::create(Sequence::create(RotateTo::create(3.f, 5),
                                                                                                RotateTo::create(3.f, 0),
                                                                                                RotateTo::create(3.f, -5),
                                                                                                RotateTo::create(3.f, 0),
                                                                                                NULL)));
                                     });
        logo->runAction(Sequence::create(move,NULL));
        
        Button* shopBtn = Button::create("ui/home/shop.png");
        shopBtn->setPosition(CMVisibleRect::getPosition(17, 25+100-40,kBorderLeft,kBorderBottom));
        this->addToUILayer(shopBtn,10,5);
        shopBtn->setAnchorPoint(Vec2(0, 0));
        shopBtn->setOpacity(0);
        
        Button* playBtn = Button::create("ui/home/play.png");
        playBtn->setPosition(CMVisibleRect::getPosition(284, 419));
        this->addToUILayer(playBtn,10,8);
        playBtn->setOpacity(0);
        
        shopBtn->runAction(Sequence::create(Spawn::create(FadeIn::create(0.3f),MoveBy::create(0.3f, Vec2(0, 20)), NULL),
                                            NULL));
        playBtn->runAction(Spawn::create(FadeIn::create(0.3f),MoveBy::create(0.3f, Vec2(0, 20)),nullptr));
        playBtn->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.8, 1.2), ScaleTo::create(0.8, 1.0), NULL)));
        
        shopBtn->addTouchEventListener(CC_CALLBACK_2(HomeScene::onButtonClick, this));
        playBtn->addTouchEventListener(CC_CALLBACK_2(HomeScene::onButtonClick, this));
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        shopBtn->setVisible(false);
//        shopBtn->setTouchEnabled(false);
#endif
        
        auto moreGameButton = SSCMoreGameButton::create();
        moreGameButton->showButton(this, true);
        moreGameButton->setClickCall([=](SSCMoreGameButton*){
            
            SSCMoreGameManager::getInstance()->show(MGAT_EXPAND);
        });
        
        auto rateBtn = Button::create("ui/home/rateus.png");
        rateBtn->setAnchorPoint(Vec2(0, 1));
        rateBtn->setPosition(CMVisibleRect::getPosition(20, 20,kBorderLeft,kBorderTop));
        this->addToUILayer(rateBtn,10,11);
        rateBtn->addTouchEventListener(CC_CALLBACK_2(HomeScene::onButtonClick, this));

        auto privacyBtn = Button::create("ui/home/pricacypolicy.png");
        privacyBtn->setAnchorPoint(Vec2(1, 1));
        privacyBtn->setPosition(CMVisibleRect::getPosition(20, 20,kBorderRight,kBorderTop));
        this->addToUILayer(privacyBtn,10,12);
        privacyBtn->addTouchEventListener(CC_CALLBACK_2(HomeScene::onButtonClick, this));

    };
    
    
    showUIcall();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(HomeScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
    
#endif
    
    //启动广告
    
    //    this->runAction(Sequence::create(DelayTime::create(1.3),
    //                                     CallFunc::create([=]
    //    {
    //
    //        if (s_isFirst)
    //        {
    ////            if (SSCIAPManager::getInstance()->isShowAds())
    ////            {
    ////#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ////                AdsLoadingScene::showLoading<AdsLoadingScene>(true);
    ////                log("======android add loading=====");
    ////#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ////                AdsManager::getInstance()->showAds(kTypeInterstitialAds);
    ////#endif
    ////            }
    //            s_isFirst = false;
    //        }
    //    }),
    //                                     NULL));
    //
    
    
    
    return true;
}


void HomeScene::onButtonClick(Ref* ref,Widget::TouchEventType type)
{
    auto btn = dynamic_cast<Button*>(ref);
    
    if (type == Widget::TouchEventType::ENDED)
    {
        
        switch (btn->getTag())
        {
                break;
            case 5: // shop
            {
                kAudioUtil->stopAllEffect();
                ShopScene* layer = ShopScene::create();
                this->addChild(layer,9999);
                CMAudioUtil::getInstance()->playEffect(kSoundClickButton);
                
            }
                break;
                break;
            case 8: // play
            {
                kAudioUtil->stopAllEffect();
                btn->setTouchEnabled(false);
                CMAudioUtil::getInstance()->playEffect(kSoundStart);
                SceneManager::getInstance()->replaceWithDefaultTransition(ChooseFoodScene::scene());
                CMAudioUtil::getInstance()->playBackGroundMusic(kBGMInModule);
            }
                break;
            case 11: // rate
            {
                SSCInternalLibManager::getInstance()->rateUs();
                
            }
                break;
            case 12: // privacy
            {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                Application::getInstance()->openURL("http://www.kidsfoodinc.com/privacy/");
#else
                Application::getInstance()->openURL("http://www.shakeitmediainc.com/privacy-policy/");
#endif
            }
                break;
            default:
                break;
        }
    }
    
}

void HomeScene::onEnterTransitionDidFinish()
{
    StudioLayer::onEnterTransitionDidFinish();
    if (s_isFirst){
        this->runAction(Sequence::create(DelayTime::create(0.3),
                                         CallFunc::create([=]
                                                          {
                                                              AdsLoadingScene::showLoading<AdsLoadingScene>(true,nullptr,INT_MAX);
                                                              AdsLoadingScene::loadingDoneCallback = [=]
                                                              {
                                                                  AdsLoadingScene::s_showAdsBg = true;
                                                        
                                                              };

                                                          }),nullptr));
        
        s_isFirst = false;
    }
    if (SSCIAPManager::getInstance()->isShowAds())
    {
        AdsManager::getInstance()->showAds(kTypeBannerAds);
    }
    CMAudioUtil::getInstance()->playBackGroundMusic(kBGMDefault);
    //    kAudioUtil->setBgMusicVolume(0.5f);
    kAudioUtil->playEffect(kVoiceStart);
    
    
    
    
}

void HomeScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    
    CCLOG("%d", keyCode);
    
    if (Director::getInstance()->getRunningScene()->getChildByTag(129))
    {
        
        Director::getInstance()->getRunningScene()->removeChildByTag(129);
        return;
    }
    
    if (!AdsLoadingScene::s_enableBackClick)
    {
        return;
    }
    
    
    
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE )
    {
        DialogLayer::showWithMessage("Are you sure you \nwant to exit game?", [=](){
            
            Director::getInstance()->end();
            
        }, nullptr, Director::getInstance()->getRunningScene(), 40.f);
    }
    
}

void HomeScene::onExit()
{
    StudioLayer::onExit();
    SSCMoreGameManager::getInstance()->delegate = nullptr;
    
}

