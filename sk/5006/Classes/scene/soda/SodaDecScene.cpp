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
#include "JuiceMachine.h"
#include "LockUIManager.h"
#include "RewardManager.h"
#include "AddSprinkle.h"
#include "ESDragUnitAdapter.h"
#include "ESScrollView.h"
REGIST_SECENE(SodaDecScene)

#define stickerScale 1.3f
SodaDecScene::SodaDecScene():
_juice(nullptr),
_sprinkleTouch(nullptr),
_selectMachineWidget(nullptr),
_isAnimation(false),
currentSprinkle(nullptr),
_temp(nullptr),
_first(true),
_stepIndex(0)
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

    
    _configVec =
    {
        sprinkleConfig,candyConfig,fruitConfig
    };
    
}


Scene* SodaDecScene::createSceneWithStepIndex(int index)
{
    Scene *scene = Scene::create();
    SodaDecScene *layer = SodaDecScene::createWithStepIndex(index);
    scene->addChild(layer);
    return scene;
}

SodaDecScene* SodaDecScene::createWithStepIndex(int index)
{
    
    SodaDecScene* pRet = new SodaDecScene();
    pRet->_stepIndex = index;
    if (pRet && pRet->init())
    {
        return pRet;
    }

    return nullptr;
}


bool SodaDecScene::init()
{
    if (!DecorationBaseLayer::init())
    {
        return false;
    }
    this->setBackground("res/eat.png");

//    Analytics::getInstance()->sendScreenEvent("food0.0_milkshake_decorate");
    
    auto food = kESDataManager->getSodaFood();
    food.strawIndex = -1;
    kESDataManager->setSodaFood(food);

    
    kAudioUtil->playEffect(kVoiceDecChooseCream);
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
    
    this->showOtherDec();
    
    return true;
}

