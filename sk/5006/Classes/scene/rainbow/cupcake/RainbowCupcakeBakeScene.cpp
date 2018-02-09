//
//  RainbowCupcakeBakeScene.cpp
//  shake5008
//
//  Created by liji on 17/1/11.
//
//

#include "RainbowCupcakeBakeScene.h"
#include "CupCakeOvenPlateNode.h"
#include "OvenNode.h"
#include "SceneHeader.h"
REGIST_SECENE(RainbowCupcakeBakeScene)



RainbowCupcakeBakeScene::RainbowCupcakeBakeScene():
_plate(nullptr),
_oven(nullptr)
{

}

RainbowCupcakeBakeScene::~RainbowCupcakeBakeScene()
{

}

bool RainbowCupcakeBakeScene::init()
{
    if (!ESMakeBaseScene::init())
    {
        return false;
    }
    this->setBackground("res/rainbow_slushy/bg-2.png");
    kESDataManager->initRainbowCupcakeColor();
    initBake();
    return true;
}

void RainbowCupcakeBakeScene::initBake()
{
    if (!_plate)
    {
        _plate = CupCakeOvenPlateNode::create();
        this->addToContentLayer(_plate,10);
        _plate->setPosition(CMVisibleRect::getPosition(320, 220));
        _plate->showAllMixture();
    }
    
    _oven = OvenNode::create();
    this->addToContentLayer(_oven,5);
    _oven->setScale(0.9f);
    
    auto ovenInCall = [=]
    {
        _plate->retain();
        _plate->removeFromParent();
        _oven->addChild(_plate);
        _plate->setPosition(_oven->convertToNodeSpace(_plate->getPosition()));
        kAudioUtil->playEffect(kVoiceBake);
        _plate->runAction(Sequence::create(DelayTime::create(1.2f),
                                           Spawn::create(EaseBackIn::create(ScaleTo::create(1.2f, 0.7f)),
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

void RainbowCupcakeBakeScene::bake()
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
                                                                                     SceneManager::getInstance()->replaceWithDefaultTransition(RainbowCupcakeDecScene::scene());
                                                                                 }, CMVisibleRect::getCenterOfScene());
                                                          //        ;
                                                          
                                                      }),
                                     NULL));
}

