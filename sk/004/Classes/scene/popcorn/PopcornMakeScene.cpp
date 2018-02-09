//
//  PopcornMakeScene.cpp
//  shake004
//
//  Created by liji on 16/10/24.
//
//

#include "PopcornMakeScene.h"
#include "ComponentTouchMove.h"
#include "ComponentTouchMoveToTarget.h"
#include "TapDropNode.h"
#include "SaltNode.h"
#include "ScoopSpoon.h"
#include "SceneHeader.h"
PopcornMakeScene::PopcornMakeScene():
_cornHeightInBowl(300),
_bowl(nullptr),
_currentMakingCornNode(nullptr),
_manualMachineNode(nullptr)
{

}

PopcornMakeScene::~PopcornMakeScene()
{

}

bool PopcornMakeScene::init()
{
    if (!ESMakeBaseScene::init())
    {
        return false;
    }
    this->setBackground("res/make_bg.png");
    _screenEventName = "food0.0_movienight_popcorn";
    
    showCorn();
//    showManualMachine();
    return true;
}

void PopcornMakeScene::showCorn()
{
    
    kAudioUtil->playEffect(kVoicePopcornCollect);
    _bowl = BowlNode::create("res/popcorn/1/bowl1_2.png", "res/popcorn/1/bowl1_1.png");
    this->addToContentLayer(_bowl, 3);
    
    auto call = [=]
    {
        initCorn();
        initTouchMakeCorn();
    };
    ActionHelper::moveInToSceneBounce(_bowl, CMVisibleRect::getCenterOfScene(), kActionDirectionLeft, call);
}


void PopcornMakeScene::initCorn()
{
    auto createCorn = [=](const Vec2& position) -> Node*
    {
        auto cornNode = CSLoader::createNode("ui/res/popcorn.csb");
        cornNode->setScale(0.6);
        cornNode->setRotation(-42);
        this->addToContentLayer(cornNode, 3);
        ActionHelper::moveInToSceneExponentialOut(cornNode, position, ActionDirection::kActionDirectionRight);
        return cornNode;
    };
    
    _cornNodesVector.pushBack(createCorn(CMVisibleRect::getPosition(462, 215)));
    _cornNodesVector.pushBack(createCorn(CMVisibleRect::getPosition(294, 215)));
    _cornNodesVector.pushBack(createCorn(CMVisibleRect::getPosition(130, 215)));
    
    this->runAction(Sequence::create(DelayTime::create(1.0), CallFunc::create([=](){
        
        this->makeCorn(*(_cornNodesVector.end() - 1));
    }), NULL));
}

void PopcornMakeScene::initTouchMakeCorn()
{
    auto moveComp = ComponentTouchMove::create();
    this->addComponent(moveComp);
    moveComp->setIsCanMove(false);
    moveComp->touchBeganCall = [=](Touch* touch)
    {
        SoundConfig::getInstance()->playLoopEffect("Spill corn kernal.mp3");
        TipsUIController::getInstance()->removeMoveTip();
    };
    moveComp->touchEndedCall = [=](Touch* touch)
    {
        SoundConfig::getInstance()->stopLoopEffect();
    };
    moveComp->touchCancelledCall = [=](Touch* touch)
    {
        SoundConfig::getInstance()->stopLoopEffect();
    };
    moveComp->touchMovedCall = [=](Touch* touch)
    {
        if (!_currentMakingCornNode)
            return;
        
        for (auto corn : _currentMakingCornNode->getChildren())
        {
            if (corn->getContentSize().width > 100.0)
                continue;
            
            if (corn->getBoundingBox().containsPoint(_currentMakingCornNode->convertToNodeSpaceAR(touch->getLocation())))
            {
                corn->retain();
                corn->setScale(1.1);
                auto position = _bowl->convertToNodeSpaceAR(corn->convertToWorldSpaceAR(Vec2::ZERO));
                corn->removeFromParent();
//                _cornClippingNode->addChild(corn);
                _bowl->addContent(corn, "corn", 10);
                corn->setOpacity(255);
                log("%f,%f",position.x,position.y);
                corn->setPosition(position);
                corn->release();
                
                float y = cocos2d::random(_cornHeightInBowl, float(_cornHeightInBowl + 50));
                corn->runAction(MoveBy::create(0.3, Vec2(0.0, -y)));
            }
        }
        
        //最后一个玉米核
        if (_currentMakingCornNode->getChildren().size() == 1)
        {
            ActionHelper::moveOutofSceneAndRemoveSelf(_currentMakingCornNode, ActionDirection::kActionDirectionLeft, [=](){
                
                _cornNodesVector.erase(_cornNodesVector.end() - 1);
                if (_cornNodesVector.size() == 0)
                {
                    SoundConfig::getInstance()->stopLoopEffect();
                    moveComp->setEnabled(false);
                    _currentMakingCornNode = nullptr;
//                    this->showNextButton();
                    kSoundInstance->playRandomStepFinishVoice();
                    ActionHelper::moveOutofSceneAndRemoveSelf(_bowl, kActionDirectionLeft,[=]
                    {
                    showManualMachine();
                    });
                    
                    return;
                }
                
                this->makeCorn(*(_cornNodesVector.end() - 1));
            });
        }
    };
    moveComp->start();
}

