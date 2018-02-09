//
//  RainbowCakePopMakeScene.cpp
//  shake5008
//
//  Created by liji on 17/1/3.
//
//

#include "RainbowCakePopMakeScene.h"
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
#include "CakepopOvenPlateNode.h"
#include "OvenNode.h"
REGIST_SECENE(RainbowCakePopMakeScene)

RainbowCakePopMakeScene::RainbowCakePopMakeScene():
_bowl(nullptr),
_flow(nullptr),
_oven(nullptr),
_plate(nullptr),
_inBowl(nullptr)
{
    _steps.insert(std::make_pair("stepPourInCup", CC_CALLBACK_0(RainbowCakePopMakeScene::stepPourInCup, this)));
    _steps.insert(std::make_pair("stepChooseColor", CC_CALLBACK_0(RainbowCakePopMakeScene::stepChooseColor, this)));
    _steps.insert(std::make_pair("stepPourInMould", CC_CALLBACK_0(RainbowCakePopMakeScene::stepPourInMould, this)));
    _steps.insert(std::make_pair("stepBake", CC_CALLBACK_0(RainbowCakePopMakeScene::stepBake, this)));
    _steps.insert(std::make_pair("stepFinish", CC_CALLBACK_0(RainbowCakePopMakeScene::stepFinish, this)));

    _stepName =
    {
        "stepPourInCup",
        "stepChooseColor",
        "stepChooseColor",
        "stepChooseColor",
        "stepChooseColor",
        "stepPourInMould",
        "stepBake",
        "stepFinish",
    };
    
    
}

RainbowCakePopMakeScene::~RainbowCakePopMakeScene()
{

}

bool RainbowCakePopMakeScene::init()
{
    if (!ESMakeBaseScene::init())
    {
        return false;
    }
    this->setBackground("res/cake_pop/cake_pop_bg1.png");
    
    vector<Vec2> cupPosVec =
    {
        CMVisibleRect::getPosition(155, 430),
        CMVisibleRect::getPosition(478, 430),
        CMVisibleRect::getPosition(467, 193),
        CMVisibleRect::getPosition(156, 193),
    };
    
    for (int index = 0; index<4; index++)
    {
        auto cup = BowlNode::create(StringUtils::format("res/cake_pop/step2_coloring/cup%d_1.png",index+1),StringUtils::format("res/cake_pop/step2_coloring/cup%d 2.png",index+1));
        this->addToContentLayer(cup,5);
        cup->setPosition(cupPosVec.at(index));
        cup->initMaskNode("res/cake_pop/step2_coloring/cup_shadow.png",Vec2(35, 56));
        _cupVec.pushBack(cup);
    }

    this->showNextStep();
    
    return true;
}

void RainbowCakePopMakeScene::stepPourInCup()
{

    if (!_bowl)
    {
        _bowl = BowlNode::create("res/cake_pop/step1_make/bowl.png", "res/cake_pop/step1_make/bowl1_1.png");
        this->addToContentLayer(_bowl,99);
        _bowl->setScale(0.7f);
        _bowl->initMaskNode("res/cake_pop/step1_make/bowl_shadow.png");
        
    }
    _inBowl = Sprite::create("res/cake_pop/step1_make/mixture7.png");
    _bowl->addContentToMask(_inBowl, "inbowl", 2,Vec2(0, 50));
    
    auto bowlInCall = [=]
    {
        kAudioUtil->playEffect(kVoicePour);
        
        _bowl->setPourWorldPoint(CMVisibleRect::getPosition(291, 682), [=]
        {
            _bowl->runAction(RotateTo::create(0.6f, -50));
            this->startPour();
            
        });
    };
    
    ActionHelper::moveInToSceneBounce(_bowl, CMVisibleRect::getPosition(320, 762), kActionDirectionLeft,bowlInCall);
}

void RainbowCakePopMakeScene::startPour()
{
    _flow = Sprite::create("res/cake_pop/step2_coloring/mixture2.png");
    _bowl->addChild(_flow,99);
    _flow->setRotation(50);
    _flow->setPosition(-260,0);
    _flow->runAction(RepeatForever::create(ActionHelper::createAnimate("res/cake_pop/step2_coloring/mixture%d.png", 2, 3)));
    _flow->setVisible(false);
    this->getNextPos();
}

