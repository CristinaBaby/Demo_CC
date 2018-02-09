//
//  RainbowMakeScene.cpp
//  shake5008
//
//  Created by liji on 17/1/3.
//
//

#include "RainbowMakeScene.h"
#include "BowlNode.h"
#include "ScoopSpoon.h"
#include "MixSpoonNode.h"
#include "ComponentTouchMove.h"
#include "SceneHeader.h"
#include "SaltNode.h"
#include "EggNode.h"
#include "LiquidNode.h"
#include "ButterNode.h"
#include "ComponentAccelerateRotate.h"
#include "RewardManager.h"
#include "LockUIManager.h"
#include "MiniShopScene.h"
REGIST_SECENE(RainbowMakeScene)

RainbowMakeScene::RainbowMakeScene():
_bowl(nullptr),
_saltIdd(0),
_eggCount(0),
_butterTipIndex(0),
_sifter(nullptr)
{
    _steps.insert(std::make_pair("stepAddIngredients1", CC_CALLBACK_0(RainbowMakeScene::stepAddIngredients1, this)));
    _steps.insert(std::make_pair("stepAddIngredients2", CC_CALLBACK_0(RainbowMakeScene::stepAddIngredients2, this)));
    _steps.insert(std::make_pair("stepAddCornPowder", CC_CALLBACK_0(RainbowMakeScene::stepAddCornPowder, this)));
    _steps.insert(std::make_pair("stepMix", CC_CALLBACK_0(RainbowMakeScene::stepMix, this)));
    _steps.insert(std::make_pair("stepOver", CC_CALLBACK_0(RainbowMakeScene::stepOver, this)));
    
    _stepName =
    {
        "stepAddIngredients1",
        "stepAddIngredients2",
        "stepAddCornPowder",
        "stepMix",
        "stepOver",
    };

}

RainbowMakeScene::~RainbowMakeScene()
{

}

bool RainbowMakeScene::init()
{
    if (!ESMakeBaseScene::init())
    {
        return false;
    }
    
    if (kESDataManager->getCurrentFood() == ESDataManager::FoodType::kFoodCakepop)
    {
        _screenEventName = "food1.0_core_rainbowcakepop";
    }
    if (kESDataManager->getCurrentFood() == ESDataManager::FoodType::kFoodCupcake)
    {
        _screenEventName = "food1.0_core_rainbowcupcake";
    }

    
    this->setBackground("res/cake_pop/cake_pop_bg1.png");
    
    this->initBowl();
    
    return true;
}

void RainbowMakeScene::initBowl()
{
    if (!_bowl)
    {
        _bowl = BowlNode::create("res/cake_pop/step1_make/bowl.png", "res/cake_pop/step1_make/bowl1_1.png");
        this->addToContentLayer(_bowl,1);
    }
    
    ActionHelper::moveInToSceneBounce(_bowl, CMVisibleRect::getPosition(320, 282), kActionDirectionLeft,CC_CALLBACK_0(RainbowMakeScene::showNextStep, this));
}

void RainbowMakeScene::stepAddIngredients1()
{
    _ingredientsCount = 3;
    this->addPowder();
    this->addSalt();
    this->addMilk();
    
}

