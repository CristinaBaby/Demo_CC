//
//  RainbowCupcakeChooseColorScene.cpp
//  shake5008
//
//  Created by liji on 17/1/10.
//
//

#include "RainbowCupcakeChooseColorScene.h"
#include "SceneHeader.h"
#include "MixSpoonNode.h"
#include "ScoopSpoon.h"
#include "ComponentTouchMove.h"
#include "LockUIManager.h"
#include "RewardManager.h"
#include "MiniShopScene.h"
REGIST_SECENE(RainbowCupcakeChooseColorScene)

RainbowCupcakeChooseColorScene::RainbowCupcakeChooseColorScene():
_colorIndex(0),
_bowl(nullptr),
_cup(nullptr),
_bag(nullptr),
_scoopCount(1),
_bagIndex(1),
_cTouch(nullptr),
_inBag(nullptr),
_cakeCup(nullptr),
_mixBag(nullptr),
_mixInCup(nullptr),
_firstTouch(true),
_moveDis(0.f)
{
    _steps.insert(std::make_pair("stepChooseColors", CC_CALLBACK_0(RainbowCupcakeChooseColorScene::stepChooseColors, this)));
    _steps.insert(std::make_pair("stepMixColor", CC_CALLBACK_0(RainbowCupcakeChooseColorScene::stepMixColor, this)));
    _steps.insert(std::make_pair("stepChooseCup", CC_CALLBACK_0(RainbowCupcakeChooseColorScene::stepChooseCup, this)));
    _steps.insert(std::make_pair("stepAddMix", CC_CALLBACK_0(RainbowCupcakeChooseColorScene::stepAddMix, this)));
    
    _stepName =
    {
        "stepChooseColors",
        "stepMixColor",
        "stepChooseCup",
        "stepAddMix",
    };
    

}

RainbowCupcakeChooseColorScene::~RainbowCupcakeChooseColorScene()
{

}

bool RainbowCupcakeChooseColorScene::init()
{
    if (!ESMakeBaseScene::init())
    {
        return false;
    }
    this->setBackground("res/rainbow_slushy/bg-2.png");
    this->showNextStep();
    return true;
}

