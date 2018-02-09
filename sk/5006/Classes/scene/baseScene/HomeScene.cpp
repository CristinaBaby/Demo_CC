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
#include "RateUsNode.h"
#include "ComponentTouchMove.h"

static bool s_isFirst = true;

REGIST_SECENE(HomeScene)

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
    
    Analytics::getInstance()->sendScreenEvent("food1.0_home");
    
    this->setBackground("res/start/start_bg.png");
    
    auto showUIcall = [=]
    {
        auto logo = Sprite::create("res/start/logo.png");
        logo->setAnchorPoint(Vec2(0.5, 1));
        logo->setPosition(CMVisibleRect::getPosition(320, 40-300,kBorderNone,kBorderTop));
        this->addToUILayer(logo,10);
        
        auto move = EaseBackOut::create(MoveBy::create(1.2f, Vec2(0, -300)));
        auto call = CallFunc::create([=]
                                     {
                                         logo->runAction(RepeatForever::create(Sequence::create(RotateTo::create(3.f, 5),
                                                                                                RotateTo::create(3.f, 0),
                                                                                                RotateTo::create(3.f, -5),
                                                                                                RotateTo::create(3.f, 0),
                                                                                                NULL)));
                                     });
        logo->runAction(Sequence::create(move,NULL));
        
        Button* shopBtn = Button::create("ui/home/store.png");
        shopBtn->setPosition(CMVisibleRect::getPosition(17, 25+100-30,kBorderLeft,kBorderBottom));
        this->addToUILayer(shopBtn,10,5);
        shopBtn->setAnchorPoint(Vec2(0, 0));
        shopBtn->setOpacity(0);
        
        Button* playBtn = Button::create("ui/home/play_btn.png");
        playBtn->setPosition(CMVisibleRect::getPosition(382-800,413));
        this->addToUILayer(playBtn,10,8);
//        playBtn->setOpacity(0);
        playBtn->runAction(Sequence::create(Spawn::create(RotateBy::create(2.4, 720),
                                                          MoveBy::create(2.4f, Vec2(800, 0)),
                                                          NULL),
                                            CallFunc::create([=]
        {
            playBtn->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.8, 1.15), ScaleTo::create(0.8, 1.0), NULL)));
            playBtn->addTouchEventListener(CC_CALLBACK_2(HomeScene::onButtonClick, this));
        }),
                                            NULL));
        shopBtn->runAction(Sequence::create(Spawn::create(FadeIn::create(0.3f),MoveBy::create(0.3f, Vec2(0, 20)), NULL),
                                            NULL));
        
        shopBtn->addTouchEventListener(CC_CALLBACK_2(HomeScene::onButtonClick, this));
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        //        shopBtn->setVisible(false);
        //        shopBtn->setTouchEnabled(false);
