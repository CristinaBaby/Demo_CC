//
//  PopcornDecScene.cpp
//  ccats1061
//
//  Created by liji on 16/8/23.
//
//

#include "PopcornDecScene.h"
#include "ScribbleSauce.h"
#include "StickerDrawNode.h"
#include "ComponentTouchMove.h"
#include "SceneHeader.h"
#include "Analytics.h"
#define stickerScale 1.3f
PopcornDecScene::PopcornDecScene():
_popcorn(nullptr),
_sprinkleTouch(nullptr)
{
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
    DecorationConfig syrupConfig =
    {
        "syrup",
        "ui/dec/dec/syrup_up.png",
        "ui/dec/dec/syrup_down.png",
        "res/decoration/icon/syrup/syrup%d.png",
        "res/decoration/surup/syrup%d.png",
        1,
        20
    };

    
    _configVec =
    {
        sprinkleConfig,candyConfig,fruitConfig,syrupConfig
    };
    
}

bool PopcornDecScene::init()
{
    if (!DecorationBaseLayer::init())
    {
        return false;
    }
    this->setBackground("res/popcorn/make_bg.png");
    
    this->runAction(Sequence::create(DelayTime::create(4.f),
                                     CallFunc::create([=]
                                                      {
                                                          _firstCup = true;
                                                      }),
                                     NULL));
    
    _module = "popcorn";
    _iapIdentifier = ShopScene::getAllContentString();
    
    _popcorn = PopcornNode::createPopcorn(false);
    this->addChild(_popcorn,10);
    //    _popcorn->showLid();
    ActionHelper::moveInToSceneBounce(_popcorn, CMVisibleRect::getCenterOfScene(), kActionDirectionRight);
    
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
        kAudioUtil->playEffect(kVoiceDecOver);
        _director->getEventDispatcher()->removeAllEventListeners();

        this->runAction(Sequence::create(DelayTime::create(2.1),
                                         CallFunc::create([=]
                                                          {
                                                              _popcorn->savePopcornImage([](RenderTexture *, const std::string &)
                                                                                         {
                                                                                             kESDataManager->setCurrentFood(ESDataManager::FoodType::kFoodPopcorn);
                                                                                             SceneManager::getInstance()->replaceWithDefaultTransition(ChooseServeScene::scene());
                                                                                         });
                                                          }),
                                         NULL));



    };
    
    this->onResetButtonTouchCallback = [=]
    {
        SceneManager::getInstance()->replaceWithDefaultTransition(PopcornDecScene::scene());
    };
    
    return true;
}



void PopcornDecScene::showScrollView(int index)
{
    function<void(ui::Widget * item)> callback;
    
    kAudioUtil->stopAllEffect();
    if (index == 0)
    {
        if (_firstCup)
        {
            kAudioUtil->playEffect(kVoiceDecChooseSprinkle);
        }

        callback = [=](ui::Widget * item)
        {
            if (_firstSticker)
            {
                TipsUIController::getInstance()->showClickTip(this, _popcorn->getPosition()+Vec2(0,225));
                _firstSticker = false;
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
                _popcorn->addSprinkle(contentPath, t->getLocation());
            };
        };
    }
    if (index == 1 || index == 2)
    {
        if (index == 1)
        {
            kAudioUtil->playEffect(kVoiceDecChooseCandy);
        }
        if (index == 2)
        {
            kAudioUtil->playEffect(kVoiceDecChooseFruit);
        }
        
        callback = [=](ui::Widget * item)
        {
            setTypeEnable(false);
            int itemTag = item->getTag();
            
            auto tPizza = kESDataManager->getTexasPizzaFood();
            tPizza.stickerIndex = itemTag;
            kESDataManager->setTexasPizzaFood(tPizza);
            
            Vec2 pos = _popcorn->getPosition()+Vec2(0,225);

            auto changeCall = [=]
            {
                setTypeEnable(true);
                _popcorn->addFood(StringUtils::format(_configVec.at(index).fileFormat.c_str(),itemTag));
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
    
    if (index == 3)
    {
        kAudioUtil->playEffect(kVoiceDecChooseSyrup);
        
        callback = [=](ui::Widget * item)
        {
            setTypeEnable(false);
            int itemTag = item->getTag();
            
            auto tPizza = kESDataManager->getTexasPizzaFood();
            tPizza.stickerIndex = itemTag;
            kESDataManager->setTexasPizzaFood(tPizza);
            Vec2 pos = _popcorn->getPosition()+Vec2(0, 225);
            
            auto changeCall = [=]
            {
                setTypeEnable(true);
                _popcorn->addSyrup(StringUtils::format(_configVec.at(index).fileFormat.c_str(),itemTag));
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
    
    this->setupDecorationWithConfig(_configVec.at(index), callback);
    
}

void PopcornDecScene::onEnter()
{
    DecorationBaseLayer::onEnter();
    //    this->showScrollView(_curTag);
}

void PopcornDecScene::setTypeEnable(bool enable)
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