void RainbowCupcakeChooseColorScene::stepChooseColors()
{
    auto food = kESDataManager->getRainbowcupcakeFood();
    food.colorIndexVec.clear();
    kESDataManager->setRainbowcupcakeFood(food);

    auto bg = Sprite::create("res/rainbow_cupcake/4/cupboard/cupboard.png");
    bg->setPosition(CMVisibleRect::getCenterOfScene());
    this->addToContentLayer(bg);

    kAudioUtil->playEffect("sound/voice over1/Choose the colors.mp3");
    
    auto removeBgCall = [=]
    {
        this->onNextButtonTouchCallback = nullptr;
        this->hideNextButton();
        for (auto node : bg->getChildren())
        {
            _director->getEventDispatcher()->removeEventListenersForTarget(node);
        }
        bg->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.5f, 0.1)),
                                       CallFunc::create([=]
                                                        {
                                                            this->showNextStep();
                                                        }),
                                       RemoveSelf::create(),
                                       NULL));
    };
    
    this->onNextButtonTouchCallback = [=]
    {
        removeBgCall();
    };

    for (int i = 0;i<4; i++)
    {
        for (int j = 0 ; j<3; j++)
        {
            int tag = i+j*4+1;
            auto icon = Button::create(StringUtils::format("res/rainbow_cupcake/4/cupboard/%d.png",i+j*4+1));
            bg->addChild(icon,1,tag);
            icon->setPosition(Vec2(187+i*130,835-30+j*(-240)));
            auto key = kRewardManager->getItemKey("rainbowcupcake", "color", tag);
            icon->setUserObject(__String::create(key));
            if(!LockUIManager::getInstance()->registerIAPLock(ShopScene::getAllFoodString(), "color", tag, icon))
            {
                LockUIManager::getInstance()->registerLock("rainbowcupcake", "color", tag, icon,Vec2(60,30));
            }

            
            icon->addTouchEventListener([=](Ref* obj,Widget::TouchEventType type)
            {
                auto btn = dynamic_cast<Button*>(obj);
                if (type == Widget::TouchEventType::ENDED)
                {
                    bool hasPurchased = kIAPManager->isPackagePurchased(ShopScene::getAllFoodString());
                    if (!hasPurchased)
                    {
                        if (kIAPManager->isFree("color", 1))
                        {
                            auto keyObject = btn->getUserObject();
                            if(keyObject)
                            {
                                string key = ((__String*)(keyObject))->getCString();
                                if(kRewardManager->isLocked(key))
                                {
                                    //如果能找到reward名字的lock，显示reward，否则显示iap
                                    if (btn->getChildByName(LOCK_NAME_REWARD))
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

                    
                    kAudioUtil->playEffect(kSoundClickButton);
                    auto btn = dynamic_cast<Button*>(obj);
                    int tag = btn->getTag();
                    this->showNextButton();
                    btn->setTouchEnabled(false);
                    auto food = kESDataManager->getRainbowcupcakeFood();
                    food.colorIndexVec.push_back(tag);
                    kESDataManager->setRainbowcupcakeFood(food);
                    
                    auto ok = Sprite::create("ui/other/OK.png");
                    btn->addChild(ok);
                    ok->setPosition(61,33);
                    
                    auto p = ParticleSystemQuad::create("particle/chosen.plist");
                    btn->addChild(p);
                    p->setPosition(btn->getContentSize()/2);

                    
                    if (food.colorIndexVec.size() == 3)
                    {
                        removeBgCall();
                    }
                }
            });
        }
    }
}

void RainbowCupcakeChooseColorScene::stepMixColor()
{
    auto food = kESDataManager->getRainbowcupcakeFood();
    int index = food.colorIndexVec.at(_colorIndex);
    _bowl = nullptr;
    _bowl = BowlNode::create("res/rainbow_cupcake/5/bowl_mix/bowl2_down.png", "res/rainbow_cupcake/5/bowl_mix/bowl2_up.png");
    this->addToContentLayer(_bowl);
    auto mix = Sprite::create("res/rainbow_cupcake/5/bowl_mix/mixture.png");
    _bowl->addContent(mix, "mix", 1);
    mix->setOpacity(255);
    
    auto inCall = [=]
    {
        auto bottlePath = StringUtils::format("res/rainbow_cupcake/5/coloring/coloring_bottle/%d.png",index);
        auto inBowlPath = StringUtils::format("res/rainbow_cupcake/5/coloring/bowl_coloring/%d.png",index);
        auto dropPath = StringUtils::format("res/rainbow_cupcake/5/coloring/coloring_drop/%d.png",index);
        
        auto item = BowlNode::create(bottlePath, "");
        this->addToContentLayer(item,3);
        auto temp = Sprite::create(inBowlPath);
        _bowl->addContent(temp, "oil", 3);
        _tempItemVec.push_back(temp);
        
        auto ingredientsInCall = [=]
        {
            TipsUIController::getInstance()->showMoveTipBezel(this, CMVisibleRect::getPosition(320, 720), CMVisibleRect::getPosition(320, 320));
            
            auto pourCall = [=]
            {
                TipsUIController::getInstance()->removeAllTips();
                TipsUIController::getInstance()->showAccelerateTip(this, item->getPosition(),false);
                item->beginPour(-70, [=]
                                {
                                    TipsUIController::getInstance()->removeAllTips();
                                    auto drop = Sprite::create(dropPath);
                                    item->addChild(drop);
                                    drop->setPosition(-18,125);
                                    drop->setRotation(item->getRotation()*-1);
                                    drop->setScale(0.f);
                                    kAudioUtil->playEffect(kSoundAddOilDrop);
                                    drop->runAction(Sequence::create(
                                                                     EaseBackOut::create(ScaleTo::create(0.3f, 1.f)),
                                                                     MoveTo::create(1.3f, Vec2(-260,39)),
                                                                     CallFunc::create([=]
                                                                                      {
                                                                                          _bowl->setContentVisible("oil");
                                                                                      }),
                                                                     FadeOut::create(0.3f),
                                                                     CallFunc::create([=]
                                                                                      {
                                                                                          kAudioUtil->playEffect(kSoundAddOilDrop);
                                                                                      }),
                                                                     FadeIn::create(0.f),
                                                                     Place::create(Vec2(-18,125)),
                                                                     EaseBackOut::create(ScaleTo::create(0.3f, 1.f)),
                                                                     MoveTo::create(1.3f, Vec2(-260,39)),
                                                                     FadeOut::create(0.3f),
                                                                     CallFunc::create([=]
                                                                                      {
                                                                                          auto doneCall = [=]
                                                                                          {
                                                                                              this->mix();
//                                                                                              bowl->showHeart(Vec2::ZERO, CC_CALLBACK_0(PizzaconeMakeScene::showNextStep, this));
                                                                                          };
                                                                                          
                                                                                          ActionHelper::moveOutofSceneAndRemoveSelf(item, kActionDirectionRight,doneCall);
                                                                                      }),
                                                                     NULL));
                                    
                                    
                                }, true);
            };
            
            item->setPourWorldPoint(CMVisibleRect::getPosition(458,591),pourCall);
        };
        
        ActionHelper::moveInToSceneBounce(item, CMVisibleRect::getPosition(320, 770), kActionDirectionRight,ingredientsInCall);
    
    };
    ActionHelper::moveInToSceneBounce(_bowl, CMVisibleRect::getPosition(320, 300), kActionDirectionLeft,inCall);
}

void RainbowCupcakeChooseColorScene::mix()
{
    int cIndex = kESDataManager->getRainbowcupcakeFood().colorIndexVec.at(_colorIndex);
    auto mix = Sprite::create(StringUtils::format("res/rainbow_cupcake/5/bowl_mix/%d_1.png",cIndex));
    _bowl->addContent(mix, "mix1", 1);
    
    auto mixSpoon = MixSpoonNode::create("res/rainbow_cupcake/5/stirrer.png");
    this->addToContentLayer(mixSpoon,3,10);
    mixSpoon->setScale(0.8f);
    mixSpoon->setMixLength(30000);
    
    auto ingredientsInCall1 = [=]
    {

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
                _bowl->setContentVisible("mix1", nullptr, false, false);
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
                //                _bowl->showHeart(Vec2::ZERO, CC_CALLBACK_0(CornDogMakeScene::showNextStep, this));
                ActionHelper::moveOutofSceneAndRemoveSelf(mixSpoon, ActionDirection::kActionDirectionLeft,[=]
                {
                    this->pack();
                });
//                ActionHelper::moveOutofSceneAndRemoveSelf(_bowl, kActionDirectionLeft,[=]
//                                                          {
//                                                              _colorIndex++;
//                                                              int num = kESDataManager->getRainbowcupcakeFood().colorIndexVec.size();
//                                                              log("num %d ====",num);
//                                                              log("_colorIndex %d ====",_colorIndex);
//                                                              if (_colorIndex<num)
//                                                              {
//                                                                  this->stepMixColor();
//                                                              }
//                                                              else
//                                                              {
//                                                                  this->showNextStep();
//                                                              }
//                                                          });
            }
        };
        mixSpoon->mixPaused = [=]
        {
            SoundConfig::getInstance()->stopLoopEffect();
        };
        auto boudingbox = _bowl->getBowlRect();
        boudingbox.origin = _bowl->convertToWorldSpace(boudingbox.origin);
    
        std::string format = StringUtils::format("res/rainbow_cupcake/5/bowl_mix/%d",cIndex)+("_%d.png");
        Vec2 range = Vec2(1, 4);
        mixSpoon->setEnableAnimation(true);
        mixSpoon->startMix(boudingbox,
                           mix,
                           format,
                           range.x,
                           range.y);
    };
    
    ActionHelper::moveInToSceneBounce(mixSpoon, CMVisibleRect::getPosition(521,694), kActionDirectionLeft,ingredientsInCall1);
    
    
    TipsUIController::getInstance()->showMixTip90Degree(_bowl, Vec2(0, 0));
}

void RainbowCupcakeChooseColorScene::pack()
{
    int cIndex = kESDataManager->getRainbowcupcakeFood().colorIndexVec.at(_colorIndex);
    _bowl->runAction(MoveTo::create(0.5f, CMVisibleRect::getPosition(137, 288)));
    
    _cup = BowlNode::create("res/rainbow_cupcake/6/bag/cup_down.png", "res/rainbow_cupcake/6/bag/cup_up.png");
    this->addToContentLayer(_cup);
    
    _bag = BowlNode::create("res/rainbow_cupcake/6/bag/bag_1/bag1_down.png", "res/rainbow_cupcake/6/bag/bag_1/bag1_up.png");
    _cup->addChild(_bag,1);
    _bag->initMaskNode("res/rainbow_cupcake/6/bag/bag_1/bag_mix_shadow.png");
    
    auto inCall = [=]
    {
        auto spoon = ScoopSpoon::create("res/rainbow_cupcake/5/spoon_mix/spoon.png");
        
        spoon->setContent(StringUtils::format("res/rainbow_cupcake/5/spoon_mix/%d.png",cIndex),Vec2::ZERO);
        this->addToContentLayer(spoon, 11);
        spoon->setMoveContentOffset(Vec2(-50.0, -50.0));
        
        auto scoopCall = [=]
        {
            auto dragCall = [=]()
            {
                TipsUIController::getInstance()->showMoveTipBezel(this, spoon->getPosition()+Vec2(0, -100), CMVisibleRect::getPosition(179,342));
                spoon->startDragToWorldPositon(CMVisibleRect::getPosition(179,523));
            };
            
            spoon->onDragCallback = []
            {
                TipsUIController::getInstance()->removeAllTips();
                kAudioUtil->playEffect(kSoundPourSauce);
            };
            spoon->onScroopedCallback = [=]
            {
                TipsUIController::getInstance()->removeAllTips();
                TipsUIController::getInstance()->showMoveTipBezel(this, CMVisibleRect::getPosition(179,523), CMVisibleRect::getPosition(570,575));
                spoon->setBackPosition(CMVisibleRect::getPosition(570,575));
            };
            spoon->onFinishPourCallback = [=]()
            {
                if (_scoopCount == 2)
                {
                    auto temp = Sprite::create(StringUtils::format("res/rainbow_cupcake/6/bag/bag_1/%d_%d.png",cIndex,_scoopCount));
                    _bag->addContentToMask(temp, StringUtils::format("color%d",_scoopCount), 3,Vec2(0, -40));
                    _bag->setContentInMaskVisible(StringUtils::format("color%d",_scoopCount),nullptr,true,true);
                    _tempItemVec.push_back(temp);
                }
                else if (_scoopCount == 3)
                {
                    auto temp = Sprite::create(StringUtils::format("res/rainbow_cupcake/6/bag/bag_1/%d_%d.png",cIndex,2));
                    _bag->addContentToMask(temp, StringUtils::format("color%d",_scoopCount), 8);
                    _bag->setContentInMaskVisible(StringUtils::format("color%d",_scoopCount),nullptr,true,true);
                    _tempItemVec.push_back(temp);
                }
                else
                {
                    auto temp = Sprite::create(StringUtils::format("res/rainbow_cupcake/6/bag/bag_1/%d_%d.png",cIndex,_scoopCount));
                    _bag->addContentToMask(temp, StringUtils::format("color%d",_scoopCount), 2,Vec2(0, -20));
                    _bag->setContentInMaskVisible(StringUtils::format("color%d",_scoopCount),nullptr,true,true);
                    _tempItemVec.push_back(temp);
                }
            };
            spoon->backToOriginalPositionCallback = [=]()
            {
                
//                ActionHelper::moveOutofSceneAndRemoveSelf(item, kActionDirectionLeft,doneCall);
                _scoopCount++;
                if (_scoopCount>=4)
                {
                    _scoopCount = 1;
                    auto doneCall = [=]
                    {
                        //                    _bowl->showHeart(Vec2::ZERO, CC_CALLBACK_0(PizzaconeMakeScene::showNextStep, this));
                        
                        ActionHelper::moveOutofSceneAndRemoveSelf(_bowl, kActionDirectionLeft,[=]
                        {
                            _cup->runAction(Sequence::create(MoveTo::create(0.5f, CMVisibleRect::getCenterOfScene()),
                                                             CallFunc::create([=]
                            {
                                _cTouch = ComponentTouchMove::create();
                                _cTouch->setIsCanMove(false);
                                _bag->addComponent(_cTouch);
                                this->packBag();
                            }),
                                                             NULL));
                        });
                    };

                    ActionHelper::moveOutofSceneAndRemoveSelf(spoon, kActionDirectionRight,doneCall);

                }
                else
                {
                    dragCall();
                }
            };
            dragCall();
        };
        
        ActionHelper::moveInToSceneBounce(spoon, CMVisibleRect::getPosition(370,856), ActionDirection::kActionDirectionLeft,scoopCall);
    };
    
    ActionHelper::moveInToSceneBounce(_cup, CMVisibleRect::getPosition(479, 322), kActionDirectionRight,inCall);
    
}

void RainbowCupcakeChooseColorScene::packBag()
{
    float gDis = 80.f;
    int cIndex = kESDataManager->getRainbowcupcakeFood().colorIndexVec.at(_colorIndex);

    switch (_bagIndex)
    {
        case 1:
        {
            TipsUIController::getInstance()->showMoveTip(this, CMVisibleRect::getPosition(400, 200), CMVisibleRect::getPosition(400, 400));
            for (auto temp : _tempItemVec)
            {
                temp->removeFromParent();
            }
            _tempItemVec.clear();
            _inBag = Sprite::create(StringUtils::format("res/rainbow_cupcake/6/bag/bag_1/%d_2.png",cIndex));
            _bag->addContent(_inBag, "inBag", 2);
            _inBag->setOpacity(255);
            
            _cTouch->touchMovedCall = [=](Touch* t)
            {
                auto deltaY = t->getDelta().y;
                if(deltaY>0)
                {
                    _moveDis += deltaY;
                }
                log("======deltaY %f",deltaY);
                log("======s_d %f",_moveDis);
            };
            _cTouch->touchEndedCall = [=](Touch* t)
            {
                if (_moveDis>gDis)
                {
                    kAudioUtil->playEffect(kSoundPlasticBag);
                    TipsUIController::getInstance()->removeAllTips();
                    _moveDis = 0.f;
                    _bagIndex++;
                    _inBag->setTexture(StringUtils::format("res/rainbow_cupcake/6/bag/bag_2/%d.png",cIndex));
                    _bag->updateBowlConfig("", "res/rainbow_cupcake/6/bag/bag_2/bag2_up.png");
                    packBag();
                }
            };
        }
            break;
        case 2:
        {
            TipsUIController::getInstance()->showMoveTip(this, CMVisibleRect::getPosition(300, 400), CMVisibleRect::getPosition(400, 400));
            _cTouch->touchMovedCall = [=](Touch* t)
            {
                auto deltaY = t->getDelta().x;
                if(deltaY>0)
                {
                    _moveDis += deltaY;
                }
                log("======deltaY %f",deltaY);
                log("======s_d %f",_moveDis);
                
                
            };
            _cTouch->touchEndedCall = [=](Touch* t)
            {
                if (_moveDis>80)
                {
                    kAudioUtil->playEffect(kSoundPlasticBag);
                    TipsUIController::getInstance()->removeAllTips();
                    _moveDis = 0.f;
                    _bagIndex++;
                    _inBag->setTexture(StringUtils::format("res/rainbow_cupcake/6/bag/bag_3/%d.png",cIndex));
                    _bag->updateBowlConfig("", "res/rainbow_cupcake/6/bag/bag_3/bag3_up.png");
                    packBag();
                }
            };
        }
            break;
        case 3:
        {
            _cTouch->touchMovedCall = nullptr;            _cTouch->touchEndedCall = nullptr;
            auto rope = BowlNode::create("res/rainbow_cupcake/6/bag/rubber.png","");
            this->addToContentLayer(rope,2);
            auto inCall = [=]
            {
                TipsUIController::getInstance()->showMoveTipBezel(this, CMVisibleRect::getPosition(508, 668), CMVisibleRect::getPosition(333, 445));
                rope->setPourWorldPoint(CMVisibleRect::getPosition(333, 545), [=]
                                        {
                                            TipsUIController::getInstance()->removeAllTips();
                                            
                                            auto call = [=]
                                            {
                                                rope->setVisible(false);
                                                _bagIndex++;
                                                _inBag->setTexture(StringUtils::format("res/rainbow_cupcake/6/bag/bag_4/%d.png",cIndex));
                                                _bag->updateBowlConfig("", "res/rainbow_cupcake/6/bag/bag_4/bag.png");
                                                this->stepFinishEffect(_cup, [=]
                                                {
                                                    ActionHelper::moveOutofSceneAndRemoveSelf(_cup, kActionDirectionLeft,[=]
                                                                                              {
                                                                                                  _colorIndex++;
                                                                                                  int num = kESDataManager->getRainbowcupcakeFood().colorIndexVec.size();
                                                                                                  log("num %d ====",num);
                                                                                                  log("_colorIndex %d ====",_colorIndex);
                                                                                                  if (_colorIndex<num)
                                                                                                  {
                                                                                                      _bagIndex = 1;
                                                                                                      this->stepMixColor();
                                                                                                  }
                                                                                                  else
                                                                                                  {
                                                                                                      _colorIndex = 0;
                                                                                                      this->showNextStep();
                                                                                                  }
                                                                                              });
                                                }, CMVisibleRect::getCenterOfScene());
                                                
                                            };
                                            
                                            rope->runAction(Sequence::create(
                                                                             ScaleTo::create(0.3f, 1.2f),
                                                                             ScaleTo::create(0.3f, 1.f),
                                                                             CallFunc::create([=]
                                                                                              {
                                                                                                  call();
                                                                                              }), NULL));

                                            
                                        });
            };
            ActionHelper::moveInToSceneBounce(rope, CMVisibleRect::getPosition(508, 668),kActionDirectionRight,inCall);
        }
            break;
        default:
            break;
    }
}

void RainbowCupcakeChooseColorScene::stepChooseCup()
{
    DecorationConfig config = DecorationConfig("rainbowcupcake",
                                               "cup",
                                               "",
                                               "",
                                               "res/rainbow_cupcake/6/cupcake_mold/%d.png",
                                               "res/rainbow_cupcake/6/cupcake_mold/%d.png",
                                               1,
                                               12
                                               );
    
    ScrollViewConfig sConfig = ScrollViewConfig
    (
     Size(getContentSize().width,224),
     "",
     "",
     CMVisibleRect::getPosition(320, 724),
     CMVisibleRect::getPosition(320, 210-300,kBorderNone,kBorderTop),
     191
     );
    
    kAudioUtil->playEffect(kVoiceDecChooseCup);
    
    auto callback = [=](ui::Widget* item)
    {
        _scrollView->setEnabled(false);
        int itemTag = item->getTag();
        log("===item clicked %d====",itemTag);
        auto food = kESDataManager->getRainbowcupcakeFood();
        food.cupIndex = itemTag;
        kESDataManager->setRainbowcupcakeFood(food);
        
        Vec2 pos = CMVisibleRect::getPosition(320, 346);
        
        auto changeCall = [=]
        {
            this->showNextButton();
            
            _scrollView->setEnabled(true);
            
            updateCup();
            //            _corndog->setVisible(true);
            //            _corndog->updateStick();
            this->showParticle(this, CMVisibleRect::getCenterOfScene());
        };
        auto tempBox = Sprite::create(StringUtils::format(config.fileFormat.c_str(),itemTag));
        tempBox->setScale(0.3f);
        auto startPos = _scrollView->getInnerContainer()->convertToWorldSpace(item->getPosition());
        tempBox->setPosition(startPos);
        auto btm = Sprite::create();
        tempBox->addChild(btm,-1);
        btm->setPosition(tempBox->getContentSize()/2);
        
        tempBox->runAction(Sequence::create(Spawn::create(ActionHelper::moveToPostionByBezier(startPos,pos, 0.8),
                                                          EaseBackOut::create(ScaleTo::create(0.8, 1.15)),
                                                          NULL),
                                            CallFunc::create(changeCall),
                                            RemoveSelf::create(),
                                            NULL));
        this->addChild(tempBox,INT_MAX);
        
        
        this->onNextButtonTouchCallback = [=]
        {
            this->hideNextButton();
            this->onNextButtonTouchCallback = nullptr;
            _scrollView->setEnabled(false);
            
            _scrollView->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(800, 0)),
                                                    CallFunc::create([=]
                                                                     {
                                                                         this->showNextStep();
                                                                     }),
                                                    RemoveSelf::create(),
                                                    CallFunc::create([=]
                                                                     {
                                                                         _scrollView = nullptr;
                                                                     }),
                                                    
                                                    NULL));
        };
    };
    
    this->createScrollView(config,callback,sConfig,false);

    auto bg = Sprite::create("res/rainbow_cupcake/6/ovenware_L/ovenware_l.png");
    bg->setAnchorPoint(Vec2(0.5, 0));
    bg->setPosition(CMVisibleRect::getPosition(320, 0,kBorderNone,kBorderBottom));
    this->addToContentLayer(bg,2);

}

