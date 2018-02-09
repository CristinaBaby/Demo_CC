//
//  RainbowCakepopDecScene.cpp
//  shake5008
//
//  Created by liji on 17/1/12.
//
//

#include "RainbowCakepopDecScene.h"
#include "ScribbleSauce.h"
#include "StickerDrawNode.h"
#include "ComponentTouchMove.h"
#include "SceneHeader.h"
#include "ESDataManager.h"
#include "Analytics.h"

REGIST_SECENE(RainbowCakepopDecScene)

RainbowCakepopDecScene::RainbowCakepopDecScene():
_currentSprinkle(nullptr),
_food(nullptr)
//_foodLayer(nullptr),
//_plate(nullptr)
{
    DecorationDecConfig candyConfig =
    {
        "candy",
        "ui/dec/candy_up.png",
        "ui/dec/candy.png",
        "res/cake_pop/icon_decoration/icon_candies/icon_candies%d.png",
        "res/cake_pop/decoration/candices/candies%d.png",
        1,
        15
    };
    
    DecorationDecConfig sprinkleConfig =
    {
        "sprinkle",
        "ui/dec/sprinkle_up.png",
        "ui/dec/sprinkle.png",
        "res/cake_pop/icon_decoration/icon_sprinkles/icon_sprinkles%d.png",
        "res/cake_pop/decoration/sprinkles/sprinkles%d.png",
        1,
        15
    };
    
    DecorationDecConfig fruitConfig =
    {
        "fruit",
        "ui/dec/fruit_up.png",
        "ui/dec/fruit.png",
        "res/cake_pop/icon_decoration/icon_fruits/icon_fruits%d.png",
        "res/cake_pop/decoration/fruits/fruits%d.png",
        1,
        15
    };
    DecorationDecConfig faceConfig =
    {
        "face",
        "ui/dec/other_up.png",
        "ui/dec/other.png",
        "res/cake_pop/icon_decoration/icon_others/icon_others%d.png",
        "res/cake_pop/decoration/others/others%d.png",
        1,
        12
    };

    //    DecorationDecConfig otherFood =
    //    {
    //        "otherFood",
    //        "ui/dec/ICON/others_cottoncandy.png",
    //        "ui/dec/ICON/others_cottoncandy.png",
    //        "res/cotton_candy/icon_decoration/otherfood/otherfood__%d.png",
    //        "res/decoration/other food/otherfood__%d.png",
    //        1,
    //        20
    //    };
    
    _configVec =
    {
        candyConfig,sprinkleConfig,fruitConfig,faceConfig
        //        otherFood
    };
    
}

bool RainbowCakepopDecScene::init()
{
    if (!DecorationBaseLayer::init())
    {
        return false;
    }
    this->setBackground("res/eat.png");
    
    kAudioUtil->playEffect(kVoiceJuiceDec);
    this->runAction(Sequence::create(DelayTime::create(4.f),
                                     CallFunc::create([=]
                                                      {
                                                          //                                                          _firstCandy = true;
                                                      }),
                                     NULL));
    
    _module = "rainbowcakepop";
    _iapIdentifier = ShopScene::getAllContentString();
    
    _food = CakePopNode::create();
    this->addChild(_food,11);
    ActionHelper::moveInToSceneBounce(_food, CMVisibleRect::getPosition(320, 620), kActionDirectionLeft);
    //    return true;
    
    this->initTypeButton();
    this->_typeBtnClicked = [=](Widget* btn)
    {
        this->showScrollView(btn->getTag());
    };
    
    this->showScrollView(0);
    
    
    this->showNextButton();
    this->onNextButtonTouchCallback = [=]
    {
        this->onNextButtonTouchCallback = nullptr;
        kAudioUtil->playEffect(kVoiceDecOver);
        _director->getEventDispatcher()->removeAllEventListeners();
        
        _food->saveImage(nullptr);
        this->runAction(Sequence::create(DelayTime::create(1.1f),
                                         CallFunc::create([=]
                                                          {
                                                              //                                                              kESDataManager->setCurrentFood(ESDataManager::FoodType::kFoodCottonCandy);
                                                              SceneManager::getInstance()->replaceWithDefaultTransition(RainbowCakepopShareScene::scene());
                                                          }),
                                         NULL));
        //        SceneManager::getInstance()->replaceWithDefaultTransition(RainbowCupcakeShareScene::scene());
        
    };
    
    this->onResetButtonTouchCallback = [=]
    {
        //        auto food = kESDataManager->getCottonCandyFood();
        //        food.hasOtherFood = false;
        //        food.stickIndex = 1;
        //        kESDataManager->setCottonCandyFood(food);
        //
                SceneManager::getInstance()->replaceWithDefaultTransition(RainbowCakepopDecScene::scene());
    };
    
    
    
    return true;
}

void RainbowCakepopDecScene::showScrollView(int index)
{
    function<void(ui::Widget * item)> callback = nullptr;
    if (_currentSprinkle)
    {
        _currentSprinkle->setEnable(false);
        _currentSprinkle = nullptr;
    }
    
    
    if (index == 0 || index == 2)
    {
        kAudioUtil->playEffect(kVoiceChooseTopping);
        callback = [=](ui::Widget * item)
        {
            setTypeEnable(false);
            int itemTag = item->getTag();
            
            Vec2 pos = _food->getPosition()+Vec2(0, 0);
            
            auto changeCall = [=]
            {
                setTypeEnable(true);
                _food->addOther(StringUtils::format(_configVec.at(index).fileFormat.c_str(),itemTag));
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
        callback = [=](ui::Widget* item)
        {
            //            if (_first)
            //            {
            //                _first = false;
            //                auto node = TipsUIController::getInstance()->showClickTip(this, CMVisibleRect::getCenterOfScene());
            //                node->setScale(0.8f);
            //            }
            
            
            if(_currentSprinkle == nullptr){
                _currentSprinkle = AddSprinkle::create();
                auto startPos = _scrollView->getInnerContainer()->convertToWorldSpace(item->getPosition());
                _food->addSprinkle(_currentSprinkle);
                _currentSprinkle->touchCallBack = [this](Touch*){
                    //                    this->stopPrompt();
                    //                    playVoice();
                    kAudioUtil->playEffect(kSoundAddSprinkle);
                    TipsUIController::getInstance()->removeAllTips();
                };
                _currentSprinkle->caculatePos(Size(264,356));
            }
            
            
            _currentSprinkle->setSprinklePath(kESDataManager->getSprinklePath(item->getTag()));
            _currentSprinkle->setEnable(true);
        };
    }
    if (index == 3)
    {
        callback = [=](ui::Widget* item)
        {
            setTypeEnable(false);
            int itemTag = item->getTag();
            
            Vec2 pos = _food->getPosition()+Vec2(0, 0);
            
            auto changeCall = [=]
            {
                setTypeEnable(true);
                _food->updateFace(StringUtils::format(_configVec.at(index).fileFormat.c_str(),itemTag));
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

