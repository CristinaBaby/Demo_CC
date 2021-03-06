//
//  SodaMakeScene.cpp
//  shake004
//
//  Created by liji on 16/10/11.
//
//

#include "SodaMakeScene.h"
#include "ComponentCycleScroll.h"
#include "ESDataManager.h"
#include "BowlNode.h"
#include "JuiceMachine.h"
#include "DialogLayer.h"
#include "ComponentTouchMove.h"
#include "SceneHeader.h"
#include "RewardManager.h"
#include "LockUIManager.h"
#include "MiniShopScene.h"
SodaMakeScene::SodaMakeScene():
_cup(nullptr),
_selectMachineWidget(nullptr),
_isAnimation(false),
_cupTop(nullptr)
{
    _steps.insert(std::make_pair("stepChooseCup", CC_CALLBACK_0(SodaMakeScene::stepChooseCup, this)));
    _steps.insert(std::make_pair("stepChooseFlavor", CC_CALLBACK_0(SodaMakeScene::stepChooseFlavor, this)));
    _steps.insert(std::make_pair("stepAddIce", CC_CALLBACK_0(SodaMakeScene::stepAddIce, this)));
    
    _stepName =
    {
        "stepChooseCup",
        "stepChooseFlavor",
        "stepAddIce",
    };
}

SodaMakeScene::~SodaMakeScene()
{

}

bool SodaMakeScene::init()
{
    if (!ESMakeBaseScene::init())
    {
        return false;
    }
//    _screenEventName = "food0.0_movienight_soda";

    
    this->setBackground("res/soda/soda_bg.png");
    
    this->showNextStep();
    return true;
}

void SodaMakeScene::stepChooseCup()
{
    
        auto _scrollNode = Node::create();
        _scrollNode->setPosition(CMVisibleRect::getPosition(320, 460));
        this->addToContentLayer(_scrollNode, 5, 7);
    
        auto scrollComp = ComponentCycleScroll::create();
        scrollComp->setEnableMoveItem(false);
        scrollComp->setCycleScrollSize(Size(getContentSize().width, 445));
        _scrollNode->addComponent(scrollComp);
        scrollComp->setMargin(252+30);
    
        int bunNumber = 15;
        for (int index=1; index<bunNumber+1; index++)
        {
            string cupBtmStr = "res/soda/cup/cup1_2.png";
            string cupTopStr = StringUtils::format("res/soda/cup/cup%d_1.png",index);
//            string lidBtmStr = "res/soda/cup/cup1_top2.png";
//            string lidTopStr = StringUtils::format("res/soda/cup/cup%d_top1.png",index);
            
            auto cup = BowlNode::create(cupBtmStr, cupTopStr);
//            auto lid = Sprite::create(lidBtmStr);
//            auto lidTop = Sprite::create(lidTopStr);
//            cup->addChild(lid);
//            lid->addChild(lidTop);
//            lidTop->setPosition(lid->getContentSize()/2);
            cup->setScale(0.8f);
            auto widget = Widget::create();
            widget->setContentSize(cup->getContentSize());
            cup->setPosition(widget->getContentSize()/2);
            widget->addChild(cup, 2);
            widget->setTag(index);
            scrollComp->pushCustomItem(widget);

            auto key = kRewardManager->getItemKey("soda", "cup", index);
            widget->setUserObject(__String::create(key));
            if(!LockUIManager::getInstance()->registerIAPLock(ShopScene::getAllContentString(), "cup", index, widget))
            {
                LockUIManager::getInstance()->registerLock("soda", "cup", index, widget,Vec2(220,50));
            }
        }
    
    scrollComp->itemClickCallback=[=](Widget *item, Touch *touch)
    {
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
        scrollComp->setEnabled(false);
        
        auto soda = kESDataManager->getSodaFood();
        soda.cupIndex = item->getTag();
        kESDataManager->setSodaFood(soda);
        
//        this->showNextButton();
//        this->onNextButtonTouchCallback = [=]
//        {
//            scrollComp->setEnabled(false);
//            this->fillPopcorn();
//            if (_scrollBG)
//            {
//                _scrollBG->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, 200)),
//                                                      RemoveSelf::create(),
//                                                      NULL));
//                _scrollBG = nullptr;
//            }
//        };
        item->setVisible(false);
        int index = item->getTag();
        string cupBtmStr = "res/soda/cup/cup1_2.png";
        string cupTopStr = StringUtils::format("res/soda/cup/cup%d_1.png",index);
        string lidBtmStr = "res/soda/cup/cup1_top2.png";
        string lidTopStr = StringUtils::format("res/soda/cup/cup%d_top1.png",index);
        
        _cup = BowlNode::create(cupBtmStr, cupTopStr);
//        auto lid = Sprite::create(lidBtmStr);
//        auto lidTop = Sprite::create(lidTopStr);
//        _cup->addChild(lid);
//        lid->addChild(lidTop);
//        lid->setName("lid");
//        lidTop->setPosition(lid->getContentSize()/2);
        _cup->setScale(0.8f);
        auto startPos = touch->getLocation();
        _cup->setPosition(startPos);
        this->addToContentLayer(_cup,10);
        _cup->runAction(MoveTo::create(0.3f, CMVisibleRect::getPosition(320, 364)));
        _cup->runAction(Sequence::create(ScaleTo::create(0.3f, 1.f),
                                        CallFunc::create([=]
        {
            _cupTop = Sprite::create(cupTopStr);
            this->addToContentLayer(_cupTop,100);
            _cupTop->setPosition(_cup->getPosition());
            _scrollNode->removeFromParentAndCleanup(true);
            this->showNextStep();
        }),
                                        NULL));
    };
    scrollComp->scrollBy(1.0, Vec2(-getContentSize().width, 0.0), nullptr);

    scrollComp->start();
}

