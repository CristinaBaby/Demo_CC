//
//  SodaDecScene.cpp
//  ccats1061
//
//  Created by liji on 16/8/23.
//
//

#include "SodaDecScene.h"
#include "ScribbleSauce.h"
#include "StickerDrawNode.h"
#include "ComponentTouchMove.h"
#include "SceneHeader.h"
#include "Analytics.h"
#define stickerScale 1.3f
SodaDecScene::SodaDecScene():
_juice(nullptr),
_sprinkleTouch(nullptr)
{
    DecorationConfig creamConfig =
    {
        "cream",
        "ui/dec/dec/cream_up.png",
        "ui/dec/dec/cream_down.png",
        "res/decoration/icon/cup_cream/cup_cream_%d.png",
        "res/decoration/cup_cream/cup_cream_%d.png",
        1,
        20
    };
    
    DecorationConfig sprinkleConfig =
    {
        "sprinkle",
        "ui/dec/dec/sprinkle_up.png",
        "ui/dec/dec/sprinkle_down.png",
        "res/decoration/icon/sprinkle/sprinkle_%d.png",
        "res/decoration/sprinkle/sprinkle_%d.png",
        1,
        20
    };
    
    DecorationConfig candyConfig =
    {
        "candy",
        "ui/dec/dec/candy_up.png",
        "ui/dec/dec/candy_down.png",
        "res/decoration/icon/candy/candy_%d.png",
        "res/decoration/candy/candy_%d.png",
        1,
        20
    };
    DecorationConfig fruitConfig =
    {
        "fruit",
        "ui/dec/dec/fruit_up.png",
        "ui/dec/dec/fruit_down.png",
        "res/decoration/icon/fruit/fruit_%d.png",
        "res/decoration/fruit/fruit_%d.png",
        1,
        20
    };
    DecorationConfig cupLidConfig =
    {
        "cupLid",
        "ui/dec/dec/lid_up.png",
        "ui/dec/dec/lid_down.png",
        "res/decoration/icon/icon_cupCover/icon_cup%d_top.png",
        "res/soda/cup/cup%d_top1.png",
        1,
        15
    };
    DecorationConfig strawConfig =
    {
        "straw",
        "ui/dec/dec/straw_up.png",
        "ui/dec/dec/straw_down.png",
        "res/decoration/icon/straw/straw%d.png",
        "res/decoration/straw/straw%d.png",
        1,
        20
    };

    
    _configVec =
    {
        creamConfig,sprinkleConfig,candyConfig,fruitConfig,cupLidConfig,strawConfig
    };
    
}

bool SodaDecScene::init()
{
    if (!DecorationBaseLayer::init())
    {
        return false;
    }
    this->setBackground("res/soda/soda_bg.png");
    
    this->runAction(Sequence::create(DelayTime::create(4.f),
                                     CallFunc::create([=]
                                                      {
                                                          _firstCup = true;
                                                      }),
                                     NULL));
    
    _module = "soda";
    _iapIdentifier = ShopScene::getAllContentString();
    
    _juice = JuiceBottleNode::create();
    this->addChild(_juice,10);
//    _juice->showLid();
    ActionHelper::moveInToSceneBounce(_juice, CMVisibleRect::getCenterOfScene(), kActionDirectionRight);
    
    this->initTypeButton();
    this->_typeBtnClicked = [=](Widget* btn)
    {
        this->showScrollView(btn->getTag());

    };
    this->showScrollView(0);
    kAudioUtil->playEffect(kVoiceJuiceDec);
    this->runAction(Sequence::create(DelayTime::create(0.5f),
                                     CallFunc::create([=]
                                                      {
                                                          _firstCup = true;
                                                      }),
                                     NULL));

    this->showNextButton();
    this->onNextButtonTouchCallback = [=]
    {
        kAudioUtil->stopAllEffect();
        _director->getEventDispatcher()->removeAllEventListeners();
        kAudioUtil->playEffect(kVoiceDecOver);
        this->runAction(Sequence::create(DelayTime::create(2.1),
                                         CallFunc::create([=]
                                                          {
                                                              _juice->saveJuiceImage([](RenderTexture *, const std::string &)
                                                                                     {
                                                                                         kESDataManager->setCurrentFood(ESDataManager::FoodType::kFoodSoda);
                                                                                         SceneManager::getInstance()->replaceWithDefaultTransition(ChooseServeScene::scene());
                                                                                     });
                                                          }),
                                         NULL));

    };
    
    this->onResetButtonTouchCallback = [=]
    {
        kESDataManager->resetSodaShare();
        SceneManager::getInstance()->replaceWithDefaultTransition(SodaDecScene::scene());
    };
    
    return true;
}