#endif
        
        auto moreGameButton = SSCMoreGameButton::create();
        moreGameButton->showButton(this, true);
        moreGameButton->setClickCall([=](SSCMoreGameButton*){
            
            SSCMoreGameManager::getInstance()->show(MGAT_EXPAND);
        });
        auto s = moreGameButton->getButtonSize();


        //moreGameButton->setPosition(CMVisibleRect::getPosition(20+s.width/2, 25+100-30+s.height/2,kBorderRight,kBorderBottom));
        //moreGameButton->setPosition(Vec2(568,160));
        auto director = Director::getInstance();
        auto size =  director->getWinSize();
        CCLOG("size ....... %f  %f ",size.width,size.height);
        moreGameButton->setPosition(Vec2(size.width - 70,160));

       
        
        auto rateBtn = Button::create("ui/home/rateus.png");
        rateBtn->setAnchorPoint(Vec2(0, 1));
        rateBtn->setPosition(CMVisibleRect::getPosition(20, 20,kBorderLeft,kBorderTop));
        this->addToUILayer(rateBtn,10,11);
        rateBtn->addTouchEventListener(CC_CALLBACK_2(HomeScene::onButtonClick, this));
        
        auto privacyBtn = Button::create("ui/home/privacy.png");
        privacyBtn->setAnchorPoint(Vec2(1, 1));
        privacyBtn->setPosition(CMVisibleRect::getPosition(20, 20,kBorderRight,kBorderTop));
        this->addToUILayer(privacyBtn,10,12);
        privacyBtn->addTouchEventListener(CC_CALLBACK_2(HomeScene::onButtonClick, this));
        
    };
    
    vector<string> iconVec = {"res/start/cakepop1.png","res/start/slushy.png","res/start/cupcake.png"};
    vector<Vec2> posVec = {CMVisibleRect::getPosition(160, 473),CMVisibleRect::getPosition(515, 544),CMVisibleRect::getPosition(380,262)};
    
    for (int i = 0; i<3;i++)
    {
        auto sp = Sprite::create(iconVec.at(i));
        this->addToContentLayer(sp,3,i);
        sp->setPosition(posVec.at(i));
        auto cTouch = ComponentTouchMove::create();
        cTouch->setIsCanMove(false);
        sp->addComponent(cTouch);
        cTouch->touchEndedCall=[=](Touch* t)
        {
            int tag = sp->getTag();
            if (tag == 2)
            {
                auto p = ParticleSystemQuad::create("particle/chosen.plist");
                sp->addChild(p);
                p->setPosition(sp->getContentSize()/2);
                p->setDuration(1.2f);
            }
            else
            {
                sp->runAction(ActionHelper::createRollJelly());
            }
        };
    }
    
    showUIcall();
    
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyPressed = CC_CALLBACK_2(HomeScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
    
#endif
    
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
                static bool s_isClicked = false;
                if (s_isClicked)
                {
                    return;
                }
                s_isClicked = true;
                this->runAction(Sequence::create(DelayTime::create(0.5f),
                                                 CallFunc::create([=]
                                                                  {
                                                                      s_isClicked = false;
                                                                  }),
                                                 NULL));

                kAudioUtil->stopAllEffect();
                ShopScene* layer = ShopScene::create();
                layer->setPosition(CMVisibleRect::getPosition(0, 0));
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
                SceneManager::getInstance()->replaceWithDefaultTransition(TransitionAnimateScene::scene());
            }
                break;
            case 11: // rate
            {
//                SSCInternalLibManager::getInstance()->rateUs();
                auto rate = RateUsNode::create();
                this->addChild(rate,99999);
                rate->setPosition(CMVisibleRect::getPosition(0, 0));
                
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
//    if (s_isFirst){
//        this->runAction(Sequence::create(DelayTime::create(0.3),
//                                         CallFunc::create([=]
//                                                          {
//                                                              if (SSCIAPManager::getInstance()->isShowAds())
//                                                                  AdsManager::getInstance()->showAds(kTypeCrosspromoAds);
//
////                                                              AdsLoadingScene::showLoading<AdsLoadingScene>(true,nullptr,INT_MAX);
////                                                              AdsLoadingScene::loadingDoneCallback = [=]
////                                                              {
////                                                                  AdsLoadingScene::s_showAdsBg = true;
////                                                                  
////                                                              };
//
//                                                          }),nullptr));
//        
////        s_isFirst = false;
//    }
    if (SSCIAPManager::getInstance()->isShowAds())
    {
        AdsManager::getInstance()->showAds(kTypeInterstitialAds);
        AdsManager::getInstance()->showAds(kTypeBannerAds);
    }
    CMAudioUtil::getInstance()->playBackGroundMusic(kBGMDefault);
    kAudioUtil->setBgMusicVolume(bgmVolume);
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
            
        }, nullptr, Director::getInstance()->getRunningScene(), 35.f);
    }
    
}

void HomeScene::onExit()
{
    StudioLayer::onExit();
    SSCMoreGameManager::getInstance()->delegate = nullptr;
    
}

