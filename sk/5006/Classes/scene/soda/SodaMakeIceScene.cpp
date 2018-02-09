//
//  SodaMakeIceScene.cpp
//  CCATS022
//
//  Created by liji on 16/11/10.
//
//

#include "SodaMakeIceScene.h"
#include "ESDataManager.h"
#include "IceContainerNode.h"
#include "LiquidNode.h"
#include "ComponentTouchMoveToTarget.h"
#include "IceBoxNode.h"
#include "SceneHeader.h"
REGIST_SECENE(SodaMakeIceScene)
SodaMakeIceScene::SodaMakeIceScene():
_iceContainer(nullptr),
_kettle(nullptr),
_waterIdd(0)
{

}

SodaMakeIceScene::~SodaMakeIceScene()
{

}

bool SodaMakeIceScene::init()
{
    if (!ESMakeBaseScene::init())
    {
        return false;
    }
    _iapIdentify = ShopScene::getAllFoodString();
    _screenEventName = "food1.0_core_rainbowslushy";

    this->setBackground("res/rainbow_slushy/bg-1.png");
    
    DecorationConfig config = DecorationConfig("soda",
                                               "ice",
                                               "",
                                               "",
                                               "res/rainbow_slushy/ice/ice_%d.png",
                                               "",
                                               1,
                                               6
                                               );
    
    ScrollViewConfig sConfig = ScrollViewConfig
    (
     Size(getContentSize().width,383),
     "",
     "",
     CMVisibleRect::getPosition(320, 480),
     CMVisibleRect::getPosition(320, 210-300,kBorderNone,kBorderTop),
     247+30
     );
    
    
    auto callback = [=](ui::Widget* item)
    {
        //            setTypeEnable(false);
        int itemTag = item->getTag();
        log("===item clicked %d====",itemTag);
        auto food = kESDataManager->getSodaFood();
        food.iceIndex = item->getTag();
        kESDataManager->setSodaFood(food);
        this->iceBoxChoosed();
        this->runAction(CallFunc::create([=]
        {
            this->removeScrollView(false);

        }));
    };
    
    this->createScrollView(config,callback,sConfig,false);

    
    
    return true;
}


void SodaMakeIceScene::iceBoxChoosed()
{
    if (!_iceContainer)
    {
        _iceContainer = IceContainerNode::create();
        this->addToContentLayer(_iceContainer,11);
    }

    
    auto call = [=]
    {
        kAudioUtil->playEffect(kVoiceJuicePourToModule);

        _kettle = Sprite::create("res/rainbow_slushy/ice/kettle1.png");
        this->addToContentLayer(_kettle,12);
        _kettle->setScale(0.8f);
        ActionHelper::moveInToSceneBounce(_kettle, CMVisibleRect::getPosition(320, 836), kActionDirectionRight,[=]
        {
            TipsUIController::getInstance()->showMoveTipBezel(this, CMVisibleRect::getPosition(320, 836), CMVisibleRect::getPosition(320, 382));
            ComponentTouchMoveToTarget* cMove = ComponentTouchMoveToTarget::create(_iceContainer->convertToWorldSpace(_iceContainer->_icePosVec.at(0)) +Vec2(150,240));
            cMove->setBackToPointLocal(CMVisibleRect::getPosition(567, 436));
            cMove->setMaxDistanceForMoveToTarget(700.f);
            _kettle->addComponent(cMove);
            cMove->componentTouchMoveToTargetDoneCall = [=]
            {
                _kettle->runAction(ScaleTo::create(0.5f, 1));
                cMove->setEnableMove(false);
                this->startPour();
                TipsUIController::getInstance()->removeAllTips();
            };
            cMove->start();
        });
    };
    
    ActionHelper::moveInToSceneBounce(_iceContainer, CMVisibleRect::getPosition(320, 382), kActionDirectionLeft,call);
}

void SodaMakeIceScene::startPour()
{
    auto flow = Sprite::create("res/rainbow_slushy/ice/kettle2.png");
    _kettle->addChild(flow,-1);
    flow->runAction(RepeatForever::create(ActionHelper::createAnimate("res/rainbow_slushy/ice/kettle2_flow%d.png", 1, 2)));
    _kettle->setTexture("res/rainbow_slushy/ice/kettle2.png");
    this->getNextPos();
    
}