void RainbowCupcakeChooseColorScene::updateCup()
{
    if (!_cakeCup)
    {
        _cakeCup = Sprite::create();
        _cakeCup->setAnchorPoint(Vec2(0.5, 0));
        _cakeCup->setPosition(CMVisibleRect::getPosition(320, 0,kBorderNone,kBorderBottom));
        this->addToContentLayer(_cakeCup,3);
    }

    auto cupIndex = kESDataManager->getRainbowcupcakeFood().cupIndex;
    _cakeCup->setTexture(StringUtils::format("res/rainbow_cupcake/6/ovenware_L/%d_0.png",cupIndex));
    
}

void RainbowCupcakeChooseColorScene::stepAddMix()
{
//    _mixBag = nullptr;
//    _mixInCup = nullptr;
    auto index = kESDataManager->getRainbowcupcakeFood().colorIndexVec.at(_colorIndex);
    _mixBag = BowlNode::create(StringUtils::format("res/rainbow_cupcake/6/bag/bag_0/%d.png",index),"");
    this->addToContentLayer(_mixBag,20);
    _mixBag->setScale(0.7f);
    auto inCall = [=]
    {
        _mixBag->setPourWorldPoint(CMVisibleRect::getPosition(404,522), [=]
        {
            TipsUIController::getInstance()->showPressTip(this, _mixBag->getPosition()+Vec2(0, 30), _mixBag->getPosition());
            
            auto touch = ComponentTouchMove::create();
            touch->setName("touch");
            _mixBag->addComponent(touch);
            touch->setIsCanMove(false);
            touch->touchBeganCall = [=](Touch* t)
            {
                TipsUIController::getInstance()->removeAllTips();
                if (_firstTouch)
                {
                    _firstTouch = false;
                    this->startPress();
                }
                else
                {
                    this->resumePress();
                }
                kSoundInstance->playLoopEffect(kSoundPourSauce);
            };
            touch->touchEndedCall = [=](Touch* t)
            {
                this->pausePress();
                kSoundInstance->stopLoopEffect();
            };
        
        });
    };
    
    ActionHelper::moveInToSceneBounce(_mixBag, CMVisibleRect::getPosition(320, 796), kActionDirectionRight,inCall);
    
    
    
}