void RainbowMakeScene::addMilk()
{
    auto milk = LiquidNode::create("res/cake_pop/step1_make/milk.png");
    addToContentLayer(milk, 11);
    
    auto milkInCall = [=]
    {
        milk->onTouchBegan=[=]
        {
            TipsUIController::getInstance()->removeAllTips();
        };
        
        milk->onMoveToTargetCallback = [=]()
        {
            auto node = TipsUIController::getInstance()->showAccelerateTip(this, milk->convertToWorldSpaceAR(Vec2::ZERO) + Vec2(-150, 0.0),false);
            node->setLocalZOrder(100);
        };
        
        milk->setPourWorldPoint(Vec2(_bowl->getPosition().x+144, _bowl->getPosition().y+234), [=](){
            TipsUIController::getInstance()->removeAllTips();

            auto floatAction = ActionHelper::createFloatAction(0.3);
            milk->runAction(Sequence::create(
                                             Repeat::create(floatAction, 20/3),
                                             NULL));
            
            int idd = kAudioUtil->playEffect(kSoundAddWater,true);
            
            auto animate = ActionHelper::createAnimate("res/cake_pop/step1_make/milk%d.png", 0, 2);
            auto milkSprite = Sprite::create("res/cake_pop/step1_make/milk0.png");
            milkSprite->setPosition(CMVisibleRect::getPosition(311+44, 428+15+10));
            this->addToContentLayer(milkSprite,22);
            auto temp = Sprite::create("res/cake_pop/step1_make/bowl_milk2.png");
            _bowl->addContent(temp, "milk", 1,Vec2(12, 30));
            temp->runAction(FadeIn::create(5.5));
            _tempItemVec.push_back(temp);

            auto floatAction1 = ActionHelper::createFloatAction(0.3);
            milkSprite->runAction(Sequence::create(
                                                   Repeat::create(floatAction1, 20/3),
                                                   NULL));
            
            milkSprite->runAction(Sequence::create(
                                                   Repeat::create(animate, 20),
                                                   CallFunc::create([=]()
                                                                    {
                                                                        kSoundInstance->playRandomStepFinishVoice();
                                                                        kAudioUtil->stopEffectBySoundId(idd);
                                                                        TipsUIController::getInstance()->removeAllTips();
                                                                        milkSprite->removeFromParent();
                                                                        ActionHelper::moveOutofSceneAndRemoveSelf(milk, kActionDirectionRight,[=]
                                                                                                                  {
                                                                                                                      if (this->isStepOver())
                                                                                                                      {
                                                                                                                          
                                                                                                                          this->showNextStep();
                                                                                                                      }
                                                                                                                  });
                                                                    }),
                                                   NULL));
        });
    };
    ActionHelper::moveInToSceneExponentialOut(milk, CMVisibleRect::getPosition(116,766), ActionDirection::kActionDirectionLeft,milkInCall);
}
void RainbowMakeScene::addPowder()
{
    auto flour = BowlNode::create("res/cake_pop/step1_make/baking_powder.png", "");
    this->addToContentLayer(flour,3);
    auto temp = Sprite::create("res/cake_pop/step1_make/bowl_powder.png");
    _bowl->addContent(temp, "flour", 2,Vec2(0, 0));
    _tempItemVec.push_back(temp);
    
    auto ingredientsInCall = [=]
    {
        TipsUIController::getInstance()->showMoveTipBezel(this, CMVisibleRect::getPosition(492, 676), CMVisibleRect::getPosition(354, 310));
        
        auto pourCall = [=]
        {
            TipsUIController::getInstance()->removeAllTips();
            TipsUIController::getInstance()->showAccelerateTip(this, flour->getPosition(),false);
            flour->beginPour(-70, [=]
                             {
                                 kAudioUtil->playEffect(kSoundAddPowder,true);
                                 
                                 TipsUIController::getInstance()->removeAllTips();
                                 auto p = ParticleSystemQuad::create("particle/flour.plist");
                                 flour->addChild(p,20);
                                 p->setPosition(-48+25,106);
                                 p->setRotation(flour->getRotation()*-1);
                                 
                                 auto m1 = ActionHelper::moveToPostionByBezier(CMVisibleRect::getPosition(486, 525), CMVisibleRect::getPosition(486-60, 525), 1.2,Vec2(60,50));
                                 auto m2 = ActionHelper::moveToPostionByBezier(CMVisibleRect::getPosition(486-60, 525), CMVisibleRect::getPosition(486, 525), 0.8f,Vec2(60,60));
                                 
                                 _bowl->setContentVisible("flour");
                                 
                                 flour->runAction(Sequence::create(m1,
                                                                   m2,
                                                                   m1,
                                                                   m2,
                                                                   CallFunc::create([=]
                                                                                    {
                                                                                        kSoundInstance->playRandomStepFinishVoice();
                                                                                        p->removeFromParent();
                                                                                        kAudioUtil->stopAllEffect();
                                                                                        auto doneCall = [=]
                                                                                        {
//                                                                                            _bowl->showHeart(Vec2::ZERO, CC_CALLBACK_0(RainbowMakeScene::checkNextStep, this));
                                                                                            this->checkNextStep();
                                                                                        };
                                                                                        
                                                                                        ActionHelper::moveOutofSceneAndRemoveSelf(flour, kActionDirectionRight,doneCall);
                                                                                    }),
                                                                   NULL));
                             }, true);
        };
        
        
        flour->setPourWorldPoint(CMVisibleRect::getPosition(440, 545),pourCall);
    };
    
    ActionHelper::moveInToSceneBounce(flour, CMVisibleRect::getPosition(524,751), kActionDirectionRight,ingredientsInCall);
}

void RainbowMakeScene::addSalt()
{
    auto salt = SaltNode::create("res/cake_pop/step1_make/salt.png");
    addToContentLayer(salt, 3);
    auto call = [=]
    {
        salt->setPourWorldPoint(Vec2(_bowl->getPosition().x + 130, _bowl->getPosition().y + 230), [=](){
            
            _saltIdd = kAudioUtil->playEffect(kSoundAddSugar,true);
            
            auto par = ParticleSystemQuad::create("particle/saltParticle.plist");
            _bowl->addContent(par, "par2", 3,Vec2(80,160));
            par->setDuration(3.2f);
            auto inbowl = Sprite::create("res/cake_pop/step1_make/bowl_salt.png");
            _bowl->addContent(inbowl, "Salt", 1,Vec2(-20,40));
            inbowl->setOpacity(0);
            inbowl->runAction(FadeIn::create(3.2f));
        },[=]()
                                {
                                    kSoundInstance->playRandomStepFinishVoice();
                                    kAudioUtil->stopEffectBySoundId(_saltIdd);
                                    ActionHelper::moveOutofSceneAndRemoveSelf(salt, kActionDirectionLeft,CC_CALLBACK_0(RainbowMakeScene::checkNextStep, this));
                                });
    };
    
    ActionHelper::moveInToSceneBounce(salt, CMVisibleRect::getPosition(328,744), ActionDirection::kActionDirectionLeft,call);
}


void RainbowMakeScene::stepAddIngredients2()
{
    _ingredientsCount = 3;
    this->addEgg();
    this->addSugar();
    this->addButter();

}


