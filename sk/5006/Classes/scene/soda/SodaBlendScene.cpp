//
//  SodaBlendScene.cpp
//  CCATS022
//
//  Created by liji on 16/11/11.
//
//

#include "SodaBlendScene.h"
#include "ESScrollView.h"
#include "ESUnitAdapter.h"
#include "ESDataManager.h"
#include "ESDragUnitAdapter.h"
#include "BowlNode.h"
#include "IceContainerNode.h"
#include "ComponentTouchMove.h"
#include "MakeNodeManager.h"
#include "ComponentCycleDisplay.hpp"
#include "ComponentTurning.hpp"
#include "ComponentCycleScroll.h"
#include "ShopScene.h"
#include "LockUIManager.h"
#include "RewardManager.h"
#include "LiquidNode.h"
#include "SceneHeader.h"
#include "JuiceMachine.h"
#include "MiniShopScene.h"
REGIST_SECENE(SodaBlendScene)
SodaBlendScene::SodaBlendScene():
_bowl(nullptr),
_blender(nullptr),
_inBlender(nullptr),
_cTurn(nullptr),
_isFirst(true),
_cup(nullptr),
_isBlenderChosen(false),
_selectMachineWidget(nullptr),
_straw(nullptr)
{
    
}

SodaBlendScene::~SodaBlendScene()
{
    
}

bool SodaBlendScene::init()
{
    if (!ESMakeBaseScene::init())
    {
        return false;
    }
//    _screenEventName = "food0.0_milkshake_stir";
    
    this->setBackground("res/rainbow_slushy/bg-2.png");
    auto food = kESDataManager->getSodaFood();
    food.flavorInBlenderVec.clear();
    kESDataManager->setSodaFood(food);
    
    _blender = BowlNode::create("res/rainbow_slushy/blender/blender1/blender1_down.png","res/rainbow_slushy/blender/blender1/blende1_up.png");
    _blender->initMaskNode("res/rainbow_slushy/blender/blender1/blender_mask.png");
    this->addToContentLayer(_blender,10);
    
    ActionHelper::moveInToSceneBounce(_blender, CMVisibleRect::getPosition(365, 325), kActionDirectionLeft,[=]
                                      {
//                                          kAudioUtil->playEffect(kVoiceJuiceAddIngredients);
                                          
                                          showIce();
                                      });
    
    
    
    
    return true;
}

void SodaBlendScene::showIce()
{
    auto icebox = IceContainerNode::create();
    this->addToContentLayer(icebox,20);
    icebox->showIce();
    
    auto call1 = [=]
    {
//        TipsUIController::getInstance()->showAccelerateTip(this, icebox->getPosition(),false);
        kAudioUtil->playEffect(kVoiceJuiceAddIce);
        
        auto call = [=]
        {
            TipsUIController::getInstance()->removeAllTips();
            for(int i =0;i<5 ;i++)
            {
                auto ice = Sprite::create(icebox->getIcePath());
                Vec2 pos;
                if (i<3)
                {
                    pos.x = random(-130,-30);
                }
                else
                {
                    pos.x = random(-30,50);
                }
                pos.y = random(-20, 10);
                _blender->addContentToMask(ice, "ice2", 1,pos);
                ice->setRotation(rand()%360);
                ice->setScale(random(75,90)*0.01);
                ice->setVisible(false);
                ice->runAction(Sequence::create(DelayTime::create(0.6*i),
                                                Show::create(),
                                                CallFunc::create([]
                                                                 {
                                                                     kAudioUtil->playEffect(kSoundAddIce);
                                                                 }),
                                                EaseBackIn::create(MoveBy::create(0.8f, Vec2(0, -100))),
                                                NULL));
                _tempInBlenderVec.push_back(ice);
            }
            icebox->runAction(Sequence::create(DelayTime::create(0.6*6),
                                               CallFunc::create([=]
                                                                {
                                                                    auto showNext = [=]
                                                                    {
                                                                        chooseJuice();
                                                                    };
                                                                    ActionHelper::moveOutofSceneAndRemoveSelf(icebox, kActionDirectionRight,showNext);
                                                                }),
                                               NULL));
            
        };
        icebox->showRotateCallback(-10,call);
    };
    ActionHelper::moveInToSceneBounce(icebox, CMVisibleRect::getPosition(511, 620), kActionDirectionRight,call1);
    icebox->setScale(0.6f);
}