void RainbowCupcakeChooseColorScene::startPress()
{
    _mixBag->runAction(RepeatForever::create(ActionHelper::createFloatAction()));
    auto index = kESDataManager->getRainbowcupcakeFood().colorIndexVec.at(_colorIndex);
    if (!_mixInCup)
    {
        _mixInCup = Sprite::create(StringUtils::format("res/rainbow_cupcake/6/ovenware_L/%d.png",index));
        _mixInCup->setPosition(CMVisibleRect::getPosition(320, 297,kBorderNone,kBorderBottom));
        this->addToContentLayer(_mixInCup,8);
        _mixInCup->setScale(0.f);
        _mixInCup->runAction(Sequence::create(ScaleTo::create(6.f, 1),
                                              CallFunc::create([=]
        {
            kSoundInstance->stopLoopEffect();
            _colorIndex++;
            int num = kESDataManager->getRainbowcupcakeFood().colorIndexVec.size();
            _mixInCup->stopAllActions();
            this->runAction(Sequence::create(DelayTime::create(0.8f),
                                             CallFunc::create([=]
            {
                log("num %d ====",num);
                log("_colorIndex %d ====",_colorIndex);
                _mixInCup = nullptr;
                _firstTouch = true;
                if (_colorIndex<num)
                {
                    this->stepAddMix();
                }
                else
                {
                    _colorIndex = 0;
                    //                    this->showNextStep();
                    SceneManager::getInstance()->replaceWithDefaultTransition(RainbowCupcakeBakeScene::scene());
                }

            }),
                                             NULL));
            ActionHelper::moveOutofSceneAndRemoveSelf(_mixBag, kActionDirectionRight,[=]
            {
            });
        }),
                                              NULL));
    }

}

void RainbowCupcakeChooseColorScene::resumePress()
{
    _actionManager->resumeTarget(_mixBag);
    _actionManager->resumeTarget(_mixInCup);

}

void RainbowCupcakeChooseColorScene::pausePress()
{
    _actionManager->pauseTarget(_mixBag);
    _actionManager->pauseTarget(_mixInCup);
}