void SodaDecScene::chooseCream()
{
    
    auto scrollNode = Node::create();
    scrollNode->setPosition(CMVisibleRect::getPosition(320,40,kBorderNone,kBorderTop));
    addChild(scrollNode, 11);
    
    auto adapter = ESUnitAdapter::create("res/4decoration/1cream/cream_machine/cream%d.png", 1, 20);
    auto scroll = ESScrollView::create();
    scroll->setItemAdapter(adapter);
    scroll->setDirection(ScrollView::Direction::HORIZONTAL);
    scroll->setContentSize(Size(getContentSize().width,getContentSize().height));
    scroll->setMarginX(250+20);
    scroll->setMarginY(getContentSize().height);
    scroll->loadScrollView();
    scroll->setAnchorPoint(Vec2(0.5, 0.5));
    scrollNode->addChild(scroll);
    scroll->loadScrollView();
    scroll->setBounceEnabled(false);
    
    for (auto item : scroll->vec_Item)
    {
        item->setOpacity(0);
        int index = item->getTag();
        auto juiceMachine = JuiceMachine::create(index);
        juiceMachine->setPosition(juiceMachine->getContentSize()/2);
        juiceMachine->setName("machine");
        item->addChild(juiceMachine, 1);
        auto key = kRewardManager->getItemKey("soda", "cream", index);
        item->setUserObject(__String::create(key));
        if(!LockUIManager::getInstance()->registerIAPLock(ShopScene::getAllContentString(), "cream", index, item))
        {
            LockUIManager::getInstance()->registerLock("soda", "cream", index, item,Vec2(220,120));
        }
    }
    
    adapter->unitTouchEnded = [=](ui::Widget* item)
    {
        if (_isAnimation ||
            _selectMachineWidget == item)
            return;
        
        bool hasPurchased = kIAPManager->isPackagePurchased(ShopScene::getAllContentString());
        if (!hasPurchased)
        {
            if (kIAPManager->isFree("cup", item->getTag()))
            {
                auto keyObject = item->getUserObject();
                if(!keyObject)
                    return;
                string key = ((__String*)(keyObject))->getCString();
                if(kRewardManager->isLocked(key))
                {
                    //如果能找到reward名字的lock，显示reward，否则显示iap
                    if (item->getChildByName(LOCK_NAME_REWARD))
                        kRewardManager->showRewardAds(key);
                    return;
                }
            }
        }
        
        kAudioUtil->playEffect("Click button.mp3");
        
        auto machine = dynamic_cast<JuiceMachine*>(item->getChildByName("machine"));
        machine->pourJuiceCallback = [=]()
        {
            kAudioUtil->playEffect("Open and close door.mp3");
            kAudioUtil->playEffect("Pouring soda.mp3");
            
            scroll->setEnabled(false);
            
            _juice->updateCream(item->getTag());
            
            this->runAction(Sequence::create(DelayTime::create(1.2f), CallFunc::create([=](){
                
                kAudioUtil->stopAllEffect();
                this->juiceMachineUnSelect();
            }), DelayTime::create(1.0), CallFunc::create([=](){
                
                scrollNode->runAction(Sequence::create(MoveBy::create(1.0, Vec2(0.0, 500.0)), RemoveSelf::create(), NULL));
            }), DelayTime::create(1.0), CallFunc::create([=](){
                
                //                _juice->testCup();
                scrollNode->runAction(MoveTo::create(1.2f, Vec2(scroll->getInnerContainerSize().width+800, 0)));
                
                _juice->runAction(Sequence::create(
                                                 CallFunc::create([=]
                                                                  {
                                                                      _juice->saveJuiceImage([=](RenderTexture *, const std::string &)
                                                                                             {
                                                                                             });
                                                                  }),
                                                   Spawn::create(ScaleTo::create(0.4f, 1.2f),
                                                                 MoveTo::create(0.4f, CMVisibleRect::getPosition(320, 490+40)),
                                                                 NULL),
                                                   CallFunc::create([=]
                                                                    {
                                                                        this->showDecorationParticle(_juice->getPosition());
                                                                    }),
//                                                 DelayTime::create(1.6f),
                                                 CallFunc::create([=]
                {
                    this->showNextButton();
                    this->onNextButtonTouchCallback=[=]
                    {
                        this->showOtherDec();
                        TipsUIController::getInstance()->removeAllTips();
                    };;
                    
//                    this->showOtherDec();
                }),
                                                 NULL));
            }), NULL));
        };
        
        auto selectCall = [=]()
        {
            auto position = item->convertToWorldSpaceAR(Vec2::ZERO);
            CCLOG("%f %f", position.x, position.y);
            scroll->scrollToIdx(item->getTag());
            scroll->setEnabled(false);
            //            scrollBy(0.3, Vec2(getContentSize().width/2 - position.x, 0.0), nullptr);
            
            juiceMachineSelect(item);
        };
        
        if (_selectMachineWidget){
            juiceMachineUnSelect();
        }
        
        selectCall();
    };
    
    scroll->addEventListener([=](Ref* ref, ScrollView::EventType type){
        if (type == ScrollView::EventType::CONTAINER_MOVED)
        {
            log ("%f ,scroll->getContainer.x ",scroll->getInnerContainerPosition().x);
            
            if (_isAnimation || !_selectMachineWidget)
                return;
            
            
            //检测选中的机器位置是否在正中，如果在，则不将机器缩回去
            auto position = _selectMachineWidget->convertToWorldSpaceAR(Vec2::ZERO);
            if (std::abs(position.x - 320) < 10.0)
                return;
            
            dynamic_cast<JuiceMachine*>(_selectMachineWidget->getChildByName("machine"))->onOffAction(false);
            juiceMachineUnSelect();
        }
    });
    
    scroll->show(nullptr);
}

