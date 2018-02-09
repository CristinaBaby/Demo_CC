//
//  TexasPizzaDecScene.cpp
//  ccats1061
//
//  Created by liji on 16/8/23.
//
//

#include "TexasPizzaDecScene.h"
#include "ScribbleSauce.h"
#include "StickerDrawNode.h"
#include "ComponentTouchMove.h"
#include "SceneHeader.h"
#include "Analytics.h"
#define stickerScale 1.3f
TexasPizzaDecScene::TexasPizzaDecScene():
_texasPizze(nullptr)
{
    DecorationConfig boxConfig =
    {
        "box",
        "ui/dec/dec/box_up.png",
        "ui/dec/dec/box_down.png",
        "res/decoration/icon/box/box_%d.png",
        "res/decoration/box/box_%d.png",
        1,
        20
    };
    
    DecorationConfig stickerConfig =
    {
        "sticker",
        "ui/dec/dec/sticker_up.png",
        "ui/dec/dec/sticker_down.png",
        "res/decoration/icon/paster/paster%d.png",
        "res/decoration/paster/paster%d.png",
        1,
        20
    };
    
    DecorationConfig bowtieConfig =
    {
        "bowtie",
        "ui/dec/dec/bowknot_up.png",
        "ui/dec/dec/bowknot_down.png",
        "res/decoration/icon/bowknot/bowknot_%d.png",
        "res/decoration/bowknot/bowknot_%d.png",
        1,
        20
    };
    
    
    _configVec =
    {
        boxConfig,stickerConfig,bowtieConfig
    };
    
}

bool TexasPizzaDecScene::init()
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
    
    _module = "pizza";
    _iapIdentifier = ShopScene::getAllContentString();
    
    _texasPizze = TexasPizzaNode::create();
    this->addChild(_texasPizze,10);
    //    _texasPizze->showLid();
    ActionHelper::moveInToSceneBounce(_texasPizze, CMVisibleRect::getCenterOfScene(), kActionDirectionRight);
    
    this->initTypeButton();
    this->_typeBtnClicked = [=](Widget* btn)
    {
        this->showScrollView(btn->getTag());
        
    };
    this->showScrollView(0);
    kAudioUtil->playEffect(kVoicePizzaDec);

    this->showNextButton();
    this->onNextButtonTouchCallback = [=]
    {
        kAudioUtil->playEffect(kVoiceDecOver);
        _director->getEventDispatcher()->removeAllEventListeners();

        this->runAction(Sequence::create(DelayTime::create(2.1),
                                         CallFunc::create([=]
        {
            kESDataManager->setCurrentFood(ESDataManager::FoodType::kFoodPizza);
            SceneManager::getInstance()->replaceWithDefaultTransition(ChooseServeScene::scene());
        }),
                                         NULL));
    };
    
    this->onResetButtonTouchCallback = [=]
    {
        kESDataManager->resetPizzaShare();
        SceneManager::getInstance()->replaceWithDefaultTransition(TexasPizzaDecScene::scene());
    };
    
    return true;
}



void TexasPizzaDecScene::showScrollView(int index)
{
    function<void(ui::Widget * item)> callback;
    kAudioUtil->stopAllEffect();

    if (index == 0)
    {
        if (_firstCup)
        {
            kAudioUtil->playEffect(kVoiceDecChooseBox);
        }
        
        callback = [=](ui::Widget * item)
        {
            setTypeEnable(false);
            int itemTag = item->getTag();
            
            auto tPizza = kESDataManager->getTexasPizzaFood();
            tPizza.boxIndex = itemTag;
            kESDataManager->setTexasPizzaFood(tPizza);
            
            auto changeCall = [=]
            {
                setTypeEnable(true);
                _texasPizze->updateBox(itemTag);
                this->showDecorationParticle(_texasPizze->getPosition());
            };
            auto tempBox = Sprite::create(StringUtils::format(_configVec.at(index).fileFormat.c_str(),itemTag));
            tempBox->setScale(0.3f);
            auto startPos = _scrollView->getInnerContainer()->convertToWorldSpace(item->getPosition());
            tempBox->setPosition(startPos);
            auto btm = Sprite::create();
            tempBox->addChild(btm,-1);
            btm->setPosition(tempBox->getContentSize()/2);
            
            tempBox->runAction(Sequence::create(Spawn::create(ActionHelper::moveToPostionByBezier(startPos,_texasPizze->getPosition()+Vec2(0,80), 0.8),
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
        kAudioUtil->playEffect(kVoiceDecChooseSticker);
        callback = [=](ui::Widget * item)
        {
            setTypeEnable(false);
            int itemTag = item->getTag();
            
            auto tPizza = kESDataManager->getTexasPizzaFood();
            tPizza.stickerIndex = itemTag;
            kESDataManager->setTexasPizzaFood(tPizza);
            
            auto changeCall = [=]
            {
                setTypeEnable(true);
                _texasPizze->updateSticker(StringUtils::format(_configVec.at(index).fileFormat.c_str(),itemTag),true);
                this->showDecorationParticle(_texasPizze->getPosition());
            };
            auto tempBox = Sprite::create(StringUtils::format(_configVec.at(index).fileFormat.c_str(),itemTag));
            tempBox->setScale(0.3f);
            auto startPos = _scrollView->getInnerContainer()->convertToWorldSpace(item->getPosition());
            tempBox->setPosition(startPos);
            auto btm = Sprite::create();
            tempBox->addChild(btm,-1);
            btm->setPosition(tempBox->getContentSize()/2);
            
            tempBox->runAction(Sequence::create(Spawn::create(ActionHelper::moveToPostionByBezier(startPos,_texasPizze->getPosition()+Vec2(0,80), 0.8),
                                                              EaseBackOut::create(ScaleTo::create(0.8, 1.15)),
                                                              NULL),
                                                CallFunc::create(changeCall),
                                                RemoveSelf::create(),
                                                NULL));
            this->addChild(tempBox,INT_MAX);
        };
    }
    if (index == 2)
    {
        kAudioUtil->playEffect(kVoiceDecChooseBowtie);
        
        callback = [=](ui::Widget * item)
        {
            setTypeEnable(false);
            int itemTag = item->getTag();
            
            auto tPizza = kESDataManager->getTexasPizzaFood();
            tPizza.bowIndex = itemTag;
            kESDataManager->setTexasPizzaFood(tPizza);
            
            auto changeCall = [=]
            {
                setTypeEnable(true);
                _texasPizze->updateBowtie(StringUtils::format(_configVec.at(index).fileFormat.c_str(),itemTag),true);
                this->showDecorationParticle(_texasPizze->getPosition());
            };
            auto tempBox = Sprite::create(StringUtils::format(_configVec.at(index).fileFormat.c_str(),itemTag));
            tempBox->setScale(0.3f);
            auto startPos = _scrollView->getInnerContainer()->convertToWorldSpace(item->getPosition());
            tempBox->setPosition(startPos);
            auto btm = Sprite::create();
            tempBox->addChild(btm,-1);
            btm->setPosition(tempBox->getContentSize()/2);
            
            tempBox->runAction(Sequence::create(Spawn::create(ActionHelper::moveToPostionByBezier(startPos,_texasPizze->getPosition()+Vec2(0,80), 0.8),
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

void TexasPizzaDecScene::onEnter()
{
    DecorationBaseLayer::onEnter();
}

void TexasPizzaDecScene::setTypeEnable(bool enable)
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