void SodaMakeScene::stepChooseFlavor()
{
    
    kAudioUtil->playEffect(kVoiceJuiceChooseFlavor);

    if (!_cup)
    {
        string cupBtmStr = "res/soda/cup/cup1_2.png";
        string cupTopStr = StringUtils::format("res/soda/cup/cup%d_1.png",1);
        string lidBtmStr = "res/soda/cup/cup1_top2.png";
        string lidTopStr = StringUtils::format("res/soda/cup/cup%d_top1.png",1);
        
        _cup = BowlNode::create(cupBtmStr, cupTopStr);
//        auto lid = Sprite::create(lidBtmStr);
//        auto lidTop = Sprite::create(lidTopStr);
//        _cup->addChild(lid);
//        lid->addChild(lidTop);
//        lid->setName("lid");
//        lidTop->setPosition(lid->getContentSize()/2);
        _cup->setScale(1.f);
        _cup->setPosition(CMVisibleRect::getPosition(320, 364));
        this->addToContentLayer(_cup,10);
    }
    
//    ActionHelper::moveOutofSceneAndRemoveSelf(_cup->getChildByName("lid"), kActionDirectionLeft);

    auto scrollNode = Node::create();
    scrollNode->setPosition(CMVisibleRect::getPosition(320, getContentSize().height+200));
    addToContentLayer(scrollNode, 11);
    
    auto scrollComp = ComponentCycleScroll::create();
    scrollComp->setEnableMoveItem(false);
    scrollComp->setCycleScrollSize(Size(getContentSize().width*2, getContentSize().height + 100.0));
    scrollComp->setMargin(300.0);
    for (int index = 1; index <= 9; ++index)
    {
        auto juiceMachine = JuiceMachine::create(index);
        juiceMachine->setPosition(juiceMachine->getContentSize()/2);
        
        auto widget = Widget::create();
        widget->addChild(juiceMachine, 1);
        widget->setContentSize(Size(juiceMachine->getContentSize().width, juiceMachine->getContentSize().height));
        widget->setTag(index);
        scrollComp->pushCustomItem(widget);
        
        auto key = kRewardManager->getItemKey("soda", "flavor", index);
        widget->setUserObject(__String::create(key));
        if(!LockUIManager::getInstance()->registerIAPLock(ShopScene::getAllContentString(), "flavor", index, widget))
        {
            LockUIManager::getInstance()->registerLock("soda", "flavor", index, widget);
        }
    }
//    LockUIManager::getInstance()->updateLockStatus();
    scrollNode->addComponent(scrollComp);
    scrollComp->start();
    scrollNode->runAction(Sequence::create(MoveTo::create(0.5, Vec2(getContentSize().width/2, getContentSize().height - 70.0)), CallFunc::create([=](){
        
        scrollComp->scrollBy(1.0, Vec2(-getContentSize().width, 0.0), nullptr);
    }), NULL));
    scrollComp->itemClickCallback = [=](Widget *item, Touch *touch)
    {
        if (_isAnimation ||
            _selectMachineWidget == item)
            return;
        
        kAudioUtil->playEffect("Open and close door.mp3");
        
        bool hasPurchased = kIAPManager->isPackagePurchased(ShopScene::getAllContentString());
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
                        kRewardManager->showRewardAds(key);
                    return;
                }
            }
        }
        
        auto machine = dynamic_cast<JuiceMachine*>(item->getChildren().at(0));
        machine->pourJuiceCallback = [=]()
        {
            kAudioUtil->playEffect("Open and close door.mp3");
            kAudioUtil->playEffect("Pouring soda.mp3");
            
            scrollComp->stop();
            
            _cup->initMaskNode("res/soda/cup_soda/cup_soda_shadow.png");
            auto inCup =  Sprite::create(StringUtils::format("res/soda/cup_soda/cup_soda%d.png",kESDataManager->getSodaFood().flavorIndex));
            _cup->addContentToMask(inCup, "inCup", 2);
            inCup->setPosition(Vec2(0, -inCup->getContentSize().height));
            inCup->runAction(MoveTo::create(5.f, Vec2(0,0 )));
            
            this->runAction(Sequence::create(DelayTime::create(4.f), CallFunc::create([=](){
                
                kAudioUtil->stopAllEffect();
                this->juiceMachineUnSelect();
            }), DelayTime::create(1.0), CallFunc::create([=](){
                
                scrollNode->runAction(Sequence::create(MoveBy::create(1.0, Vec2(0.0, 500.0)), RemoveSelf::create(), NULL));
            }), DelayTime::create(1.0), CallFunc::create([=](){
                
                this->showNextStep();
            }), NULL));
        };
        
        auto selectCall = [=]()
        {
            auto position = item->convertToWorldSpaceAR(Vec2::ZERO);
            CCLOG("%f %f", position.x, position.y);
            scrollComp->scrollBy(0.3, Vec2(getContentSize().width/2 - position.x, 0.0), nullptr);
            
            juiceMachineSelect(item);
        };
        
        if (_selectMachineWidget){
            juiceMachineUnSelect();
        }
        
        selectCall();
    };
    scrollComp->scrollMoveCallback = [=]()
    {
        //滑动的时候，让选中的机器回到原始位置
        if (_isAnimation || !_selectMachineWidget)
            return;
        
        //检测选中的机器位置是否在正中，如果在，则不将机器缩回去
        auto position = _selectMachineWidget->convertToWorldSpaceAR(Vec2::ZERO);
        if (std::abs(position.x - 320) < 10.0)
            return;
        
        dynamic_cast<JuiceMachine*>(_selectMachineWidget->getChildren().at(0))->onOffAction(false);
        juiceMachineUnSelect();
    };
}