void RainbowCakePopMakeScene::getNextPos()
{
    _flow->setVisible(false);
    kAudioUtil->stopEffectBySoundId(_waterIdd);
    static int index = -1;
    index++;
    if (index>=4)
    {
        index = -1;
        kAudioUtil->stopEffectBySoundId(_waterIdd);
        ActionHelper::moveOutofSceneAndRemoveSelf(_bowl, kActionDirectionLeft,CC_CALLBACK_0(RainbowCakePopMakeScene::showNextStep, this));
        return ;
    }
    vector<Vec2> cupPosVec =
    {
        CMVisibleRect::getPosition(155, 430),
        CMVisibleRect::getPosition(478, 430),
        CMVisibleRect::getPosition(467, 193),
        CMVisibleRect::getPosition(156, 193),
    };
    Vec2 pos = cupPosVec.at(index) + Vec2(136,230);

    _inBowl->runAction(Sequence::create(DelayTime::create(0.8f),
                                        MoveBy::create(3.f, Vec2(0, -18)),
                                        NULL));

    auto floatAction = ActionHelper::createFloatAction(0.2);
    _bowl->runAction(Sequence::create(MoveTo::create(0.8f, pos),
                                        CallFunc::create([=]
                                                         {
                                                             _waterIdd = kAudioUtil->playEffect(kSoundPourSauce,true);
                                                             this->fillInCup(index);
                                                         }),
                                        Repeat::create(floatAction, 5),
                                        CallFunc::create([=]
                                                         {
                                                             this->getNextPos();
                                                         }),
                                        NULL));
}

void RainbowCakePopMakeScene::fillInCup(int index)
{
    _flow->setVisible(true);
    auto inCup = Sprite::create("res/cake_pop/step2_coloring/cup_mixture.png");

    _cupVec.at(index)->addContentToMask(inCup, StringUtils::format("incup%d",index), 2,Vec2(0, -25));
    _cupVec.at(index)->showContentInMask(StringUtils::format("incup%d",index),3.f);
}

void RainbowCakePopMakeScene::stepChooseColor()
{
    DecorationConfig config = DecorationConfig("rainbowcakepop",
                                               "color",
                                               "",
                                               "",
                                               "res/cake_pop/step2_coloring/coloring_bottle/icon_coloring_bottle%d.png",
                                               "",
                                               1,
                                               12
                                               );
    
    ScrollViewConfig sConfig = ScrollViewConfig
    (
     Size(getContentSize().width,224),
     "",
     "",
     CMVisibleRect::getPosition(320-800, 735),
     CMVisibleRect::getPosition(320, 210-300,kBorderNone,kBorderTop),
     142+20
     );
    if (_firstChooseColor)
    {
        kAudioUtil->playEffect("sound/voice over1/Then choose your favorite colors.mp3");
        _firstChooseColor = false;
    }
    
    auto callback = [=](ui::Widget* item)
    {
        //            setTypeEnable(false);
        int itemTag = item->getTag();
        log("===item clicked %d====",itemTag);
        auto food = kESDataManager->getRainbowcakepopFood();
        food.colorIndexVec.at(_cupIndex) = item->getTag();
        item->setVisible(false);
        kESDataManager->setRainbowcakepopFood(food);
        this->colorChoosed(itemTag);
        _scrollView->setEnabled(false);
        _scrollView->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(800, 0)),
                                                CallFunc::create([=]
                                                                 {
                                                                     this->removeScrollView(false);
                                                                 }),
                                                NULL));

    };
    
    this->createScrollView(config,callback,sConfig,false);
    
    _cupVec.at(_cupIndex)->runAction(Sequence::create(
                                                      DelayTime::create(0.4f),
                                                      MoveTo::create(1.2f, CMVisibleRect::getPosition(320, 245)),
                                                CallFunc::create([=]
    {
        _scrollView->runAction(MoveBy::create(0.5f, Vec2(800, 0)));
    }),
                                                NULL));
    if(_cupIndex == 0)
    {
        for (int i = 1; i<_cupVec.size();i++)
        {
            auto cup = _cupVec.at(i);
            if (i == _cupVec.size()-1)
            {
                cup->runAction(MoveBy::create(0.6f, Vec2(-400, 0)));
            }
            else
            {
                cup->runAction(MoveBy::create(0.6f, Vec2(400, 0)));
            }
        }
    }
    
}