void PopcornMakeScene::makeCorn(Node *cornNode)
{
    _currentMakingCornNode = nullptr;
    _cornHeightInBowl -= 30.0;
    
    auto pos = _bowl->getPosition() + Vec2(0, 320.0);
    TipsUIController::getInstance()->showMoveTip(this, cornNode->getPosition(), pos);
    
    auto moveToComp = ComponentTouchMoveToTarget::create(pos);
    cornNode->addComponent(moveToComp);
    moveToComp->start();
    moveToComp->setMaxDistanceForMoveToTarget(500);
    moveToComp->touchBeganCall = [=](Touch *touch)
    {
        kAudioUtil->playEffect("Drag corn.mp3");
        TipsUIController::getInstance()->removeMoveTip();
    };
    moveToComp->componentTouchMoveToTargetDoneCall = [=]()
    {
        if (!_once)
            kAudioUtil->playEffect("popcorn1.mp3");
        _once = true;
        
        cornNode->setLocalZOrder(1);
        cornNode->runAction(ScaleTo::create(0.2, 1.0));
        cornNode->runAction(RotateTo::create(0.2, 0.f));
        TipsUIController::getInstance()->showMoveTip(this, cornNode->getPosition() + Vec2(150.0, 0.0), cornNode->getPosition() + Vec2(-150.0, 0.0));
        _currentMakingCornNode = cornNode;
    };
}


void PopcornMakeScene::showManualMachine()
{

    _manualMachineNode = CSLoader::createNode("popcornMachine.csb");
    addToContentLayer(_manualMachineNode, 1);
    
    auto door = Helper::seekNodeByName(_manualMachineNode, "door1_7");
    door->setLocalZOrder(11);
    
    ActionHelper::moveInToSceneExponentialOut(_manualMachineNode, CMVisibleRect::getPosition(320, 580), ActionDirection::kActionDirectionLeft, [=](){
        
        TipsUIController::getInstance()->showClickTip(this, CMVisibleRect::getPosition(320, 520));
        
        auto clickComp = ComponentTouchMove::create();
        clickComp->setIsCanMove(false);
        door->addComponent(clickComp);
        clickComp->start();
        clickComp->touchClickedCall = [=](Touch*)
        {
            clickComp->setEnabled(false);
            
            kAudioUtil->playEffect("Open and close door.mp3");
            TipsUIController::getInstance()->removeClickTip();
            OrbitCamera* orbit = OrbitCamera::create(1.0, 0.3, 0.0, 0.0, -180.0, 0.0, 10.0);
            door->runAction(Sequence::create(EaseBackOut::create(orbit), CallFunc::create([=](){
                auto cover = Helper::seekNodeByName(_manualMachineNode, "pot1_6");
//                cover->setLocalZOrder(11);
                dynamic_cast<Sprite*>(cover)->setTexture("res/popcorn/2/pot2.png");
                manualMachineShowOil();
            }), NULL));
        };
    });
}