void SodaMakeScene::juiceMachineSelect(Widget *widget)
{
    _isAnimation = true;
    auto soda = kESDataManager->getSodaFood();
    soda.flavorIndex = widget->getTag();
    kESDataManager->setSodaFood(soda);

    widget->runAction(ScaleTo::create(0.2f, 1.05));
    widget->runAction(Sequence::create(MoveBy::create(0.3, Vec2(0.0, -90)), CallFunc::create([=](){
        
        _isAnimation = false;
        dynamic_cast<JuiceMachine*>(widget->getChildren().at(0))->setMachineSelect(true);
        _selectMachineWidget = widget;
    }), NULL));
}

void SodaMakeScene::juiceMachineUnSelect()
{
    _isAnimation = true;
    _selectMachineWidget->runAction(ScaleTo::create(0.2f, 1.f));
    _selectMachineWidget->runAction(Sequence::create(MoveBy::create(0.27, Vec2(0.0, 90)), CallFunc::create([=](){
        
        dynamic_cast<JuiceMachine*>(_selectMachineWidget->getChildren().at(0))->onOffAction(false);
        dynamic_cast<JuiceMachine*>(_selectMachineWidget->getChildren().at(0))->setMachineSelect(false);
        _selectMachineWidget = nullptr;
        _isAnimation = false;
    }), NULL));
}