void RainbowCakePopMakeScene::colorChoosed(int cIndex)
{
    auto food = kESDataManager->getRainbowcakepopFood();
    food.colorIndexVec.at(_cupIndex) = cIndex;
    kESDataManager->setRainbowcakepopFood(food);
    
    auto item = BowlNode::create(StringUtils::format("res/cake_pop/step2_coloring/coloring_bottle/coloring_bottle%d.png",cIndex), "");
    this->addToContentLayer(item,13);
    auto temp = Sprite::create(StringUtils::format("res/cake_pop/step2_coloring/bowl_coloring/bowl_coloring%d.png",cIndex));
    auto cCup = _cupVec.at(_cupIndex);
    cCup->addContentToMask(temp, "oil", 3);
    temp->setOpacity(0);
    _tempItemVec.push_back(temp);
    
    auto ingredientsInCall = [=]
    {
        TipsUIController::getInstance()->showMoveTipBezel(this, CMVisibleRect::getPosition(534, 590), CMVisibleRect::getPosition(320, 265));
        
        auto pourCall = [=]
        {
            TipsUIController::getInstance()->removeAllTips();
            TipsUIController::getInstance()->showAccelerateTip(this, item->getPosition(),false);
            item->beginPour(-70, [=]
                            {
                                TipsUIController::getInstance()->removeAllTips();
                                auto drop = Sprite::create(StringUtils::format("res/cake_pop/step2_coloring/coloring/coloring%d.png",cIndex));
                                item->addChild(drop);
                                drop->setPosition(-8,95);
                                drop->setRotation(item->getRotation()*-1);
                                drop->setScale(0.f);
                                kAudioUtil->playEffect(kSoundAddOilDrop);
                                drop->runAction(Sequence::create(
                                                                 EaseBackOut::create(ScaleTo::create(0.3f, 1.f)),
                                                                 MoveTo::create(1.6f, Vec2(-180,39)),
                                                                 CallFunc::create([=]
                                                                                  {
                                                                                      cCup->setContentInMaskVisible("oil");
                                                                                  }),
                                                                 FadeOut::create(0.4f),
                                                                 CallFunc::create([=]
                                                                                  {
                                                                                      kAudioUtil->playEffect(kSoundAddOilDrop);
                                                                                  }),
                                                                 FadeIn::create(0.f),
                                                                 Place::create(Vec2(-8,95)),
                                                                 EaseBackOut::create(ScaleTo::create(0.3f, 1.f)),
                                                                 MoveTo::create(1.6f, Vec2(-180,39)),
                                                                 FadeOut::create(0.4f),
                                                                 CallFunc::create([=]
                                                                                  {
                                                                                      auto doneCall = [=]
                                                                                      {
//                                                                                          cCup->showHeart(Vec2::ZERO, CC_CALLBACK_0(PizzaconeMakeScene::showNextStep, this));
                                                                                          showMixCup(cIndex);
                                                                                      };
                                                                                      
                                                                                      ActionHelper::moveOutofSceneAndRemoveSelf(item, kActionDirectionRight,doneCall);
                                                                                  }),
                                                                 NULL));
                                
                                
                            }, true);
        };
        
        item->setPourWorldPoint(CMVisibleRect::getPosition(461,484),pourCall);
    };
    
    ActionHelper::moveInToSceneBounce(item, CMVisibleRect::getPosition(534, 490), kActionDirectionRight,ingredientsInCall);
}