void RainbowMakeScene::addEgg()
{
    auto eggPlate = Sprite::create("res/cake_pop/step1_make/egg_plate.png");
    addToContentLayer(eggPlate, 7);
    
    auto egg1 = Sprite::create("res/cake_pop/step1_make/bowl_egg.png");
    auto egg2 = Sprite::create("res/cake_pop/step1_make/bowl_egg.png");
    auto egg3 = Sprite::create("res/cake_pop/step1_make/bowl_egg.png");
    _bowl->addContent(egg1, "Egg1", 3, Vec2(-100.0, 40));
    _bowl->addContent(egg2, "Egg2", 3, Vec2(0.0, 40+40));
    _bowl->addContent(egg3, "Egg3", 3, Vec2(100.0, 40));
//    _bowl->addContent(egg1, "Egg1", 3);
//    _bowl->addContent(egg2, "Egg2", 3);
//    _bowl->addContent(egg3, "Egg3", 3);
    
    _tempItemVec.push_back(egg1);
    _tempItemVec.push_back(egg2);
    _tempItemVec.push_back(egg3);
    
    auto createEgg = [=](const Vec2& position, int index) -> EggNode*
    {
        auto egg = EggNode::create(StringUtils::format("res/cake_pop/step1_make/egg_plate%d.png",index));
        egg->setPosition(Vec2(eggPlate->getContentSize().width/2, eggPlate->getContentSize().height/2) + position);
        egg->onMoveToTargetCallback = [=]()
        {
            auto node = TipsUIController::getInstance()->showClickTip(this, egg->convertToWorldSpaceAR(Vec2::ZERO));
            node->setLocalZOrder(100);
        };
        egg->setPourWorldPoint(Vec2(_bowl->getPosition().x, _bowl->getPosition().y + 200.0), [=](){
            
            _eggCount++;
            kAudioUtil->playEffect(kSoundBreakEgg);
            
            TipsUIController::getInstance()->removeAllTips();
            auto animate = ActionHelper::createAnimate("res/cake_pop/step1_make/egg%d.png", 0, 3,0.3);
            auto eggSprite = Sprite::create("res/cake_pop/step1_make/egg0.png");
            
            eggSprite->setPosition(Vec2(egg->convertToWorldSpaceAR(Vec2(-35.0+30*_eggCount, -130.0+30*_eggCount))));
            eggSprite->runAction(Sequence::create(animate, RemoveSelf::create(), NULL));
            this->addToContentLayer(eggSprite, 26);
            egg->setVisible(false);
            //            egg->removeAllComponents();
            
            _bowl->runAction(Sequence::create(DelayTime::create(0.9), CallFunc::create([=](){
                
                _bowl->setContentVisible(StringUtils::format("Egg%d",index), [=](){
                    
                    if (_eggCount >= 3)
                    {
                        _eggCount = 0;
                        ActionHelper::moveOutofSceneAndRemoveSelf(eggPlate, ActionDirection::kActionDirectionRight, [=](){
                            
                            //                            _hasAddEgg = true;
                            //                            checkFinishAdd();
                            this->checkNextStep();
                        });
                    }
                });
            }), NULL));
        });
        
        return egg;
    };
    float eggWidth = 134.0;
    float eggHeight = 133;
    eggPlate->addChild(createEgg(Vec2(0.0, 80), 3));
    eggPlate->addChild(createEgg(Vec2(-eggWidth/2 + 30.0, -eggHeight/2 + 80.0), 2));
    eggPlate->addChild(createEgg(Vec2(eggWidth/2 - 30.0, -eggHeight/2 + 80.0), 1));
    ActionHelper::moveInToSceneBounce(eggPlate, CMVisibleRect::getPosition(120,714), ActionDirection::kActionDirectionLeft);
}

void RainbowMakeScene::addButter()
{
    auto butterPlate = Sprite::create("res/cake_pop/step1_make/butter_plate.png");
    auto butter = ButterNode::createPicChange("res/cake_pop/step1_make/butter%d.png",Vec2(0, 4));
    butter->setPosition(butterPlate->getContentSize() * 0.5);
    butterPlate->addChild(butter);
    
    addToContentLayer(butterPlate, 11);
    
    ActionHelper::moveInToSceneBounce(butterPlate, CMVisibleRect::getPosition(537, 711), ActionDirection::kActionDirectionRight);
    
    butter->onMoveToTargetCallback = [=]()
    {
        
        auto node = TipsUIController::getInstance()->showClickTip(this, butter->convertToWorldSpaceAR(Vec2(0.0, -20.00)));
        node->setLocalZOrder(100);
    };
    butter->setPourWorldPoint(Vec2(_bowl->getPosition().x, _bowl->getPosition().y + 200.0), [=](){
        TipsUIController::getInstance()->removeAllTips();
        
        kAudioUtil->playEffect(kSoundAddButter);
        
        auto butterTip = Sprite::create("res/cake_pop/step1_make/bowl_butter0.png");
        butterTip->setPosition(_bowl->convertToNodeSpaceAR(butter->convertToWorldSpaceAR(Vec2::ZERO)));
        _bowl->addChild(butterTip, 3);
        
        float x = random(-60.0, 60.0);
        float y = random(10.0, 50.0);
        float roatate = random(0.0, 360.0);
        
        butterTip->runAction(Spawn::create(MoveTo::create(0.3, Vec2(x, y)), RotateTo::create(0.3, roatate), ScaleTo::create(0.3, 1), NULL));
        
        if (++_butterTipIndex == 5)
        {
            ActionHelper::moveOutofSceneAndRemoveSelf(butterPlate, kActionDirectionRight);
            kSoundInstance->playRandomStepFinishVoice();

            //finish
            butter->removeFromParent();
            this->checkNextStep();
        }
    });
}