void SodaMakeScene::stepAddIce()
{
    kAudioUtil->playEffect(kVoiceJuiceAddIce);

    if (_cupTop)
    {
        _cupTop->removeFromParent();
    }
    
    
    auto dialog = DialogLayer::showWithMessage("", CC_CALLBACK_0(SodaMakeScene::addIce,this), []
    {
//        SceneManager::getInstance()->replaceWithDefaultTransition(SodaDecScene::scene());
    }, this);
    dialog->updateBg("ui/popup/tips_ice.png");
    
}

void SodaMakeScene::addIce()
{
    vector<Vec2> icePosVec =
    {
        Vec2(-38, -141),
        Vec2(-21, -164),
        Vec2(20, -167),
        Vec2(-49, -105),
        Vec2(0, -110),
        Vec2(44, -124),
    };
    
    auto scrollBG = Sprite::create("ui/dec/bg.png");
    scrollBG->setAnchorPoint(Vec2(0.5,1));
    this->addToUILayer(scrollBG,2);
    scrollBG->setPosition(CMVisibleRect::getPosition(320,130,kBorderNone,kBorderTop));
    
    for (int i = 1;i<5;i++)
    {
        auto icon = Sprite::create(StringUtils::format("res/soda/ice/ice%d.png",i));
        scrollBG->addChild(icon);
        icon->setTag(i);
        icon->setPosition(Vec2(56+81/2+(81+56)*(i-1), scrollBG->getContentSize().height/2));
        
        auto key = kRewardManager->getItemKey("soda", "ice", i);
        icon->setUserObject(__String::create(key));
        if(!LockUIManager::getInstance()->registerIAPLock(ShopScene::getAllContentString(), "ice", i, icon))
        {
            LockUIManager::getInstance()->registerLock("soda", "ice", i, icon);
        }
        
        auto cTouch = ComponentTouchMove::create();
        cTouch->setIsCanMove(false);
        icon->addComponent(cTouch);
        cTouch->touchEndedCall=[=](Touch* t)
        {
            bool hasPurchased = kIAPManager->isPackagePurchased(ShopScene::getAllContentString());
            if (!hasPurchased)
            {
                if (kIAPManager->isFree("ice", icon->getTag()))
                {
                    auto keyObject = icon->getUserObject();
                    if(!keyObject)
                        return;
                    string key = ((__String*)(keyObject))->getCString();
                    if(kRewardManager->isLocked(key))
                    {
                        //如果能找到reward名字的lock，显示reward，否则显示iap
                        if (icon->getChildByName(LOCK_NAME_REWARD))
                            kRewardManager->showRewardAds(key);
                        return;
                    }
                }
            }

            
            _cup->runAction(MoveTo::create(0.3f, CMVisibleRect::getPosition(181, 437)));
            auto food = kESDataManager->getSodaFood();
            food.iceIndex = icon->getTag();
            kESDataManager->setSodaFood(food);
            cTouch->setEnabled(false);

            auto iceBowl = BowlNode::create("res/soda/ice/ice_bowl2.png", "res/soda/ice/ice_bow1.png");
            this->addToContentLayer(iceBowl,10);
            
            for (int i = 0; i<6; i++)
            {
                auto iceInBowl = Sprite::create(StringUtils::format("res/soda/ice/ice%d.png",kESDataManager->getSodaFood().iceIndex));
                Vec2 pos = Vec2(-40+rand()%(67+10), -8+rand()%(68-8));
                iceInBowl->setRotation(rand()%360);
                iceInBowl->setScale(rand()%3*0.1+0.8);
                iceInBowl->setTag(i);
                iceBowl->addContent(iceInBowl, StringUtils::format("iceInBowl%d",i),1,pos);
                _iceVec.push_back(iceInBowl);
                iceInBowl->setOpacity(255);
                //            this->runAction(Sequence::create(DelayTime::create(0.1f+i*0.1f),
                //                                             CallFunc::create([=]
                //                                                              {
                //                                                                  iceBowl->setContentVisible(StringUtils::format("fruitInbowl%d",i));
                //                                                              }),
                //                                             NULL));
                
            }
            ActionHelper::moveInToSceneBounce(iceBowl, CMVisibleRect::getPosition(500, 234), kActionDirectionRight,[=]
            {
                iceBowl->setPourWorldPoint(CMVisibleRect::getPosition(363,594), [=]
                                            {
                                                TipsUIController::getInstance()->showAccelerateTip(this, iceBowl->getPosition(),true);
                                                
                                                iceBowl->beginPour(-45.0, [=](){
                                                    
                                                    for (auto ice : _iceVec)
                                                    {
                                                        ice->runAction(Sequence::create(DelayTime::create(0.15*ice->getTag()),
                                                                                        EaseBackIn::create(ScaleTo::create(0.2f, 0)),
                                                                                        CallFunc::create([=]
                                                        {
                                                            auto iceInBowl = Sprite::create(StringUtils::format("res/soda/ice/ice%d.png",kESDataManager->getSodaFood().iceIndex));
                                                            iceInBowl->setRotation(rand()%360);
                                                            iceInBowl->setScale(rand()%3*0.1+0.8);
                                                            _cup->addContentToMask(iceInBowl, "iceInCup",10,icePosVec.at(ice->getTag())+Vec2(0,60));
                                                            iceInBowl->runAction(MoveBy::create(0.2f, Vec2(0, -60)));
                                                        }),
                                                                                        NULL));
                                                    }
                                                    
                                                    kAudioUtil->playEffect(kSoundAddIce);
                                                    TipsUIController::getInstance()->removeAccelerateTip();
                                                    
                                                    auto particle = ParticleSystemQuad::create("particle/cornParticle.plist");
                                                    particle->setPosition(iceBowl->getPosition() + Vec2(-iceBowl->getContentSize().width/2, -50.0));
                                                    this->addToContentLayer(particle, 10);
                                                    particle->setTexture(_director->getTextureCache()->addImage(StringUtils::format("res/soda/ice/ice%d.png",kESDataManager->getSodaFood().iceIndex)));
                                                    
                                                    
                                                    
                                                    iceBowl->runAction(Sequence::create(RotateTo::create(0.15*6, 0.0),
                                                                                        MoveBy::create(0.3, Vec2(800.0, 0.0)),
                                                                                        CallFunc::create([=]()
                                                    {
                                                        _cup->runAction(Sequence::create(MoveTo::create(0.8f, CMVisibleRect::getCenterOfScene()),
                                                                                         EaseBackOut::create(ScaleTo::create(0.4f,1.1)),
                                                                                         EaseBackIn::create(ScaleTo::create(0.4f,1)),
                                                                                         CallFunc::create([=]
                                                        {
                                                            this->showNextButton();
                                                            this->onNextButtonTouchCallback=[]
                                                            {
//                                                                SceneManager::getInstance()->replaceWithDefaultTransition(SodaDecScene::scene());
                                                            };
                                                        }),
                                                                                         NULL));
                                                        kSoundInstance->playRandomStepFinishVoice();
                                                        
                                                    }), RemoveSelf::create(), NULL));
                                                },true);
                                            });
            });
            scrollBG->setVisible(false);

        };

    }
}