void SodaDecScene::juiceMachineSelect(Widget *widget)
{
    _isAnimation = true;
    auto soda = kESDataManager->getSodaFood();
    soda.creamIndex = widget->getTag();
    kESDataManager->setSodaFood(soda);
    
    widget->runAction(ScaleTo::create(0.2f, 1.05));
    widget->runAction(Sequence::create(MoveBy::create(0.3, Vec2(0.0, -90)), CallFunc::create([=](){
        
        _isAnimation = false;
        dynamic_cast<JuiceMachine*>(widget->getChildByName("machine"))->setMachineSelect(true);
        _selectMachineWidget = widget;
    }), NULL));
}

void SodaDecScene::juiceMachineUnSelect()
{
    _isAnimation = true;
    if (!_selectMachineWidget)
        return;
    _selectMachineWidget->runAction(ScaleTo::create(0.2f, 1.f));
    _selectMachineWidget->runAction(Sequence::create(MoveBy::create(0.27, Vec2(0.0, 90)), CallFunc::create([=](){
        
        dynamic_cast<JuiceMachine*>(_selectMachineWidget->getChildByName("machine"))->onOffAction(false);
        dynamic_cast<JuiceMachine*>(_selectMachineWidget->getChildByName("machine"))->setMachineSelect(false);
        _selectMachineWidget = nullptr;
        _isAnimation = false;
    }), NULL));
}


void SodaDecScene::showOtherDec()
{
    this->onResetButtonTouchCallback = [=]
    {
        kESDataManager->resetSodaShare();
        SceneManager::getInstance()->replaceWithDefaultTransition(SodaDecScene::createSceneWithStepIndex(1));
    };

    _juice->runAction(ScaleTo::create(0.4f, 1.f));
    _juice->runAction(MoveTo::create(0.4f, CMVisibleRect::getPosition(320, 490+40)));
//    _juice->showDrink();
    _juice->showLid(false);
    auto path = FileUtils::getInstance()->getWritablePath()+ shareNameJuice;
    if (!FileUtils::getInstance()->isFileExist(path))
    {
        _juice->updateCream(kESDataManager->getSodaFood().creamIndex);
    }
    
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
        _juice->showLid(true);
        TipsUIController::getInstance()->removeAllTips();
        this->hideNextButton();
        if (currentSprinkle)
        {
            currentSprinkle->setEnable(false);
            currentSprinkle = nullptr;
        }
        if (_sprinkleTouch)
        {
            _sprinkleTouch->getOwner()->removeComponent(_sprinkleTouch);_sprinkleTouch=nullptr;
        }

        kAudioUtil->stopAllEffect();
        this->removeCycleScroll(false,nullptr);
        this->chooseStraw();
    };

}