void SodaBlendScene::chooseJuice()
{
    _blender->runAction(MoveTo::create(0.5f, CMVisibleRect::getPosition(365, 265)));
    
    auto scrollNode = Node::create();
    scrollNode->setPosition(CMVisibleRect::getPosition(320,80,kBorderNone,kBorderTop));
    addChild(scrollNode, 11);
    
    auto adapter = ESUnitAdapter::create("res/rainbow_slushy/machine/juice_%d.png", 0, 14);
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
        auto key = kRewardManager->getItemKey("soda", "flavor", index);
        item->setUserObject(__String::create(key));
        if(!LockUIManager::getInstance()->registerIAPLock(ShopScene::getAllFoodString(), "flavor", index, item))
        {
            LockUIManager::getInstance()->registerLock("soda", "flavor", index, item,Vec2(220,120));
        }
    }
    
    adapter->unitTouchEnded = [=](ui::Widget* item)
    {
        if (_isAnimation ||
            _selectMachineWidget == item)
            return;
        
        bool hasPurchased = kIAPManager->isPackagePurchased(ShopScene::getAllFoodString());
        if (!hasPurchased)
        {
            if (kIAPManager->isFree("flavor", item->getTag()))
            {
                auto keyObject = item->getUserObject();
                if(!keyObject)
                    return;
                string key = ((__String*)(keyObject))->getCString();
                if(kRewardManager->isLocked(key))
                {
                    //如果能找到reward名字的lock，显示reward，否则显示iap
                    if (item->getChildByName(LOCK_NAME_REWARD))
                    {
                        static bool s_isClicked = false;
                        if (s_isClicked)
                        {
                            return;
                        }
                        s_isClicked = true;
                        this->runAction(Sequence::create(DelayTime::create(0.2f),
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
                        
                            this->showShop();
                        };
                    }
                    return;
                }
            }
        }
        
        kAudioUtil->playEffect("sound/sound1/Open and close door.mp");
        
        auto machine = dynamic_cast<JuiceMachine*>(item->getChildByName("machine"));
        machine->pourJuiceCallback = [=]()
        {
            kAudioUtil->playEffect("Open and close door.mp3");
            kAudioUtil->playEffect("Pouring soda.mp3");
            
            scroll->setEnabled(false);
            
            float duration = 7.f;
            auto inBlender = Sprite::create(StringUtils::format("res/rainbow_slushy/blender/blender1/blender_%d.png",item->getTag()));
            _blender->addContentToMask(inBlender, "_blender", 2,Vec2(0, -40));
            _blender->showContentInMask("_blender",duration);
            
//            _juice->updateCream(item->getTag());
//            
            this->runAction(Sequence::create(DelayTime::create(duration),
                                             CallFunc::create([=]()
            {
                
                kAudioUtil->stopAllEffect();
                this->juiceMachineUnSelect();
            }),
                                             DelayTime::create(1.0),
                                             CallFunc::create([=]()
            {
                
                scrollNode->runAction(Sequence::create(MoveBy::create(1.0, Vec2(0.0, 500.0)), RemoveSelf::create(), NULL));
            }),
                                             DelayTime::create(1.0),
                                             CallFunc::create([=](){
                
                //                _juice->testCup();
                scrollNode->runAction(MoveTo::create(1.2f, Vec2(scroll->getInnerContainerSize().width+800, 0)));
                
                _blender->runAction(Sequence::create(MoveBy::create(0.8f, Vec2(800, 0)),
                                                     CallFunc::create([=]
                {
                    this->showAutoBlender();
                }),
                                                     NULL));
            }),
                                             NULL));
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

void SodaBlendScene::juiceMachineSelect(Widget *widget)
{
    _isAnimation = true;
    auto soda = kESDataManager->getSodaFood();
    soda.flavorIndex = widget->getTag();
    kESDataManager->setSodaFood(soda);
    
    widget->runAction(ScaleTo::create(0.2f, 1.05));
    widget->runAction(Sequence::create(MoveBy::create(0.3, Vec2(0.0, -90)), CallFunc::create([=](){
        
        _isAnimation = false;
        dynamic_cast<JuiceMachine*>(widget->getChildByName("machine"))->setMachineSelect(true);
        _selectMachineWidget = widget;
    }), NULL));
}

void SodaBlendScene::juiceMachineUnSelect()
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

void SodaBlendScene::showBlender()
{
    auto autoBtn = Button::create("res/3mix/1/icon_blender2.png");
    autoBtn->loadTexturePressed("res/3mix/1/icon_blender2.png");
    autoBtn->loadTextureDisabled("res/3mix/1/icon_blender2.png");
    this->addToContentLayer(autoBtn,10,22);
    autoBtn->setScale(0.9f);
    auto key = kRewardManager->getItemKey("soda", "tool", 2);
    autoBtn->setUserObject(__String::create(key));
    if(!LockUIManager::getInstance()->registerIAPLock(ShopScene::getAllContentString(), "tool", 2, autoBtn))
    {
        LockUIManager::getInstance()->registerLock("soda", "tool", 2, autoBtn,Vec2(220,20));
    }
    ActionHelper::moveInToSceneBounce(autoBtn, CMVisibleRect::getPosition(162, 393+50), kActionDirectionLeft);
    
    auto mBtn = Button::create("res/3mix/1/icon_blender1.png");
    mBtn->loadTexturePressed("res/3mix/1/icon_blender1.png");
    mBtn->loadTextureDisabled("res/3mix/1/icon_blender1.png");
    this->addToContentLayer(mBtn,10,33);
    mBtn->setScale(0.7f);
    ActionHelper::moveInToSceneBounce(mBtn, CMVisibleRect::getPosition(465, 393+50), kActionDirectionRight);
    _tempButton.push_back(mBtn);
    _tempButton.push_back(autoBtn);
    
    kAudioUtil->playEffect(kVoiceJuiceChooseBlender);
    
    auto call = [=](Ref* r,Widget::TouchEventType t)
    {
        if (_isBlenderChosen)
        {
            return;
        }
        kAudioUtil->stopAllEffect();
        auto btn = dynamic_cast<Button*>(r);
        int tag = btn->getTag();
        if (t == Widget::TouchEventType::ENDED)
        {
            
            switch (tag)
            {
                case 22:
                {
                    bool hasPurchased = kIAPManager->isPackagePurchased(ShopScene::getAllContentString());
                    if (!hasPurchased)
                    {
                        if (kIAPManager->isFree("tool", 2))
                        {
                            auto keyObject = btn->getUserObject();
                            if(!keyObject)
                                return;
                            string key = ((__String*)(keyObject))->getCString();
                            if(kRewardManager->isLocked(key))
                            {
                                //如果能找到reward名字的lock，显示reward，否则显示iap
                                if (btn->getChildByName(LOCK_NAME_REWARD))
                                    kRewardManager->showRewardAds(key);
                                return;
                            }
                        }
                    }
                    if (!_isBlenderChosen)
                    {
                        _isBlenderChosen = true;
                    }

                    this->runAction(Sequence::create(DelayTime::create(1.2f),
                                                     CallFunc::create([=]
                                                                      {
                                                                          showAutoBlender();
                                                                      }),
                                                     NULL));
                }
                    break;
                case 33:
                {
                    if (!_isBlenderChosen)
                    {
                        _isBlenderChosen = true;
                    }

                    this->runAction(Sequence::create(DelayTime::create(1.2f),
                                                     CallFunc::create([=]
                                                                      {
                                                                          showManualBlender();
                                                                      }),
                                                     NULL));
                }
                    break;
                    
                default:
                    break;
            }
            
            mBtn->runAction(EaseBackIn::create(MoveBy::create(1.2f,Vec2(800, 0))));
            mBtn->setEnabled(false);
            autoBtn->runAction(MoveBy::create(0.9f,Vec2(-800, 0)));
            autoBtn->setEnabled(false);
        }
    };
    
    mBtn->addTouchEventListener(call);
    autoBtn->addTouchEventListener(call);
    
    
    
}


void SodaBlendScene::showAutoBlender()
{
    auto call = [=]
    {
        auto blenderBtm = Sprite::create("res/rainbow_slushy/blender/blender2.png");
        blenderBtm->setPosition(Vec2(-31,-321));
        _blender->addChild(blenderBtm,-1);
        _blender->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(1.2f, CMVisibleRect::getPosition(348,593))),
                                             CallFunc::create([=]
                                                              {
                                                                  //                                                                    TipsUIController::getInstance()->showClickTip(this, CMVisibleRect::getPosition(281+250, 190));
                                                              }),
                                             NULL));
        
        auto turnSprite = Sprite::create("res/rainbow_slushy/blender/blender_button1.png");
        blenderBtm->addChild(turnSprite,3);
        turnSprite->setPosition(Vec2(150,150));
        MakeNodeManager::getInstance()->registerRemoveNode(_blender, ActionDirection::kActionDirectionLeft);
        
        auto cTouch = ComponentTouchMove::create();
        cTouch->touchEndedCall = [=](Touch *t)
        {
            TipsUIController::getInstance()->removeAllTips();
            kSoundInstance->playLoopEffect(kSoundBlend);
            this->startBlend();
            cTouch->pause();
            turnSprite->setTexture("res/rainbow_slushy/blender/blender_button2.png");
            kAudioUtil->playEffect(kVoiceJuiceBlending);
        };
        cTouch->setIsCanMove(false);
        turnSprite->addComponent(cTouch);
        
        auto lid = Sprite::create("res/rainbow_slushy/blender/blender1/blender_lid.png");
        _blender->addContent(lid, "lid", 3 ,Vec2(0, 0));
        _blender->setContentVisible("lid");
    };
    
    call();
}