void PopcornMakeScene::manualMachineShowOil()
{
    kAudioUtil->playEffect(kVoicePopcornPourIntoMachine);

    
    kAudioUtil->playEffect("Tools slide in and out.mp3");
    
    auto oil = TapDropNode::create("res/popcorn/3/olive_oil.png");
    addToContentLayer(oil, 26);
    
    ActionHelper::moveInToSceneExponentialOut(oil, CMVisibleRect::getPosition(433, 289), ActionDirection::kActionDirectionRight, [=](){
        
        oil->setPourWorldPoint(CMVisibleRect::getPosition(511, 793));
        
        oil->rotateDoneCall = [=]
        {
            TipsUIController::getInstance()->showClickTip(this, oil->convertToWorldSpaceAR(Vec2(0.0, -50.0)));
        };
        
        oil->dropShowCall = [=]
        {
            SoundConfig::getInstance()->stopLoopEffect();
            
            TipsUIController::getInstance()->removeAllTips();
            auto drop = Sprite::create("res/popcorn/3/olive_oi1.png");
            _manualMachineNode->addChild(drop, 2);
            auto position = _manualMachineNode->convertToNodeSpaceAR(oil->getPosition() + Vec2(-150.0, -100.0));
            drop->setPosition(position);
            drop->runAction(Repeat::create(Sequence::create(CallFunc::create([]{
                kAudioUtil->playEffect("Water drop.mp3");}),
                                                            Place::create(position),
                                                            FadeIn::create(0),
                                                            MoveBy::create(0.3, Vec2(0, -40)),
                                                            FadeOut::create(0.3),
                                                            NULL), 1));
            
        };
        oil->dropDoneCall = [=]()
        {
            oil->backToOriginal([=](){
                ActionHelper::moveOutofSceneAndRemoveSelf(oil, ActionDirection::kActionDirectionRight, [=](){
                    
                    manualMachineShowCorn();
                });
            });
        };
    });
}

void PopcornMakeScene::manualMachineShowCorn()
{
    kAudioUtil->playEffect("Tools slide in and out.mp3");
    
    auto bowl = BowlNode::create("res/popcorn/2/bowl2_3.png", "res/popcorn/2/bowl2_1.png");
    auto corn = bowl->addContent("res/popcorn/2/bowl2_corn.png", "corn", 2);
    corn->setPosition(Vec2(0.0, 0));
    corn->setOpacity(255);
    this->addToContentLayer(bowl, 2);
    
    ActionHelper::moveInToSceneExponentialOut(bowl, CMVisibleRect::getPosition(152,244), ActionDirection::kActionDirectionRight, [=](){
        TipsUIController::getInstance()->showMoveTip(this, bowl->getPosition(), CMVisibleRect::getPosition(475, 781));

        bowl->setPourWorldPoint(CMVisibleRect::getPosition(560, 780), [=]
        {
            TipsUIController::getInstance()->removeAllTips();
            TipsUIController::getInstance()->showAccelerateTip(this, bowl->getPosition(),true);

            bowl->beginPour(-45.0, [=](){
                
                kAudioUtil->playEffect("Pour corn kernal.mp3");
                TipsUIController::getInstance()->removeAccelerateTip();
                
                auto particle = ParticleSystemQuad::create("particle/cornParticle.plist");
                particle->setPosition(bowl->getPosition() + Vec2(-bowl->getContentSize().width/2, -50.0));
                this->addToContentLayer(particle, 10);
                
                corn->runAction(Sequence::create(ScaleTo::create(1.0, 0.0), CallFunc::create([=](){
                    
                    bowl->runAction(Sequence::create(RotateTo::create(0.3, 0.0), MoveBy::create(0.3, Vec2(400.0, 0.0)), CallFunc::create([=](){
                        
                        kSoundInstance->playRandomStepFinishVoice();
                        this->manualMachineShowSalt();
                    }), RemoveSelf::create(), NULL));
                }), NULL));
            });
        });
    });
}

void PopcornMakeScene::manualMachineShowSalt()
{
    kAudioUtil->playEffect("Tools slide in and out.mp3");
    
    auto salt = SaltNode::create("res/popcorn/3/salt.png");
    this->addToContentLayer(salt, 10);
    ActionHelper::moveInToSceneExponentialOut(salt, CMVisibleRect::getPosition(553, 241), ActionDirection::kActionDirectionRight, [=](){
        salt->setPourWorldPoint(CMVisibleRect::getPosition(475, 781), [=]
        {
            TipsUIController::getInstance()->removeAllTips();
            auto par = ParticleSystemQuad::create("particle/saltParticle.plist");
            this->addToContentLayer(par, -1);
            par->setPosition(salt->convertToWorldSpace(Vec2(40.0, 80.0)));
            
            salt->runAction(Sequence::create(DelayTime::create(1.5), MoveBy::create(0.3, Vec2(400.0, 0.0)), CallFunc::create(CC_CALLBACK_0(PopcornMakeScene::manualMachineFinishMake, this)), NULL));
        }, nullptr);
        
        TipsUIController::getInstance()->showMoveTip(this, salt->getPosition(), CMVisibleRect::getPosition(475, 781));
    });
}