void RainbowMakeScene::addSugar()
{
    auto flour = BowlNode::create("res/cake_pop/step1_make/sugar.png", "");
    this->addToContentLayer(flour,3);
    auto temp = Sprite::create("res/cake_pop/step1_make/bowl_sugar.png");
    _bowl->addContent(temp, "sugar", 3);
    _tempItemVec.push_back(temp);
    
    auto ingredientsInCall = [=]
    {
        TipsUIController::getInstance()->showMoveTipBezel(this, CMVisibleRect::getPosition(492, 676), CMVisibleRect::getPosition(354, 310));
        
        auto pourCall = [=]
        {
            TipsUIController::getInstance()->removeAllTips();
            TipsUIController::getInstance()->showAccelerateTip(this, flour->getPosition(),false);
            flour->beginPour(-70, [=]
                             {
                                 kAudioUtil->playEffect(kSoundAddSugar,true);
                                 
                                 TipsUIController::getInstance()->removeAllTips();
                                 auto p = ParticleSystemQuad::create("particle/poursugar.plist");
                                 flour->addChild(p,20);
                                 p->setPosition(-48,106);
                                 p->setRotation(flour->getRotation()*-1);
                                 p->setDuration(6.f);
                                 p->setTexture(_director->getTextureCache()->addImage("particle/water.png"));
                                 
                                 auto m1 = ActionHelper::moveToPostionByBezier(CMVisibleRect::getPosition(486, 525), CMVisibleRect::getPosition(486-60, 525), 1.2,Vec2(60,50));
                                 auto m2 = ActionHelper::moveToPostionByBezier(CMVisibleRect::getPosition(486-60, 525), CMVisibleRect::getPosition(486, 525), 0.8f,Vec2(60,60));
                                 
                                 _bowl->setContentVisible("sugar");
                                 
                                 flour->runAction(Sequence::create(m1,
                                                                   m2,
                                                                   m1,
                                                                   m2,
                                                                   CallFunc::create([=]
                                                                                    {
                                                                                        kSoundInstance->playRandomStepFinishVoice();
                                                                                        p->removeFromParent();
                                                                                        kAudioUtil->stopAllEffect();
                                                                                        auto doneCall = [=]
                                                                                        {
//                                                                                            _bowl->showHeart(Vec2::ZERO, CC_CALLBACK_0(RainbowMakeScene::checkNextStep, this));
                                                                                            this->checkNextStep();

                                                                                        };
                                                                                        
                                                                                        ActionHelper::moveOutofSceneAndRemoveSelf(flour, kActionDirectionRight,doneCall);
                                                                                    }),
                                                                   NULL));
                             }, true);
        };
        
        flour->setPourWorldPoint(CMVisibleRect::getPosition(486, 525),pourCall);
    };
    
    ActionHelper::moveInToSceneBounce(flour, CMVisibleRect::getPosition(308, 764), kActionDirectionLeft,ingredientsInCall);
}

void RainbowMakeScene::stepAddCornPowder()
{
    _sifter = BowlNode::create("res/cake_pop/step1_make/flour_sifter.png","");
    this->addToContentLayer(_sifter,10);
    
    
    auto inCall = [=]
    {
        {
            auto item = Sprite::create("res/cake_pop/step1_make/flour.png");
            this->addToContentLayer(item,1);
            
            auto ingredientsInCall = [=]
            {
                auto spoon = ScoopSpoon::create("res/cake_pop/step1_make/flour_spoon1.png");
                spoon->setContent("res/cake_pop/step1_make/flour_spoon1_1.png",Vec2::ZERO);
                this->addToContentLayer(spoon, 11);
                spoon->setMoveContentOffset(Vec2(-50.0, -50.0));
                
                auto scoopCall = [=]
                {
                    TipsUIController::getInstance()->showClickTip(this, CMVisibleRect::getPosition(570,310));
                    auto dragCall = [=]()
                    {
                        spoon->startDragToWorldPositon(CMVisibleRect::getPosition(570,330));
                    };
                    
                    spoon->onDragCallback = []
                    {
                        TipsUIController::getInstance()->removeAllTips();
                        kAudioUtil->playEffect(kSoundAddSugar);
                    };
                    spoon->onScroopedCallback = [=]
                    {
                        TipsUIController::getInstance()->removeAllTips();
                        TipsUIController::getInstance()->showMoveTipBezel(this, CMVisibleRect::getPosition(570,310), CMVisibleRect::getPosition(300,668));
                        spoon->setBackPosition(CMVisibleRect::getPosition(428,668));
                    };
                    spoon->onFinishPourCallback = [=]()
                    {
                        auto temp = Sprite::create("res/cake_pop/step1_make/sifterl_flour.png");
                        _sifter->addContent(temp, "yeast", 4);
                        //                    _tempItemVec.push_back(temp);
                        _sifter->setContentVisible("yeast");
                    };
                    spoon->backToOriginalPositionCallback = [=]()
                    {
                        auto doneCall = [=]
                        {
                            shakeSifter();
                        };
                        
                        ActionHelper::moveOutofSceneAndRemoveSelf(item, kActionDirectionRight,doneCall);
                        ActionHelper::moveOutofSceneAndRemoveSelf(spoon, kActionDirectionRight);
                    };
                    dragCall();
                };
                
                ActionHelper::moveInToSceneBounce(spoon, CMVisibleRect::getPosition(583,313), ActionDirection::kActionDirectionRight,scoopCall);
                
            };
            ActionHelper::moveInToSceneBounce(item, CMVisibleRect::getPosition(516,175), kActionDirectionRight,ingredientsInCall);
        }
        
    };
    
    ActionHelper::moveInToSceneBounce(_sifter, CMVisibleRect::getPosition(320, 545+80), kActionDirectionLeft,inCall);

}

