//
//  SodaShareScene.cpp
//  shake004
//
//  Created by liji on 16/10/31.
//
//

#include "SodaShareScene.h"
#include "ESDataManager.h"
#include "JuiceBottleNode.h"
#include "SceneHeader.h"
#include "Charactor.h"
SodaShareScene::SodaShareScene():
_npc(nullptr),
_walkOut(false),
_allCoinGet(false)
{
}

SodaShareScene::~SodaShareScene()
{
}

bool SodaShareScene::init()
{
    if (!ShareBaseScene::init())
    {
        return false;
    }
    this->initFood();
    
    
    return true;
}

void SodaShareScene::initFood()
{
    auto food = JuiceBottleNode::create();
    this->addToContentLayer(food,4);
    food->setPosition(CMVisibleRect::getPosition(320, 460));
    food->showDrink();
    
    food->setScale(0.9f);
    this->setBackground("res/eat.png");
    auto resetDrink = [=]
    {
        auto eatAgainButton = Button::create("ui/tap/eatagain.png");
        eatAgainButton->setPosition(CMVisibleRect::getCenterOfScene()+Vec2(0,-100));
        this->addToUILayer(eatAgainButton, 20);
        
        eatAgainButton->runAction(ActionHelper::createFloatAction());
        
        eatAgainButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
            
            if (type == Widget::TouchEventType::ENDED)
            {
                food->resetDrink();
                
                eatAgainButton->getActionManager()->removeAllActionsFromTarget(eatAgainButton);
                eatAgainButton->runAction(Sequence::create(EaseExponentialIn::create(Spawn::create(RotateBy::create(0.2, 180.0), ScaleTo::create(0.2, 0.0), NULL)), RemoveSelf::create(), NULL));
            }
        });
    };
    
    food->enableDrink(nullptr, resetDrink);
    this->initEatHint(false);
    
    this->_preBtnClicked = [=]
    {
        this->_preBtnClicked = nullptr;
        SceneManager::getInstance()->replaceWithDefaultTransition(SodaDecScene::scene());
    };
}

void SodaShareScene::resetServe()
{
    auto food = JuiceBottleNode::create();
    this->addToContentLayer(food,4);
    food->setPosition(CMVisibleRect::getPosition(320, 460));
    food->showDrink();

    int index = random(1, 4);
    
    if (_npc)
    {
        _npc->removeFromParent();_npc = nullptr;
    }
    _npc = Charactor::createModel(StringUtils::format("npc%d",index));
    this->addToContentLayer(_npc,2,index);
    _npc->setPosition(CMVisibleRect::getPosition(478+800, 283+50));
    _npc->setScale(0.9f);
    this->chooseServe();
    food->setScale(0.7f);
    
    ActionHelper::moveInToSceneBounce(food, CMVisibleRect::getPosition(300, 460), kActionDirectionLeft);
    
    auto npcWalkIn = [=]
    {
        TipsUIController::getInstance()->showMoveTip(this, CMVisibleRect::getPosition(300, 460), CMVisibleRect::getPosition(413, 554+50));
        
        kAudioUtil->playEffect(StringUtils::format("sound/npc/hello%d.mp3",_npc->getTag()).c_str());
        
        
        _npc->getAnimation()->stop();
        _npc->getAnimation()->play("stand by",-1,1);
        
        this->registServe(food,
                          _npc->getBoundingBox(),
                          [=]
                          {
                              TipsUIController::getInstance()->removeAllTips();
                              
                              Bone* bone = _npc->getBone("milkshake");
                              
                              food->runAction(Spawn::create(ScaleTo::create(0.7f, 0.32),
                                                            MoveTo::create(0.7f, CMVisibleRect::getPosition(313, 554+50)),
                                                            NULL));
                              food->runAction(Sequence::create(DelayTime::create(0.7f),
                                                               Hide::create(),
                                                               NULL));
                              _npc->getAnimation()->play("na",-1,1);
                              
                              auto spawn = Spawn::create(MoveBy::create(4.5f, CMVisibleRect::getPosition(-900, 0)),
                                                         Sequence::create(DelayTime::create(1.f),
                                                                          CallFunc::create([=]
                              {
                                  this->showCoin();
                                  this->_allCoinCollceted = [=]
                                  {
                                      _allCoinGet = true;
                                      checkResever();
                                  };
                                  food->removeFromParent();
                              }),
                                                                          NULL),
                                                         CallFunc::create([=]
                                                                          {
                                                                              _walkOut = true;
                                                                              checkResever();
                                                                          }),

                                                         NULL);
                              
                              _npc->runAction(Sequence::create(DelayTime::create(0.6f),
                                                               CallFunc::create([=]
                                                                                {
                                                                                    kAudioUtil->playEffect(StringUtils::format("sound/npc/cool%d.mp3",_npc->getTag()).c_str());
                                                                                    
                                                                                    _npc->getAnimation()->stop();
                                                                                    auto sp =Sprite::create();
                                                                                    auto juice = JuiceBottleNode::create();
                                                                                    juice->showDrink();
                                                                                    sp->addChild(juice);
                                                                                    sp->setPosition(Vec2(0, 10));
                                                                                    bone->addDisplay(sp,0);
                                                                                    bone->setIgnoreMovementBoneData(true);
                                                                                    bone->setScale(0.3);
                                                                                    bone->changeDisplayWithIndex(0, true);
                                                                                    _npc->getAnimation()->play("food",-1,1);
                                                                                }),
                                                                                spawn,
                                                               NULL));
                          });
        
    };
    _npc->walkTo(CMVisibleRect::getPosition(478, 283+50), 3.5f, npcWalkIn);
}

void SodaShareScene::checkResever()
{
    if (_allCoinGet && _walkOut)
    {
        _allCoinGet = false;
        _walkOut = false;
        this->resetServe();
    }
}