void PopcornMakeScene::manualMachineFinishMake()
{
    auto cover = Helper::seekNodeByName(_manualMachineNode, "pot1_6");
//    auto bucket = Helper::seekNodeByName(_manualMachineNode, "Node_1");
    auto door = Helper::seekNodeByName(_manualMachineNode, "door1_7");
    dynamic_cast<Sprite*>(cover)->setTexture("res/popcorn/2/pot1.png");
            kAudioUtil->playEffect("Open and close door.mp3");
            
            OrbitCamera* orbit = OrbitCamera::create(1.0, 0.3, 0.0, -180.0, 180.0, 0.0, 10.0);
            door->setLocalZOrder(10);
            door->runAction(Sequence::create(EaseBackOut::create(orbit), CallFunc::create([=](){
                
                auto checkBox = dynamic_cast<CheckBox*>(Helper::seekNodeByName(_manualMachineNode, "CheckBox_1"));
                checkBox->setTouchEnabled(true);
                kAudioUtil->playEffect(kVoicePopcornPressButton);

                TipsUIController::getInstance()->showClickTip(this, checkBox->convertToWorldSpaceAR(Vec2::ZERO)+Vec2(50, -50));
                checkBox->addEventListener([=](Ref*,CheckBox::EventType type){
                    
                    kAudioUtil->playEffect(kSoundClickButton);
                    
                    checkBox->setTouchEnabled(false);
                    TipsUIController::getInstance()->removeClickTip();
                    
                    auto turn = dynamic_cast<Sprite*>(Helper::seekNodeByName(_manualMachineNode, "pot_0_3"));
                    turn->runAction(RepeatForever::create(Sequence::create(FlipX::create(true),
                                                                           DelayTime::create(0.1f),
                                                                           FlipX::create(false),
                                                                           DelayTime::create(0.1f),
                                                                           NULL)));
//                    Helper::seekNodeByName(_manualMachineNode, "drawer_close_5")->setLocalZOrder(10);
                    
                    this->runAction(Sequence::create(DelayTime::create(0.2), CallFunc::create([=](){
                        
                        kAudioUtil->stopAllEffect();
                        SoundConfig::getInstance()->playLoopEffect("Popcorn maker running.mp3");
                        
                    }), DelayTime::create(1.0), CallFunc::create([=](){
                        
                        auto particle = ParticleSystemQuad::create("particle/popCorn.plist");
                        particle->setPosition(turn->getPosition() + Vec2(0.0, 80.0));
                        turn->getParent()->addChild(particle, 5);
                    }), DelayTime::create(3.0), CallFunc::create([=](){
                        
                        auto cornContent = Helper::seekNodeByName(_manualMachineNode, "drawer_popcorn1_24");
                        cornContent->setVisible(true);
                        cornContent->runAction(Sequence::create(ActionHelper::createAnimate("res/popcorn/4/drawer_popcorn%d.png", 1, 4,0.8f), CallFunc::create([=](){
                            
                            SoundConfig::getInstance()->stopLoopEffect();
                            turn->getActionManager()->removeAllActionsFromTarget(turn);
                            checkBox->setSelected(false);
                            
                            auto handle = Helper::seekNodeByName(_manualMachineNode, "handle0_5");
                            TipsUIController::getInstance()->showMoveTip(this, handle->convertToWorldSpaceAR(Vec2::ZERO) + Vec2(20, -90.0), handle->convertToWorldSpaceAR(Vec2::ZERO) + Vec2(20, 0.0));
                            auto comp = ComponentTouchMove::create();
                            comp->setIsCanMove(false);
                            handle->addComponent(comp);
                            comp->start();
                            comp->touchBeganCall = [=](Touch *touch)
                            {
                                TipsUIController::getInstance()->removeMoveTip();
                            };
                            comp->touchMovedCall = [=](Touch *touch)
                            {
                                auto p1 = touch->getStartLocation();
                                auto p2 = touch->getLocation();
                                if (p2.y - p1.y > 30.0 && fabsf(p1.x - p2.x) < 20.0)
                                {
                                    kAudioUtil->playEffect("75 Fabulous.mp3");
                                    kAudioUtil->playEffect("Open and close door.mp3");
                                    
                                    comp->setEnabled(false);
                                    this->manualMachineZoomIn();
                                }
                            };
                        }), NULL));
                    }), NULL));
                });
    }), NULL));
}