void SodaBlendScene::showManualBlender()
{
    auto blenderBtm = Sprite::create("res/3mix/1/blender1_2.png");
    blenderBtm->setPosition(Vec2(-158, -63));
    blenderBtm->setScale(1.1);
    _blender->addChild(blenderBtm,-1);
    auto call = [=]
    {
        auto turnSprite = Sprite::create("res/3mix/1/blender1_3.png");
        this->addToContentLayer(turnSprite,20);
        turnSprite->setAnchorPoint(Vec2(0.5, 0.16));
        turnSprite->setPosition(CMVisibleRect::getPosition(229-28,409-15));
        //    turnSprite->runAction(MoveBy::create(0.3f, Vec2(600, 0)));
        MakeNodeManager::getInstance()->registerRemoveNode(_blender, ActionDirection::kActionDirectionLeft);
        MakeNodeManager::getInstance()->registerRemoveNode(turnSprite, ActionDirection::kActionDirectionLeft);
        
        _cTurn = ComponentTurning::create();
        turnSprite->addComponent(_cTurn);
        
        _cTurn->_turningCallback=[this](float totalAngle)
        {
            kSoundInstance->playLoopEffect(kSoundBlend);
            
            if (_isFirst)
            {
                this->startBlend();
                _isFirst = false;
            }
            else
            {
                this->resumeBlend();
            }
        };
        _cTurn->_turningStop=[this]()
        {
            this->pauseBlend();
            kSoundInstance->stopLoopEffect();
            
        };
    };

    auto turnSpriteShow = Sprite::create("res/3mix/1/blender1_3.png");
    _blender->addChild(turnSpriteShow,20);
    turnSpriteShow->setPosition(-239, -88);
    
    _blender->runAction(Sequence::create(MoveTo::create(0.3f, CMVisibleRect::getPosition(447,500)),
                                         CallFunc::create([=]
                                                          {
                                                              turnSpriteShow->setVisible(false);
                                                              call();
                                                          }),
                                         NULL));
    
    
    auto lid = Sprite::create("res/3mix/1/blender1_1.png");
    _blender->addContent(lid, "lid", 3 ,Vec2(-36, 156));
    _blender->setContentVisible("lid");
}