void SodaMakeIceScene::getNextPos()
{
    kAudioUtil->stopEffectBySoundId(_waterIdd);
    static int index = -1;
    index++;
    if (index>=5)
    {
        index = -1;
        kAudioUtil->stopEffectBySoundId(_waterIdd);
        ActionHelper::moveOutofSceneAndRemoveSelf(_kettle, kActionDirectionLeft,CC_CALLBACK_0(SodaMakeIceScene::freezeIce, this));
        return ;
    }
    Vec2 pos = _iceContainer->convertToWorldSpace(_iceContainer->_icePosVec.at(index)) +Vec2(150,240);
    
    auto floatAction = ActionHelper::createFloatAction(0.2);
    _kettle->runAction(Sequence::create(MoveTo::create(0.8f, pos),
                                        CallFunc::create([=]
                                                         {
                                                             _waterIdd = kAudioUtil->playEffect(kSoundAddWater,true);
                                                             _iceContainer->pourWaterIn(index);
                                                         }),
                                        Repeat::create(floatAction, 5),
                                        CallFunc::create([=]
    {
        this->getNextPos();
    }),
                                        NULL));
}

void SodaMakeIceScene::freezeIce()
{
    
    static bool s_first = true;
    static bool s_first1 = true;
    auto iceBox = IceBoxNode::create();
    this->addToContentLayer(iceBox,2);
    ActionHelper::moveInToSceneBounce(iceBox, CMVisibleRect::getPosition(320, 460), kActionDirectionLeft);
    iceBox->successFroozenCallback=[=]
    {
        
        kAudioUtil->playEffect(kSoundHappy);
        log("next");
        this->showNextButton();
        this->onNextButtonTouchCallback = []
        {
            kESDataManager->setNextStep(2);
            SceneManager::getInstance()->replaceWithDefaultTransition(SodaBlendScene::scene());
        };

    };
    iceBox->froozenTimeCallback=[=](float time)
    {
        if (time>iceBox->getFrozenDoneZone().x)
        {
            if (s_first1)
            {
                s_first1 = false;
                TipsUIController::getInstance()->showClickTip(this, CMVisibleRect::getPosition(579, 256));
            }
        }
        if (time>iceBox->getFrozenDoneZone().y)
        {
            if (s_first)
            {
                s_first = false;
                kAudioUtil->playEffect(kSoundFrozen);
                TipsUIController::getInstance()->removeAllTips();
            }
        }
        if (time>=iceBox->getFroozenDuration())
        {
            _iceContainer->resetFreeze();
        }
    };

    iceBox->onOffCallback=[=](bool isOn)
    {
        kAudioUtil->stopAllEffect();
        if (isOn)
        {
            _iceContainer->freeze(iceBox->getFrozenDoneZone().x);
            kAudioUtil->playEffect(kSoundFreeze);
        }
        TipsUIController::getInstance()->removeAllTips();
        kAudioUtil->playEffect(kSoundClickButton);
    };
    iceBox->failFroozenCallback=[=]
    {
        kAudioUtil->stopAllEffect();
        _iceContainer->resetFreeze();
        s_first = true;
        s_first1 = true;
    };

    auto call = [=]
    {
        _iceContainer->retain();
        _iceContainer->removeFromParent();
        iceBox->addChild(_iceContainer,3);
        _iceContainer->setPosition(Vec2(0, -81));
        _iceContainer->runAction(Spawn::create(MoveTo::create(0.6f, Vec2(0, -40)),
                                               ScaleTo::create(0.6f, 0.8f),
                                               CallFunc::create([=]
        {
            TipsUIController::getInstance()->showClickTip(this, CMVisibleRect::getPosition(579, 256));
            iceBox->showProgressTimer();
        }),
                                               NULL));

    };

    _iceContainer->runAction(Sequence::create(MoveTo::create(1.2f, CMVisibleRect::getPosition(320, 382)),
                                              DelayTime::create(0.7f),
                                              CallFunc::create(call),
                                              NULL));

}

void SodaMakeIceScene::onEnterTransitionDidFinish()
{
    ESMakeBaseScene::onEnterTransitionDidFinish();
    
    if (kESDataManager->showAds())
    {
//        AdsManager::getInstance()->showAds(kTypeInterstitialAds);
    }
    
}