void RainbowMakeScene::shakeSifter()
{
    Device::setAccelerometerEnabled(true);
    Device::setAccelerometerInterval(0.1);
    _sifter->removeAllComponents();
    auto cTouch = ComponentTouchMove::create();
    cTouch->setIsCanMove(false);
    cTouch->setName("cTouch");
    _sifter->addComponent(cTouch);
    
    static bool s_f =  true;
    
    if (s_f)
    {
        s_f = false;
        kAudioUtil->playEffect("sound/voice over1/Sieve the flour before using it.mp3");
    }
    
    auto cShake = ComponentAccelerateRotate::create(0);
    cShake->setName("cShake");
    _sifter->addComponent(cShake);
    TipsUIController::getInstance()->showClickTip(this, _sifter->getPosition());
    auto shakeCall = [=]
    {
        TipsUIController::getInstance()->removeAllTips();
        auto m = Sequence::create(MoveBy::create(0.2f, Vec2(0, -20)),
                                  MoveBy::create(0.2f, Vec2(0, 40)),
                                  MoveBy::create(0.2f, Vec2(0, -20)),
                                  NULL);
        
        _sifter->runAction(Sequence::create(
                                            Repeat::create(m, 10),
                                            CallFunc::create([=]
                                                             {
                                                                 kAudioUtil->stopAllEffect();
//                                                                 _bowl->showHeart(Vec2::ZERO, CC_CALLBACK_0(RainbowMakeScene::showNextStep, this));
                                                                 this->showNextStep();

                                                                 ActionHelper::moveOutofSceneAndRemoveSelf(_sifter, kActionDirectionRight);
                                                             }),
                                            NULL));
        auto p = ParticleSystemQuad::create("particle/powder.plist");
        _sifter->addChild(p,-1);
        p->setDuration(0.6*10);
        p->setPosition(0, -40);
        
        kAudioUtil->playEffect(kSoundAddSprinkle,true);
        //        _sifter->setContentVisible("corn",nullptr,false);
        //        _sifter->setContentVisible("yeast",nullptr,false);
        
        auto corn = dynamic_cast<Node*>(_sifter->getChildByName("corn"));
        if (corn)
        {
            corn->runAction(Sequence::create(DelayTime::create(3.2f),
                                             FadeOut::create(2.f),
                                             NULL));
        }
        auto yeast = dynamic_cast<Node*>(_sifter->getChildByName("yeast"));
        if (yeast)
        {
            yeast->runAction(Sequence::create(DelayTime::create(3.2f),
                                              FadeOut::create(2.f),
                                              NULL));
        }
        
        auto temp = Sprite::create("res/cake_pop/step1_make/bowl_flour.png");
        _bowl->addContent(temp, "bowl_flour", 3,Vec2(0,40));
        _tempItemVec.push_back(temp);
        _bowl->setContentVisible("bowl_flour");
    };
    
    cShake->componentAccCall = [=](cocos2d::Acceleration *acceleration, cocos2d::Event * e)
    {
        log("x:%.2f, y:%.2f, z:%.2f", acceleration->x, acceleration->y, acceleration->z);
        
        if (abs(acceleration->x)>0.3)
        {
            log("xxxx");
            cTouch->setEnabled(false);
            cShake->pause();
            shakeCall();
            Device::setAccelerometerEnabled(false);
        }
        
    };
    cShake->start();
    
    cTouch->touchEndedCall = [=](Touch* t)
    {
        Device::setAccelerometerEnabled(false);
        cTouch->setEnabled(false);
        cShake->pause();
        log("touchEndedCalltouchEndedCall");
        shakeCall();
    };
}