void SodaBlendScene::startBlend()
{
    for (int i = 0; i<kESDataManager->getJuiceCrashedPath().size(); i++)
    {
        if (FileUtils::getInstance()->isFileExist(kESDataManager->getJuiceCrashedPath().at(i)))
        {
            auto crashed = Sprite::create(kESDataManager->getJuiceCrashedPath().at(i));
            _blender->addContentToMask(crashed, "crashed", 99,Vec2(34,-81));
            auto cCycle = ComponentCycleDisplay::create();
            crashed->addComponent(cCycle);
            cCycle->setName("cCycle");
            _cycleVec.push_back(cCycle);
            crashed->setScale(0);
            crashed->runAction(Sequence::create(DelayTime::create(2.7f),
                                                ScaleTo::create(0.2f, 1),
                                                CallFunc::create([=]
                                                                 {
                                                                     cCycle->start();
                                                                 }),
                                                FadeOut::create(4.f),
                                                NULL));
            _crashedFruitVec.push_back(crashed);
        }
    }
    
    for(auto food : _tempInBlenderVec)
    {
        food->runAction(ActionHelper::createFloatAction());
        food->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, 20)),
                                         FadeOut::create(2.7f),
                                         NULL));
    }
    
    
    
    _inBlender = Sprite::create(kESDataManager->getJuicePath());
    _inBlender->setAnchorPoint(Vec2(0.5, 0.06));
    _inBlender->setOpacity(30);
    _blender->addContentToMask(_inBlender, "inBlender", 99,Vec2(0,-280));
    auto ani = Spawn::create(FadeIn::create(12.f),
                             MoveBy::create(9.f, Vec2(0, 120)),
                             NULL);
    _inBlender->runAction(Sequence::create(ani,
                                           CallFunc::create([=]
                                                            {
                                                                log("============pour");
                                                                MakeNodeManager::getInstance()->removeAllRegisterNodes(CC_CALLBACK_0(SodaBlendScene::chooseCup, this));
                                                                this->pauseBlend();
                                                                if (_cTurn)
                                                                {
                                                                    _cTurn->stop();
                                                                    _cTurn->getOwner()->setVisible(false);
                                                                }
                                                                _inBlender->stopAllActions();
                                                                kSoundInstance->stopLoopEffect();
                                                                
                                                            }),
                                           NULL));
    _inBlender->runAction(RepeatForever::create(ActionHelper::createRollJelly()));
}