void SodaDecScene::showScrollView(int index)
{
    function<void(ui::Widget * item)> callback;
    if (currentSprinkle)
    {
        currentSprinkle->setEnable(false);
        currentSprinkle = nullptr;
    }
    if (_sprinkleTouch)
    {
        _sprinkleTouch->getOwner()->removeComponent(_sprinkleTouch);_sprinkleTouch=nullptr;
    }
    
    kAudioUtil->stopAllEffect();
    if (index == 0)//cream
    {
        kAudioUtil->playEffect(kVoiceDecChooseSprinkle);

        callback = [=](ui::Widget * item)
        {
//            setTypeEnable(false);
//            int itemTag = item->getTag();
            if (_first)
            {
                _first = false;
                auto node = TipsUIController::getInstance()->showClickTip(this, CMVisibleRect::getCenterOfScene());
                node->setScale(0.8f);
            }

            
            if(currentSprinkle == nullptr){
                currentSprinkle = AddSprinkle::create();
                auto startPos = _scrollView->getInnerContainer()->convertToWorldSpace(item->getPosition());
                _juice->addSprinkleAnimated(currentSprinkle, startPos);
                currentSprinkle->touchCallBack = [this](Touch*){
//                    this->stopPrompt();
//                    playVoice();
                    kAudioUtil->playEffect(kSoundAddSprinkle);
                    TipsUIController::getInstance()->removeAllTips();
                };
                currentSprinkle->caculatePos(_juice->getContentSize()+Size(0,300));
            }

            
            currentSprinkle->setSprinklePath(kESDataManager->getSprinklePath(item->getTag()));
            currentSprinkle->setEnable(true);
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
            //            setTypeEnable(false);
            if (_first)
            {
                _first = false;
                auto node = TipsUIController::getInstance()->showClickTip(this, CMVisibleRect::getCenterOfScene());
                node->setScale(0.8f);
            }

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
    
    _nextButton->setTouchEnabled(enable);
    _resetButton->setTouchEnabled(enable);
    
    this->ignorAllTouch(!enable);
}

void SodaDecScene::chooseStraw()
{
    _juice->stopAllActions();
    _juice->runAction(MoveTo::create(0.5f,CMVisibleRect::getPosition(320, 445)));
    _juice->runAction(ScaleTo::create(0.5f, 0.7f));
    this->removeTypeButton();
    kAudioUtil->playEffect(kVoiceDecChooseStraw);
    
    DecorationDecConfig strawConfig =
    {
        "straw",
        "ui/dec/fruit_up.png",
        "ui/dec/fruit.png",
        "res/rainbow_slushy/decoration/icon_straw/straw%d.png",
        "res/rainbow_slushy/decoration/straw/straw%d.png",
        1,
        15
    };

    auto callback = [=](ui::Widget * item)
    {
        //            setTypeEnable(false);
        //            int itemTag = item->getTag();
        _scrollView->setEnabled(false);
        int itemTag = item->getTag();
        log("===item clicked %d====",itemTag);
        auto food = kESDataManager->getSodaFood();
        food.strawIndex = itemTag;
        kESDataManager->setSodaFood(food);
        
        Vec2 pos = CMVisibleRect::getPosition(320, 426);
        
        auto changeCall = [=]
        {
            this->showNextButton();
            
            _scrollView->setEnabled(true);
            
            _juice->updateStraw(itemTag);
            //            _corndog->setVisible(true);
            //            _corndog->updateStick();
            this->showDecorationParticle(pos);
        };
        auto tempBox = Sprite::create(StringUtils::format(strawConfig.fileFormat.c_str(),itemTag));
        tempBox->setScale(0.3f);
        auto startPos = _scrollView->getInnerContainer()->convertToWorldSpace(item->getPosition());
        tempBox->setPosition(startPos);
        auto btm = Sprite::create();
        tempBox->addChild(btm,-1);
        btm->setPosition(tempBox->getContentSize()/2);
        
        tempBox->runAction(Sequence::create(Spawn::create(ActionHelper::moveToPostionByBezier(startPos,pos, 0.8),
                                                          EaseBackOut::create(ScaleTo::create(0.8, 0.9)),
                                                          NULL),
                                            CallFunc::create(changeCall),
                                            RemoveSelf::create(),
                                            NULL));
        this->addChild(tempBox,INT_MAX);
    };
    

    this->setupDecorationWithConfig(strawConfig, callback);
    
    
    this->onNextButtonTouchCallback = [=]
    {
        this->onNextButtonTouchCallback = nullptr;
        kAudioUtil->stopAllEffect();
        _director->getEventDispatcher()->removeAllEventListeners();
        kAudioUtil->playEffect(kVoiceDecOver);
        _juice->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5, CMVisibleRect::getPosition(320, 480)),
                                                         ScaleTo::create(0.5f, 1.1f),
                                                         NULL),
                                           CallFunc::create([=]
                                                            {
                                                                _juice->saveJuiceImage([](RenderTexture *, const std::string &)
                                                                                       {
                                                                                       });
                                                            }),
                                           DelayTime::create(1.1),
                                           CallFunc::create([=]
                                                            {
                                                                SceneManager::getInstance()->replaceWithDefaultTransition(SodaShareScene::scene());
                                                            }),
                                         NULL));
    };
}