void SodaDecScene::showScrollView(int index)
{
    function<void(ui::Widget * item)> callback;
    
    kAudioUtil->stopAllEffect();
    if (index == 0)//cream
    {
        if (_firstCup)
            kAudioUtil->playEffect(kVoiceDecChooseCream);
        
        
        callback = [=](ui::Widget * item)
        {
            setTypeEnable(false);
            int itemTag = item->getTag();
            
            auto juice = kESDataManager->getSodaFood();
            juice.creamIndex = itemTag;
            kESDataManager->setSodaFood(juice);
            
            Vec2 pos = _juice->getPosition()+Vec2(0,80);
            
            auto changeCall = [=]
            {
                setTypeEnable(true);
                _juice->updateCream(juice.creamIndex);
                this->showDecorationParticle(pos);
            };
            auto tempBox = Sprite::create(StringUtils::format(_configVec.at(index).fileFormat.c_str(),itemTag));
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
        };
    }
    if (index == 1)
    {
        kAudioUtil->playEffect(kVoiceDecChooseSprinkle);

        callback = [=](ui::Widget * item)
        {
            if (_firstSprinkle)
            {
                _firstSprinkle = false;
                TipsUIController::getInstance()->showClickTip(this, CMVisibleRect::getCenterOfScene());

            }

            
            //            setTypeEnable(false);
            int itemTag = item->getTag();
            if (!_sprinkleTouch)
            {
                _sprinkleTouch = ComponentTouchMove::create();
                this->addComponent(_sprinkleTouch);
                _sprinkleTouch->setIsCanMove(false);
            }
            else
            {
                _sprinkleTouch->resume();
                _sprinkleTouch->touchEndedCall = nullptr;
            }
            _sprinkleTouch->touchEndedCall = [=](Touch* t)
            {
                TipsUIController::getInstance()->removeAllTips();
                auto contentPath = StringUtils::format(_configVec.at(index).fileFormat.c_str(),itemTag);
                _juice->addSprinkle(contentPath, t->getLocation());
            };
        };
    }
    if (index == 5)
    {
        
        kAudioUtil->playEffect(kVoiceDecChooseStraw);
        callback = [=](ui::Widget * item)
        {
            setTypeEnable(false);
            int itemTag = item->getTag();
            
            auto juice = kESDataManager->getSodaFood();
            juice.strawIndex = itemTag;
            kESDataManager->setSodaFood(juice);
            
            auto changeCall = [=]
            {
                setTypeEnable(true);
                _juice->updateStraw(juice.strawIndex);
                this->showDecorationParticle(_juice->getPosition());
            };
            auto tempBox = Sprite::create(StringUtils::format(_configVec.at(index).fileFormat.c_str(),itemTag));
            tempBox->setScale(0.3f);
            auto startPos = _scrollView->getInnerContainer()->convertToWorldSpace(item->getPosition());
            tempBox->setPosition(startPos);
            tempBox->runAction(Sequence::create(Spawn::create(ActionHelper::moveToPostionByBezier(startPos,_juice->getPosition()+Vec2(42,71+30), 0.8),
                                                              EaseBackOut::create(ScaleTo::create(0.8, 1.15)),
                                                              NULL),
                                                CallFunc::create(changeCall),
                                                RemoveSelf::create(),
                                                NULL));
            this->addChild(tempBox,INT_MAX);
        };
    }
    
    if (index == 3 || index == 2)
    {
        if (index == 3)//fruit
        {
            kAudioUtil->playEffect(kVoiceDecChooseFruit);
        }
        if (index == 2)
        {
            kAudioUtil->playEffect(kVoiceDecChooseCandy);
        }

        callback = [=](ui::Widget * item)
        {
            int itemTag = item->getTag();
            
            auto changeCall = [=]
            {
                setTypeEnable(true);
                _juice->addFood(StringUtils::format(_configVec.at(index).fileFormat.c_str(),itemTag));
                this->showDecorationParticle(_juice->getPosition()+Vec2(0,150));
            };
            auto tempBox = Sprite::create(StringUtils::format(_configVec.at(index).fileFormat.c_str(),itemTag));
            tempBox->setScale(0.3f);
            auto startPos = _scrollView->getInnerContainer()->convertToWorldSpace(item->getPosition());
            tempBox->setPosition(startPos);
            auto btm = Sprite::create();
            tempBox->addChild(btm,-1);
            btm->setPosition(tempBox->getContentSize()/2);
            
            tempBox->runAction(Sequence::create(Spawn::create(ActionHelper::moveToPostionByBezier(startPos,_juice->getPosition()+Vec2(0,150), 0.8),
                                                              EaseBackOut::create(ScaleTo::create(0.8, 1.15)),
                                                              NULL),
                                                CallFunc::create(changeCall),
                                                RemoveSelf::create(),
                                                NULL));
            this->addChild(tempBox,INT_MAX);

        };
    }
    
    if (index == 4) //lid
    {
        kAudioUtil->playEffect(kVoiceDecChooseCupLid);

        callback = [=](ui::Widget * item)
        {
            setTypeEnable(false);
            int itemTag = item->getTag();
            
            auto juice = kESDataManager->getSodaFood();
            juice.lidIndex = itemTag;
            kESDataManager->setSodaFood(juice);
            Vec2 pos = _juice->getPosition()+Vec2(0,80);

            auto changeCall = [=]
            {
                setTypeEnable(true);
                _juice->updateBottleLid(juice.lidIndex);
                this->showDecorationParticle(pos);
            };
            auto tempBox = Sprite::create(StringUtils::format(_configVec.at(index).fileFormat.c_str(),itemTag));
            tempBox->setScale(0.3f);
            auto startPos = _scrollView->getInnerContainer()->convertToWorldSpace(item->getPosition());
            tempBox->setPosition(startPos);
            auto btm = Sprite::create("res/soda/cup/cup1_top2.png");
            tempBox->addChild(btm,-1);
            btm->setPosition(tempBox->getContentSize()/2);
            tempBox->runAction(Sequence::create(Spawn::create(ActionHelper::moveToPostionByBezier(startPos,_juice->getPosition(), 0.8),
                                                              EaseBackOut::create(ScaleTo::create(0.8, 1.15)),
                                                              NULL),
                                                CallFunc::create(changeCall),
                                                RemoveSelf::create(),
                                                NULL));
            this->addChild(tempBox,INT_MAX);
        };
        
    }
    
    
    this->setupDecorationWithConfig(_configVec.at(index), callback);
    
}

void SodaDecScene::onEnter()
{
    DecorationBaseLayer::onEnter();
}

void SodaDecScene::setTypeEnable(bool enable)
{
    for (auto btn : _typeBtnVec)
    {
        btn->setTouchEnabled(enable);
    }
    
    _scrollView->setEnabled(enable);
    _nextButton->setTouchEnabled(enable);
    _resetButton->setTouchEnabled(enable);
    
    this->ignorAllTouch(!enable);
}