void SodaBlendScene::pauseBlend()
{
    _actionManager->pauseTarget(_inBlender);
    for (auto e : _crashedFruitVec)
    {
        _actionManager->pauseTarget(e);
        if (e->getComponent("cCycle"))
        {
            dynamic_cast<ComponentCycleDisplay*>(e->getComponent("cCycle"))->pause();
        }
    }
    for (auto c :_cycleVec)
    {
        c->pause();
    }
    for(auto food : _tempInBlenderVec)
    {
        _actionManager->pauseTarget(food);
    }
}

void SodaBlendScene::resumeBlend()
{
    _actionManager->resumeTarget(_inBlender);
    for (auto e : _crashedFruitVec)
    {
        _actionManager->resumeTarget(e);
    }
    for (auto c :_cycleVec)
    {
        c->resume();
    }
    for(auto food : _tempInBlenderVec)
    {
        _actionManager->resumeTarget(food);
    }
    
}

void SodaBlendScene::chooseCup()
{
    auto scrollNode = Node::create();
    scrollNode->setPosition(CMVisibleRect::getPosition(320, 460+30));
    this->addToContentLayer(scrollNode, 5, 7);
    
    auto adapter = ESUnitAdapter::create("res/rainbow_slushy/cup/cup%d_1.png", 1, 15);
    auto scroll = ESScrollView::create();
    scroll->setItemAdapter(adapter);
    scroll->setDirection(ScrollView::Direction::HORIZONTAL);
    scroll->setContentSize(Size(getContentSize().width,600));
    scroll->setMarginX(250+10);
    scroll->setMarginY(600);
    scroll->loadScrollView();
    //    scroll->setPosition(CMVisibleRect::getPosition(320, 725));
    scroll->setAnchorPoint(Vec2(0.5, 0.5));
    scrollNode->addChild(scroll);
    scroll->loadScrollView();
    
    for (auto item : scroll->vec_Item)
    {
        int index = item->getTag();
        string cupBtmStr = "res/rainbow_slushy/cup/cup1_2.png";
        auto btm = ImageView::create(cupBtmStr);
        item->addChild(btm,-1);
        btm->setAnchorPoint(Vec2::ZERO);
        item->setScale(0.8f);
        if (item->getTag()%2)
        {
            item->setPosition(item->getPosition()+Vec2(0, 90));
        }
        
        
        auto key = kRewardManager->getItemKey("soda", "cup", index);
        item->setUserObject(__String::create(key));
        if(!LockUIManager::getInstance()->registerIAPLock(ShopScene::getAllFoodString(), "cup", index, item))
        {
            LockUIManager::getInstance()->registerLock("soda", "cup", index, item,Vec2(200,60));
        }
    }
    kAudioUtil->playEffect(kVoiceDecChooseCup);
    
    adapter->unitTouchEnded = [=](ui::Widget* item)
    {
        bool hasPurchased = kIAPManager->isPackagePurchased(ShopScene::getAllFoodString());
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
                            
                            this->showShop();
                        };
                    }
                        return;
                }
            }
        }
        scroll->setEnabled(false);
        
        auto soda = kESDataManager->getSodaFood();
        soda.cupIndex = item->getTag();
        kESDataManager->setSodaFood(soda);
        
        item->setVisible(false);
        int index = item->getTag();
        string cupBtmStr = StringUtils::format("res/rainbow_slushy/cup/cup%d_2.png",index);
        string cupTopStr = StringUtils::format("res/rainbow_slushy/cup/cup%d_1.png",index);
        
        _cup = BowlNode::create(cupBtmStr, cupTopStr);
        _cup->setScale(0.8f);
        auto startPos = scroll->getInnerContainer()->convertToWorldSpace(item->getPosition());
        _cup->setPosition(startPos);
        this->addToContentLayer(_cup,10);
        _cup->runAction(MoveTo::create(0.3f, CMVisibleRect::getPosition(146,437)));
        _cup->runAction(Sequence::create(ScaleTo::create(0.3f, 1.f),
                                         CallFunc::create([=]
                                                          {
                                                              scrollNode->removeFromParentAndCleanup(true);
                                                              pourJuice();
                                                          }),
                                         NULL));
    };
    
    scroll->show(nullptr);
}