void RainbowMakeScene::stepMix()
{
    kAudioUtil->playEffect(kVoiceChooseStirTool);
    auto mix = Sprite::create("res/cake_pop/step1_make/mixture1.png");
    _bowl->addContent(mix, "mix", 15,Vec2(0,50));

    auto mixSpoonAuto = MixSpoonNode::create("res/cake_pop/blender/blender1.png");
    this->addToContentLayer(mixSpoonAuto,3,11);
    mixSpoonAuto->setAutoConfig();
    mixSpoonAuto->setScale(0.55f);
    mixSpoonAuto->addlock();
    LockUIManager::getInstance()->updateLockStatus();
    if (mixSpoonAuto->getChildByTag(TAG_REWARD_LOCK))
    {
        mixSpoonAuto->getChildByTag(TAG_REWARD_LOCK)->setScale(1.8f);
    }

    auto mixSpoon = MixSpoonNode::create("res/cake_pop/blender/blender2.png");
    this->addToContentLayer(mixSpoon,3,10);
    mixSpoon->setScale(0.65f);

    auto call1 = [=]
    {
        static bool isFirst = true;
        isFirst = true;
        static bool s_firstHint = true;
        s_firstHint = true;
        static bool s_firstBlend = true;
        s_firstBlend = true;
        
        mixSpoonAuto->mixerTouchBegan = [=](int tag)
        {
            if (s_firstHint)
            {
                s_firstHint = false;
                mixSpoon->removeAllComponents();
                ActionHelper::moveOutofSceneAndRemoveSelf(mixSpoon, kActionDirectionRight);
            }
            mixSpoonAuto->runAction(ScaleTo::create(0.5f, 0.85f));
            TipsUIController::getInstance()->showMixTip90Degree(_bowl, Vec2(0, 0));
            mixSpoonAuto->mixerTouchBegan = nullptr;

        };
        
        mixSpoonAuto->mixPercentCallback = [=](float percent)
        {
            if (isFirst)
            {
                this->initProgressTimer();
                isFirst = false;
                _bowl->setContentVisible("mix", nullptr, false, false);
                TipsUIController::getInstance()->removeAllTips();
                if (_tempItemVec.size()>0)
                {
                    for (auto temp : _tempItemVec)
                    {
                        temp->runAction(Sequence::create(FadeOut::create(0.3f),
                                                         RemoveSelf::create(),
                                                         NULL));
                    }
                    _tempItemVec.clear();
                }
            }
            SoundConfig::getInstance()->playLoopEffect("Liquid mixing.mp3");
            mixSpoonAuto->startDrill();
            if (percent>random(30, 50))
            {
                if (s_firstBlend)
                {
                    kAudioUtil->playEffect(kVoiceStir1);
                    s_firstBlend = false;
                }
            }
            _progressTimer->setPercentage(percent);
            
            if(percent > 99)
            {
                this->removeProgressTimer();
                SoundConfig::getInstance()->stopLoopEffect();
                mixSpoonAuto->mixPercentCallback = nullptr;
                //                _bowl->showHeart(Vec2::ZERO, CC_CALLBACK_0(RainbowMakeScene::showNextStep, this));
                this->showNextStep();
                ActionHelper::moveOutofSceneAndRemoveSelf(mixSpoonAuto, ActionDirection::kActionDirectionLeft);
                mixSpoonAuto->pauseDrill();
            }
        };
        mixSpoonAuto->mixPaused = [=]
        {
            SoundConfig::getInstance()->stopLoopEffect();
            mixSpoonAuto->pauseDrill();
        };
        auto boudingbox = _bowl->getBowlRect();
        boudingbox.origin = _bowl->convertToWorldSpace(boudingbox.origin);
        
        std::string format = "res/cake_pop/step1_make/mixture%d.png";
        Vec2 range = Vec2(1, 7);
        mixSpoonAuto->setEnableAnimation(false);
        mixSpoonAuto->startMix(boudingbox,
                               mix,
                               format,
                               range.x,
                               range.y);
    };
    ActionHelper::moveInToSceneBounce(mixSpoonAuto, CMVisibleRect::getPosition(110,720), kActionDirectionLeft,call1);
    
    
    auto call2 = [=]
    {
        
        static bool isFirst = true;
        isFirst = true;
        
        static bool s_firstHint = true;
        s_firstHint = true;
        
        static bool s_firstBlend = true;
        s_firstBlend = true;
        
        mixSpoon->mixerTouchBegan = [=](int tag)
        {
            if (s_firstHint)
            {
                s_firstHint = false;
                mixSpoonAuto->removeAllComponents();
                ActionHelper::moveOutofSceneAndRemoveSelf(mixSpoonAuto, kActionDirectionLeft);
            }
            kAudioUtil->stopAllEffect();
            mixSpoon->runAction(ScaleTo::create(0.5f, 0.85f));
            mixSpoon->mixerTouchBegan = nullptr;
        };
        
        mixSpoon->mixPercentCallback = [=](float percent)
        {
            if (isFirst)
            {
                this->initProgressTimer();
                isFirst = false;
                _bowl->setContentVisible("mix", nullptr, false, false);
                TipsUIController::getInstance()->removeAllTips();
                if (_tempItemVec.size()>0)
                {
                    for (auto temp : _tempItemVec)
                    {
                        temp->runAction(Sequence::create(FadeOut::create(0.3f),
                                                         RemoveSelf::create(),
                                                         NULL));
                    }
                    _tempItemVec.clear();
                }
            }
            SoundConfig::getInstance()->playLoopEffect("Liquid mixing.mp3");
            if (percent>random(30, 50))
            {
                if (s_firstBlend)
                {
                    kAudioUtil->playEffect(kVoiceStir1);
                    s_firstBlend = false;
                }
            }
            _progressTimer->setPercentage(percent);
            
            
            if(percent > 99)
            {
                this->removeProgressTimer();
                SoundConfig::getInstance()->stopLoopEffect();
                mixSpoon->mixPercentCallback = nullptr;
                //                _bowl->showHeart(Vec2::ZERO, CC_CALLBACK_0(RainbowMakeScene::showNextStep, this));
                this->showNextStep();
                ActionHelper::moveOutofSceneAndRemoveSelf(mixSpoon, ActionDirection::kActionDirectionLeft);
            }
        };
        mixSpoon->mixPaused = [=]
        {
            SoundConfig::getInstance()->stopLoopEffect();
        };
        auto boudingbox = _bowl->getBowlRect();
        boudingbox.origin = _bowl->convertToWorldSpace(boudingbox.origin);
        
        std::string format = "res/cake_pop/step1_make/mixture%d.png";
        Vec2 range = Vec2(1, 7);
        mixSpoon->setEnableAnimation(false);
        mixSpoon->startMix(boudingbox,
                           mix,
                           format,
                           range.x,
                           range.y);
    };

    ActionHelper::moveInToSceneBounce(mixSpoon, CMVisibleRect::getPosition(521,760), kActionDirectionLeft,call2);
    

}