void PopcornMakeScene::manualMachineZoomIn()
{
    auto handle = Helper::seekNodeByName(_manualMachineNode, "handle0_5");
    auto turn = dynamic_cast<Sprite*>(Helper::seekNodeByName(_manualMachineNode, "pot_0_3"));
    auto door = Helper::seekNodeByName(_manualMachineNode, "door1_7");
    
    handle->runAction(Sequence::create(ScaleTo::create(0.3, 1.0, -1.0), CallFunc::create([=](){
        
        kAudioUtil->playEffect("Spill corn kernal.mp3");
        
        auto particle = ParticleSystemQuad::create("particle/popCornDrop.plist");
        particle->setPosition(turn->getPosition() + Vec2(0.0, 80.0));
        turn->getParent()->addChild(particle, 5);
    }), DelayTime::create(2.5), CallFunc::create([=](){
        
        OrbitCamera* orbit = OrbitCamera::create(1.0, 0.3, 0.0, 0.0, -180.0, 0.0, 10.0);
        door->runAction(orbit);
        kAudioUtil->playEffect("Open and close door.mp3");
        
        Helper::seekNodeByName(_manualMachineNode, "drawer_open1_10")->setVisible(true);
        Helper::seekNodeByName(_manualMachineNode, "drawer_open1_10")->runAction(FadeIn::create(1.0));
        
        _manualMachineNode->runAction(Sequence::create(Spawn::create(ScaleTo::create(1.0, 1.2), MoveBy::create(1.0, Vec2(-150.0, 220.0)), CallFunc::create([=](){
            
            kAudioUtil->playEffect("Tools slide in and out.mp3");
            kAudioUtil->playEffect(kVoicePopcornScoopIntoBowl);
            
            auto bowl = BowlNode::create("res/popcorn/4/bowl1_2.png", "res/popcorn/4/bowl1_1.png");
            auto corn = bowl->addContent("res/popcorn/4/drawer_popcorn1.png", "corn", 2);
//            corn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            corn->setPosition(Vec2(0.0, 0));
            corn->setOpacity(255);
            corn->setScale(0.8f);
            corn->setVisible(false);
            
            this->addToContentLayer(bowl, 10);
            ActionHelper::moveInToSceneExponentialOut(bowl, CMVisibleRect::getPosition(470, 255), ActionDirection::kActionDirectionRight);
            auto spoon = ScoopSpoon::create("res/popcorn/4/spoon1.png");
            spoon->setContent("res/popcorn/4/spoon1_popcorn.png",Vec2::ZERO);
            this->addToContentLayer(spoon, 11);
            spoon->setMoveContentOffset(Vec2(-50.0, -50.0));
            ActionHelper::moveInToSceneExponentialOut(spoon, CMVisibleRect::getPosition(470, 255+130), ActionDirection::kActionDirectionRight, [=](){
                
                kAudioUtil->playEffect("popcorn4.mp3");
                
                auto dragCall = [=]()
                {
                    spoon->startDragToWorldPositon(CMVisibleRect::getPosition(248,645+100));
                };
                
                spoon->onFinishPourCallback = [=]()
                {
                    corn->setVisible(true);
                    kAudioUtil->playEffect("Spill corn kernal.mp3");
                    int index = _scoopTimes+++1;
                    string str = StringUtils::format("res/popcorn/4/drawer_popcorn%d.png",index);
                    if (index == 4)
                    {
                        str = "res/popcorn/6/1.png";
                    }
                    dynamic_cast<Sprite*>(corn)->setTexture(str);
                };
                spoon->backToOriginalPositionCallback = [=]()
                {
                    if (_scoopTimes > 3)
                    {
                        ActionHelper::moveOutofSceneAndRemoveSelf(spoon, ActionDirection::kActionDirectionRight);
                        ActionHelper::moveOutofSceneAndRemoveSelf(_manualMachineNode, ActionDirection::kActionDirectionRight);
                        bowl->runAction(Sequence::create(MoveTo::create(0.8f, CMVisibleRect::getCenterOfScene()),
                                                         EaseBackOut::create(ScaleTo::create(0.4f,1.1)),
                                                         EaseBackIn::create(ScaleTo::create(0.4f,1)),
                                                         CallFunc::create([=]
                        {
                            kSoundInstance->playRandomStepFinishVoice();
                            this->showNextButton();
                            this->onNextButtonTouchCallback=[=]
                            {
                                SceneManager::getInstance()->replaceWithDefaultTransition(PopcornColorMakeScene::scene());
                            };
                        }),
                                                         NULL));
                        return;
                    }
                    dragCall();
                };
                dragCall();
            });
        }), NULL),
                                      NULL));
    }), NULL));
}