void SodaBlendScene::pourJuice()
{
    auto juicefood = kESDataManager->getSodaFood();
    auto path = StringUtils::format("res/rainbow_slushy/cup/cup_mixture/cup_%d.png",juicefood.flavorIndex);
    auto juice = LiquidNode::create("res/rainbow_slushy/blender/blender1/blender1_down.png", "res/rainbow_slushy/blender/blender1/blende1_up.png","res/rainbow_slushy/blender/blender1/blender_mask.png",path);
    this->addToContentLayer(juice,3);
    juice->setLiquidPos(Vec2(-44, -100));
    juice->setRotateDegree(-55);
    juice->setDuration(12.f);
    
    string maskPath = "res/rainbow_slushy/cup/cup_mixture/shadow.png";
    _cup->initMaskNode(maskPath);
    
    float pourDuration = 8.f;
    
    auto call = [=]
    {
        kAudioUtil->playEffect(kVoiceJuicePour);
        
        TipsUIController::getInstance()->showMoveTipBezel(this, juice->getPosition(), CMVisibleRect::getPosition(266, 727));
        
        juice->onMoveToTargetCallback = [=]()
        {
            TipsUIController::getInstance()->removeAllTips();
            auto node = TipsUIController::getInstance()->showAccelerateTip(this, juice->convertToWorldSpaceAR(Vec2::ZERO) - Vec2(250.0, 0.0),false);
            node->setLocalZOrder(100);
        };
        juice->setPourWorldPoint(CMVisibleRect::getPosition(436, 737), [=](){
            
            kAudioUtil->playEffect(kSoundAddWater);
            
            auto particle = ParticleSystemQuad::create("particle/pourJuiceRight.plist");
            _cup->addContent(particle, "particle", 2,Vec2(79+35, 191+20));
            _cup->setContentVisible("particle");
            particle->setTexture(_director->getTextureCache()->addImage(StringUtils::format("particle/pa/%d.png",kESDataManager->getSodaFood().flavorIndex)));
            
            Sprite* juiceW = Sprite::create(path);
            _cup->addContentToMask(juiceW, "juice", 1);
            _cup->showContentInMask("juice",pourDuration);
            auto backCall = [=]
            {
                auto finishCAll = [=]
                {
                    SceneManager::getInstance()->replaceWithDefaultTransition(SodaDecScene::scene());
                };
                _cup->runAction(Sequence::create(  MoveTo::create(0.8f, CMVisibleRect::getPosition(320, 520)),
                                                 CallFunc::create([=]
                                                                  {
                                                                      kSoundInstance->playRandomStepFinishVoice();
                                                                      kAudioUtil->playEffect(kSoundShow);
                                                                      
                                                                      auto particle = ParticleSystemQuad::create("particle/wirteParticle.plist");
                                                                      particle->setPosition(CMVisibleRect::getCenterOfScene());
                                                                      this->addChild(particle,INT_MAX);
                                                                  }),
                                                 DelayTime::create(2.f),
                                                 CallFunc::create(finishCAll),
                                                 NULL));
                ActionHelper::moveOutofSceneAndRemoveSelf(juice, kActionDirectionRight);
            };
            
            this->runAction(Sequence::create(DelayTime::create(pourDuration),
                                             CallFunc::create([=]()
                                                              {
                                                                  TipsUIController::getInstance()->removeAllTips();
                                                                  particle->removeFromParent();
                                                                  juice->backToOriginal([=]()
                                                                                        {
                                                                                            backCall();
                                                                                        });
                                                              }),
                                             NULL));
        });
        
    };
    
    ActionHelper::moveInToSceneBounce(juice, CMVisibleRect::getPosition(553,396), kActionDirectionRight,call);
    
}