void RainbowMakeScene::stepMixAbanded()
{
    kAudioUtil->playEffect(kVoiceChooseStirTool);
    auto mix = Sprite::create("res/cake_pop/step1_make/mixture1.png");
    _bowl->addContent(mix, "mix", 15,Vec2(0,50));

    auto spoonAuto = Button::create("res/cake_pop/blender/blender1.png");
    spoonAuto->loadTexturePressed("res/cake_pop/blender/blender1.png");
    this->addToUILayer(spoonAuto);
    auto btm = Sprite::create("res/cake_pop/blender/blende1_1.png");
    btm->setPosition(60, -106);
    spoonAuto->addChild(btm,-1);
    
    spoonAuto->setScale(0.5f);
    {
        auto key = kRewardManager->getItemKey("rainbowcupcake", "tool", 1);
        spoonAuto->setUserObject(__String::create(key));
//        if(!LockUIManager::getInstance()->registerIAPLock(ShopScene::getAllFoodString(), "tool", 1, spoonAuto))
//        {
//            LockUIManager::getInstance()->registerLock("rainbowcupcake", "tool", 1, spoonAuto,Vec2(90,30));
//        }
    }
    ActionHelper::moveInToSceneBounce(spoonAuto, CMVisibleRect::getPosition(170,810), kActionDirectionLeft);

    auto spoon = Button::create("res/cake_pop/blender/blender2.png");
    this->addToUILayer(spoon);
    ActionHelper::moveInToSceneBounce(spoon, CMVisibleRect::getPosition(521,760), kActionDirectionLeft);
    spoon->setScale(0.55);

    spoonAuto->addTouchEventListener([=](Ref*,Widget::TouchEventType type)
    {
        if (type == Widget::TouchEventType::ENDED)
        {
            bool hasPurchased = kIAPManager->isPackagePurchased(ShopScene::getAllFoodString());
            if (!hasPurchased)
            {
                if (kIAPManager->isFree("tool", 1))
                {
                    auto keyObject = spoonAuto->getUserObject();
                    if(keyObject)
                    {
                        string key = ((__String*)(keyObject))->getCString();
                        if(kRewardManager->isLocked(key))
                        {
                            //如果能找到reward名字的lock，显示reward，否则显示iap
                            if (spoonAuto->getChildByName(LOCK_NAME_REWARD))
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

                                auto node = MiniShopScene::create();
                                this->addChild(node,9999);
                                node->setPosition(CMVisibleRect::getPosition(0, 0));
                                node->showAllFoodButton();
                                node->_onRewardButtonClicked = [=]
                                {
                                    kRewardManager->showRewardAds(key);
                                };
                                node->_onShopButtonClicked = [=]
                                {
                                    ShopScene* layer = ShopScene::create();
                                    layer->setPosition(CMVisibleRect::getPosition(0, 0));
                                    this->addChild(layer,9999);
                                };
                            }
                            return;
                        }
                    }
                }
            }
            
            spoonAuto->setVisible(false);
            spoonAuto->setEnabled(false);
            spoon->setTouchEnabled(false);
            ActionHelper::moveOutofSceneAndRemoveSelf(spoon, kActionDirectionRight);
            
            auto mixSpoonAuto = MixSpoonNode::create("res/cake_pop/blender/blender1.png");
            this->addToContentLayer(mixSpoonAuto,3,11);
            mixSpoonAuto->setAutoConfig();
            mixSpoonAuto->setScale(0.55f);
            mixSpoonAuto->setPosition(spoonAuto->getPosition());
            mixSpoonAuto->runAction(Sequence::create(MoveTo::create(0.6f, CMVisibleRect::getPosition(320,520)),
                                                 CallFunc::create([=]
                                                                  {
                                                                      TipsUIController::getInstance()->showMixTip90Degree(_bowl, Vec2(0, 0));
                                                                  }),
                                                 NULL));

            
            static bool isFirst = true;
            isFirst = true;
            static bool s_firstBlend = true;
            s_firstBlend = true;
            
            mixSpoonAuto->mixerTouchBegan = [=](int tag)
            {
                mixSpoonAuto->runAction(ScaleTo::create(0.5f, 0.85f));
                
                mixSpoonAuto->mixerTouchBegan = nullptr;
            };
            
            mixSpoonAuto->mixPercentCallback = [=](float percent)
            {
                if (isFirst)
                {
                    this->initProgressTimer();
                    isFirst = false;
                    _bowl->setContentVisible("mix", nullptr, false, false);
                    TipsUIController::getInstance()->removeAllTips();
                    if (_tempItemVec.size()>0)
                    {
                        for (auto temp : _tempItemVec)
                        {
                            temp->runAction(Sequence::create(FadeOut::create(0.3f),
                                                             RemoveSelf::create(),
                                                             NULL));
                        }
                        _tempItemVec.clear();
                    }
                }
                SoundConfig::getInstance()->playLoopEffect("Liquid mixing.mp3");
                mixSpoonAuto->startDrill();
                if (percent>random(30, 50))
                {
                    if (s_firstBlend)
                    {
                        kAudioUtil->playEffect(kVoiceStir1);
                        s_firstBlend = false;
                    }
                }
                _progressTimer->setPercentage(percent);
                
                if(percent > 99)
                {
                    this->removeProgressTimer();
                    SoundConfig::getInstance()->stopLoopEffect();
                    mixSpoonAuto->mixPercentCallback = nullptr;
                    //                _bowl->showHeart(Vec2::ZERO, CC_CALLBACK_0(RainbowMakeScene::showNextStep, this));
                    this->showNextStep();
                    ActionHelper::moveOutofSceneAndRemoveSelf(mixSpoonAuto, ActionDirection::kActionDirectionLeft);
                    mixSpoonAuto->pauseDrill();
                }
            };
            mixSpoonAuto->mixPaused = [=]
            {
                SoundConfig::getInstance()->stopLoopEffect();
                mixSpoonAuto->pauseDrill();
            };
            auto boudingbox = _bowl->getBowlRect();
            boudingbox.origin = _bowl->convertToWorldSpace(boudingbox.origin);
            
            std::string format = "res/cake_pop/step1_make/mixture%d.png";
            Vec2 range = Vec2(1, 7);
            mixSpoonAuto->setEnableAnimation(false);
            mixSpoonAuto->startMix(boudingbox,
                                   mix,
                                   format,
                                   range.x,
                                   range.y);
        }
    });
    
    spoon->addTouchEventListener([=](Ref*,Widget::TouchEventType type)
    {
        if (type == Widget::TouchEventType::ENDED)
        {
            spoon->setVisible(false);
            spoon->setEnabled(false);
            spoonAuto->setTouchEnabled(false);
            ActionHelper::moveOutofSceneAndRemoveSelf(spoonAuto, kActionDirectionLeft);
            
            auto mixSpoon = MixSpoonNode::create("res/cake_pop/blender/blender2.png");
            this->addToContentLayer(mixSpoon,3,10);
            mixSpoon->setPosition(spoon->getPosition());
            mixSpoon->setScale(0.65f);
            mixSpoon->addlock();
            mixSpoon->runAction(Sequence::create(MoveTo::create(0.6f, CMVisibleRect::getPosition(320,520)),
                                                 CallFunc::create([=]
            {
                TipsUIController::getInstance()->showMixTip90Degree(_bowl, Vec2(0, 0));
            }),
                                                 NULL));

            
            
            static bool isFirst = true;
            isFirst = true;
            
            static bool s_firstHint = true;
            s_firstHint = true;
            
            static bool s_firstBlend = true;
            s_firstBlend = true;
            
            mixSpoon->mixerTouchBegan = [=](int tag)
            {
                if (s_firstHint)
                {
                    s_firstHint = false;
                }
                kAudioUtil->stopAllEffect();
                mixSpoon->runAction(ScaleTo::create(0.5f, 0.85f));
                mixSpoon->mixerTouchBegan = nullptr;
            };
            
            mixSpoon->mixPercentCallback = [=](float percent)
            {
                if (isFirst)
                {
                    this->initProgressTimer();
                    isFirst = false;
                    _bowl->setContentVisible("mix", nullptr, false, false);
                    TipsUIController::getInstance()->removeAllTips();
                    if (_tempItemVec.size()>0)
                    {
                        for (auto temp : _tempItemVec)
                        {
                            temp->runAction(Sequence::create(FadeOut::create(0.3f),
                                                             RemoveSelf::create(),
                                                             NULL));
                        }
                        _tempItemVec.clear();
                    }
                }
                SoundConfig::getInstance()->playLoopEffect("Liquid mixing.mp3");
                if (percent>random(30, 50))
                {
                    if (s_firstBlend)
                    {
                        kAudioUtil->playEffect(kVoiceStir1);
                        s_firstBlend = false;
                    }
                }
                _progressTimer->setPercentage(percent);
                
                
                if(percent > 99)
                {
                    this->removeProgressTimer();
                    SoundConfig::getInstance()->stopLoopEffect();
                    mixSpoon->mixPercentCallback = nullptr;
                    //                _bowl->showHeart(Vec2::ZERO, CC_CALLBACK_0(RainbowMakeScene::showNextStep, this));
                    this->showNextStep();
                    ActionHelper::moveOutofSceneAndRemoveSelf(mixSpoon, ActionDirection::kActionDirectionLeft);
                }
            };
            mixSpoon->mixPaused = [=]
            {
                SoundConfig::getInstance()->stopLoopEffect();
            };
            auto boudingbox = _bowl->getBowlRect();
            boudingbox.origin = _bowl->convertToWorldSpace(boudingbox.origin);
            
            std::string format = "res/cake_pop/step1_make/mixture%d.png";
            Vec2 range = Vec2(1, 7);
            mixSpoon->setEnableAnimation(false);
            mixSpoon->startMix(boudingbox,
                               mix,
                               format,
                               range.x,
                               range.y);
        }
    
    });

    return;

}

void RainbowMakeScene::stepOver()
{
    
    
    if (kESDataManager->getCurrentFood() == ESDataManager::FoodType::kFoodCakepop)
    {
        SceneManager::getInstance()->replaceWithDefaultTransition(RainbowCakePopMakeScene::scene());
    }
    if (kESDataManager->getCurrentFood() == ESDataManager::FoodType::kFoodCupcake)
    {
        SceneManager::getInstance()->replaceWithDefaultTransition(RainbowCupcakeChooseColorScene::scene());
    }

}

void RainbowMakeScene::onEnterTransitionDidFinish()
{
    ESMakeBaseScene::onEnterTransitionDidFinish();

    if (kESDataManager->showAds())
    {
//        AdsManager::getInstance()->showAds(kTypeInterstitialAds);
    }

}