void RainbowCakePopMakeScene::showMixCup(int cIndex)
{
    
    auto cCup = _cupVec.at(_cupIndex);
    auto mix = Sprite::create(StringUtils::format("res/cake_pop/step2_coloring/color_mixture/mixture%d_1.png",cIndex));
    cCup->addContentToMask(mix, "mix", 10,Vec2(0,-20));
    mix->setOpacity(0);
    
    auto mixSpoon = MixSpoonNode::create("res/cake_pop/step2_coloring/b.png");
    cCup->addChild(mixSpoon,9999,20);
    mixSpoon->setScale(0.7f);
    mixSpoon->setRotation(10);
    mixSpoon->setMixLength(30000);

    auto ingredientsInCall1 = [=]
    {

        mixSpoon->setZOrder(11);
        static bool isFirst = true;
        isFirst = true;
        static bool s_firstBlend = true;
        s_firstBlend = true;

        mixSpoon->mixerTouchBegan = [=](int tag)
        {            
            mixSpoon->mixerTouchBegan = nullptr;
        };
        
        mixSpoon->mixPercentCallback = [=](float percent)
        {
            if (isFirst)
            {
                this->initProgressTimer();

                isFirst = false;
                auto node = cCup->getMaskNode()->getChildByName(StringUtils::format("incup%d",_cupIndex));
                if (node)
                {
                    node->setVisible(false);
                }
                cCup->setContentInMaskVisible("mix", nullptr, false, false);
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
            _progressTimer->setPercentage(percent);

            SoundConfig::getInstance()->playLoopEffect("Liquid mixing.mp3");
            if (percent>random(30, 50))
            {
                if (s_firstBlend)
                {
                    kAudioUtil->playEffect(kVoiceStir1);
                    s_firstBlend = false;
                }
            }

            if(percent > 99)
            {
                this->removeProgressTimer();
                mixSpoon->getCTouch()->setEnabled(false);
                mixSpoon->setVisible(false);
                this->stepFinishEffect(cCup, [=]
                                       {
                                           mixSpoon->removeFromParent();
                                           cCup->runAction(Sequence::create(
                                                                            MoveBy::create(0.4f, Vec2(800, 0)),
                                                                            CallFunc::create(CC_CALLBACK_0(RainbowCakePopMakeScene::showNextStep, this)),
                                                                            NULL));
                                       }, CMVisibleRect::getPosition(320,400));

                SoundConfig::getInstance()->stopLoopEffect();
                mixSpoon->mixPercentCallback = nullptr;
                _cupIndex++;
            }
        };
        mixSpoon->mixPaused = [=]
        {
            SoundConfig::getInstance()->stopLoopEffect();
        };
        auto boudingbox = cCup->getBowlRect();
        boudingbox.origin = cCup->convertToWorldSpace(boudingbox.origin);
        boudingbox.size = boudingbox.size + Size(0,80);
        
        std::string format = StringUtils::format("res/cake_pop/step2_coloring/color_mixture/mixture%d",cIndex) + "_%d.png";
        Vec2 range = Vec2(1, 4);
        mixSpoon->setEnableAnimation(true);
        mixSpoon->startMix(boudingbox,
                           mix,
                           format,
                           range.x,
                           range.y);
        Rect r;
        r.origin = CMVisibleRect::getPosition(322, 295);
        r.size = Size(90,66);
        mixSpoon->setMixMoveRect(r);
    };
    
    ActionHelper::moveInToSceneBounce(mixSpoon, Vec2(10,90), kActionDirectionLeft,ingredientsInCall1);
}

void RainbowCakePopMakeScene::stepPourInMould()
{
    _cupIndex = 0;
    _plate = CakepopOvenPlateNode::create();
    this->addToContentLayer(_plate,10);
    
    for (int i = 0; i<_cupVec.size(); i++)
    {
        _cupVec.at(i)->setPosition(-500, -500);
    }
    
    auto inCall = [=]
    {
        kAudioUtil->playEffect(kVoicePour2);

        this->pourIntoMould();
    };
    
    ActionHelper::moveInToSceneBounce(_plate, CMVisibleRect::getPosition(320,220), kActionDirectionLeft,inCall);
}

void RainbowCakePopMakeScene::pourIntoMould()
{
    auto cCup = _cupVec.at(_cupIndex);
    cCup->setZOrder(20);
    ActionHelper::moveInToSceneBounce(cCup, CMVisibleRect::getPosition(320, 746), kActionDirectionLeft);
    vector<Vec2> posVec =
    {
        CMVisibleRect::getPosition(50, 558-10),
        CMVisibleRect::getPosition(333, 538-10),
        CMVisibleRect::getPosition(47, 462),
        CMVisibleRect::getPosition(334, 445),
    
    };
    
    int colorIndex = kESDataManager->getRainbowcakepopFood().colorIndexVec.at(_cupIndex);
    
    cCup->setPourWorldPoint(posVec.at(_cupIndex), [=]
                            {
                                cCup->beginPour(50, [=]
                                                {
                                                    _plate->showMixtureInPlate(_cupIndex, colorIndex);
                                                    
                                                    auto flow = Sprite::create(StringUtils::format("res/cake_pop/step3_oven/mixture/mixture%d_1.png",colorIndex));
                                                    cCup->addChild(flow,99);
                                                    flow->setPosition(219, 23);
                                                    flow->setRotation(cCup->getRotation()*-1);
                                                    flow->runAction(Sequence::create(Repeat::create(ActionHelper::createAnimate(StringUtils::format("res/cake_pop/step3_oven/mixture/mixture%d",colorIndex)+"_%d.png", 1, 2), 15),
                                                                                     Hide::create(),
                                                                                     CallFunc::create([=]
                                                    {
                                                        ActionHelper::moveOutofSceneAndRemoveSelf(cCup, kActionDirectionLeft,[=]
                                                        {
                                                            _cupIndex++;
                                                            if(_cupIndex==4)
                                                            {
                                                                this->showNextStep();
                                                            }
                                                            else
                                                            {
                                                                pourIntoMould();
                                                            }
                                                        });
                                                    }),
                                                                                     NULL));
//                                                    _plate->showAllMixture();
                                                    flow->setFlippedX(true);
                                                
                                                }, false);
                                
                            });

}

void RainbowCakePopMakeScene::stepBake()
{
    for (int i = 0; i<_cupVec.size(); i++)
    {
        _cupVec.at(i)->setPosition(-500, -500);
    }
    
    if (!_plate)
    {
        _plate = CakepopOvenPlateNode::create();
        this->addToContentLayer(_plate,10);
        _plate->setPosition(CMVisibleRect::getPosition(320, 220));
    }

    _oven = OvenNode::create();
    this->addToContentLayer(_oven,5);
    _oven->setScale(0.9f);
    
    auto ovenInCall = [=]
    {
        _plate->retain();
        _plate->removeFromParent();
        _oven->addChild(_plate);
        _plate->showAllMixture();
        _plate->setPosition(_oven->convertToNodeSpace(_plate->getPosition()));
        kAudioUtil->playEffect(kVoiceBake);
        _plate->runAction(Sequence::create(DelayTime::create(1.2),
                                           Spawn::create(EaseBackIn::create(ScaleTo::create(1.2f, 0.5f)),
                                                         MoveTo::create(1.2f, Vec2(-36, -65)),
                                                         NULL),
                                           CallFunc::create([=]
        {
            _oven->showOvenDoor(true);
            _oven->showButtonTurnNode([=]{bake();});
        }),
                                           NULL));
    };
    
    ActionHelper::moveInToSceneBounce(_oven, CMVisibleRect::getPosition(320,540), kActionDirectionLeft,ovenInCall);

    
}

void RainbowCakePopMakeScene::bake()
{
    _oven->runAction(MoveTo::create(0.5f, CMVisibleRect::getCenterOfScene()));
    
    float interval = 4.1f*4;
    
    int idd = kAudioUtil->playEffect(kSoundWaving,true);
    _oven->rotateButton(interval);
    
    _plate->bakeMixture(interval);
    this->runAction(Sequence::create(DelayTime::create(interval),
                                     CallFunc::create([=]
                                                    {
                                                        kAudioUtil->playEffect(kSoundOvenDone);
                                                        kAudioUtil->stopEffectBySoundId(idd);
                                                        this->stepFinishEffect(_oven, [=]
                                                                               {
                                                                                   ActionHelper::moveOutofSceneAndRemoveSelf(_oven, kActionDirectionRight,CC_CALLBACK_0(RainbowCakePopMakeScene::showNextStep, this));
                                                                               }, CMVisibleRect::getCenterOfScene());
                                                        //        ;
                                                        
                                                    }),
                                           NULL));
}

void RainbowCakePopMakeScene::stepFinish()
{
    SceneManager::getInstance()->replaceWithDefaultTransition(RainbowCakePopPressScene::scene());

}