void SodaBlendScene::chooseStraw()
{
    
    _cup->runAction(Spawn::create(ScaleTo::create(0.5f, 0.8f),
                                  MoveTo::create(0.5f, CMVisibleRect::getPosition(320, 220)),
                                  NULL));
    
    this->onNextButtonTouchCallback = [=]
    {
        this->onNextButtonTouchCallback = nullptr;
        
        SceneManager::getInstance()->replaceWithDefaultTransition(SodaDecScene::scene());
    };
    
    
    DecorationConfig config = DecorationConfig(
                                               "rainbowcakepop",
                                               "stick",
                                               "",
                                               "",
                                               "res/rainbow_slushy/decoration/cup_straw/cup_straw%d.png",
                                               "res/rainbow_slushy/decoration/straw/straw%d.png",
                                               1,
                                               20
                                               );
    
    ScrollViewConfig sConfig = ScrollViewConfig
    (
     Size(getContentSize().width,280),
     "",
     "",
     CMVisibleRect::getPosition(320, 669+50),
     CMVisibleRect::getPosition(320, 669),
     194+20
     );
    
    auto callback = [=](ui::Widget* item)
    {
        //            setTypeEnable(false);
        _scrollView->setEnabled(false);
        int itemTag = item->getTag();
        log("===item clicked %d====",itemTag);
        auto food = kESDataManager->getSodaFood();
        food.strawIndex = itemTag;
        kESDataManager->setSodaFood(food);
        
        Vec2 pos = CMVisibleRect::getPosition(320, 346);
        
        auto changeCall = [=]
        {
            this->showNextButton();
            
            _scrollView->setEnabled(true);
            
            updateStraw();
            //            _corndog->setVisible(true);
            //            _corndog->updateStick();
            this->showParticle(_straw, Vec2::ZERO);
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
    };
    
    this->createScrollView(config,callback,sConfig,false);
}

void SodaBlendScene::updateStraw()
{
    if (!_straw)
    {
        _straw = Sprite::create();
        _cup->addChild(_straw,3);
        _straw->setScale(0.8f);
        _straw->setPosition(0, 30);
    }
    int index = kESDataManager->getSodaFood().strawIndex;
    auto path = StringUtils::format("res/rainbow_slushy/decoration/straw/straw%d.png",index);
    _straw->setTexture(path);
    
}


