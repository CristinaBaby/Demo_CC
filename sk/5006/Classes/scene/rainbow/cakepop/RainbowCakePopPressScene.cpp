//
//  RainbowCakePopPressScene.cpp
//  shake5008
//
//  Created by liji on 17/1/5.
//
//

#include "RainbowCakePopPressScene.h"
#include "SceneHeader.h"
#include "ComponentTouchMove.h"
REGIST_SECENE(RainbowCakePopPressScene)

RainbowCakePopPressScene::RainbowCakePopPressScene():
_cupIndex(0),
_moveDis(0.f),
_bagIndex(1),
_bag(nullptr),
_cTouch(nullptr),
_totalDis(0),
_stick(nullptr)
{
    _steps.insert(std::make_pair("stepPress", CC_CALLBACK_0(RainbowCakePopPressScene::stepPress, this)));
    _steps.insert(std::make_pair("stepRoll", CC_CALLBACK_0(RainbowCakePopPressScene::stepRoll, this)));
    _steps.insert(std::make_pair("stepChooseStick", CC_CALLBACK_0(RainbowCakePopPressScene::stepChooseStick, this)));
    
    _stepName =
    {
        "stepPress",
        "stepRoll",
        "stepChooseStick",
    };

}

RainbowCakePopPressScene::~RainbowCakePopPressScene()
{

}

bool RainbowCakePopPressScene::init()
{
    if (!ESMakeBaseScene::init())
    {
        return false;
    }

    this->setBackground("res/cake_pop/step4_push/cake_pop_bg2.png");
    
    
    showNextStep();
    return true;
}

void RainbowCakePopPressScene::stepPress()
{
    auto plate = Sprite::create("res/cake_pop/step4_push/plate1.png");
    this->addToContentLayer(plate);
    
    auto btm = Sprite::create("res/cake_pop/step4_push/push4_cup.png");
    plate->addChild(btm);
    btm->setPosition(plate->getContentSize()/2+Size(-10,40));
    btm->setVisible(false);
    int index = kESDataManager->getRainbowcakepopFood().colorIndexVec.at(_cupIndex);

    auto cup = Sprite::create(StringUtils::format("res/cake_pop/step4_push/push1_%d.png",index));
    plate->addChild(cup,2);
    cup->setPosition(plate->getContentSize()/2+Size(0,80));

    static int s_shapeIndex = 3;
    s_shapeIndex = 3;
    
    auto cTouch = ComponentTouchMove::create();
    cTouch->touchClickedCall=[=](Touch* t)
    {
        kAudioUtil->playEffect(kSoundPress);
        TipsUIController::getInstance()->removeAllTips();
        cTouch->setEnabled(false);
        auto call = [=]
        {
            cTouch->setEnabled(true);

            int rIndex = (int)s_shapeIndex/2;
            if (rIndex <=0)
            {
                rIndex = 1;
            }
            cup->setTexture(StringUtils::format("res/cake_pop/step4_push/push%d_%d.png",rIndex,index));
            if (s_shapeIndex == 8)
            {
                cup->setPosition(plate->getContentSize()/2+Size(0,50));
                cup->setScale(0.95f);
                btm->setVisible(true);
                cTouch->setEnabled(false);
            }
            else
            {
                btm->setVisible(false);
            }
            s_shapeIndex++;
            if (s_shapeIndex>8)
            {
                _cupIndex++;
                cTouch->setEnabled(false);

                this->stepFinishEffect(plate, [=]
                                       {
                                           ActionHelper::moveOutofSceneAndRemoveSelf(plate, kActionDirectionRight,[=]
                                                                                     {
                                                                                         if (_cupIndex<4)
                                                                                         {
                                                                                             stepPress();
                                                                                         }
                                                                                         else
                                                                                         {
                                                                                             this->showNextStep();
                                                                                         }
                                                                                     });
                                       }, CMVisibleRect::getCenterOfScene());
                return;
            }
        };
        cup->runAction(Sequence::create(ActionHelper::createRollJelly(),
                                        CallFunc::create(call),
                                        NULL));
        
    };
    cup->addComponent(cTouch);
    cTouch->setIsCanMove(false);
    
    ActionHelper::moveInToSceneBounce(plate, CMVisibleRect::getPosition(320, 320), kActionDirectionLeft,[=]
    {
        TipsUIController::getInstance()->showClickTip(this, plate->getPosition());
    });
}

void RainbowCakePopPressScene::stepRoll()
{
//    this->setBackground("res/cake_pop/cake_pop_bg1.png");

    _bag = BowlNode::create("res/cake_pop/step5_roll/1/membrane1.png","");
    this->addToContentLayer(_bag);
    _bag->initMaskNode("res/cake_pop/step5_roll/5/cakepop_shadow5.png",Vec2(0, -5));
    _bag->adjustPostion(Vec2(0,120));
    
    auto inCall = [=]
    {
        _cTouch = ComponentTouchMove::create();
        _cTouch->setIsCanMove(false);
        _bag->addComponent(_cTouch);
        this->packBag();
        
        for (int i = 0 ; i<4;i++)
        {
            int index = kESDataManager->getRainbowcakepopFood().colorIndexVec.at(i);
            auto cake = Sprite::create(StringUtils::format("res/cake_pop/step4_push/push4_%d.png",index));
            _bag->addContent(cake, StringUtils::format("cake%d",i), 1);
            cake->setScale(0.97f);
            cake->setOpacity(255);
            _cakeVec.push_back(cake);
            ActionHelper::moveInToSceneBounce(cake, Vec2(8,-60+i*20+120), kActionDirectionLeft,nullptr,1.2);
        }
    };
    
    ActionHelper::moveInToSceneBounce(_bag, CMVisibleRect::getPosition(320, 320), kActionDirectionLeft,inCall);
}

void RainbowCakePopPressScene::packBag()
{
    float gDis = 80.f;

    switch (_bagIndex)
    {
        case 1:
        {
            TipsUIController::getInstance()->showMoveTip(this, CMVisibleRect::getPosition(260, 300), CMVisibleRect::getPosition(260, 600));
            _cTouch->touchMovedCall = [=](Touch* t)
            {
                auto deltaY = t->getDelta().y;
                if(deltaY>0)
                {
                    _moveDis += deltaY;
                }
                log("======deltaY %f",deltaY);
                log("======s_d %f",_moveDis);
                
                
            };
            _cTouch->touchEndedCall = [=](Touch* t)
            {
                if (_moveDis>gDis)
                {
                    kAudioUtil->playEffect(kSoundPlasticBag);

                    TipsUIController::getInstance()->removeAllTips();
                    _moveDis = 0.f;
                    _bagIndex++;
                    _bag->updateBowlConfig(StringUtils::format("res/cake_pop/step5_roll/%d/membrane%d_1.png",_bagIndex,_bagIndex), StringUtils::format("res/cake_pop/step5_roll/%d/membrane%d.png",_bagIndex,_bagIndex));
                    packBag();
                }
            };
        }
            break;
        case 2:
        {
            TipsUIController::getInstance()->showMoveTip(this, CMVisibleRect::getPosition(225, 540), CMVisibleRect::getPosition(325, 540));
            _cTouch->touchMovedCall = [=](Touch* t)
            {
                auto deltaY = t->getDelta().x;
                if(deltaY>0)
                {
                    _moveDis += deltaY;
                }
                log("======deltaY %f",deltaY);
                log("======s_d %f",_moveDis);
                
                
            };
            _cTouch->touchEndedCall = [=](Touch* t)
            {
                if (_moveDis>80)
                {
                    kAudioUtil->playEffect(kSoundPlasticBag);
                    TipsUIController::getInstance()->removeAllTips();
                    _moveDis = 0.f;
                    _bagIndex++;
                    _bag->updateBowlConfig(StringUtils::format("res/cake_pop/step5_roll/%d/membrane%d_1.png",_bagIndex,_bagIndex), StringUtils::format("res/cake_pop/step5_roll/%d/membrane%d.png",_bagIndex,_bagIndex));
                    packBag();
                }
            };
        }
            break;
        case 3:
        {
            _cTouch->touchMovedCall = nullptr;            _cTouch->touchEndedCall = nullptr;
            auto rope = BowlNode::create("res/cake_pop/step5_roll/4/rope.png","");
            this->addToContentLayer(rope,2);
            auto inCall = [=]
            {
                TipsUIController::getInstance()->showMoveTipBezel(this, CMVisibleRect::getPosition(508, 568), CMVisibleRect::getPosition(333, 445));

                rope->setPourWorldPoint(CMVisibleRect::getPosition(323, 485+60), [=]
                {
                    TipsUIController::getInstance()->removeAllTips();

                    rope->runAction(Sequence::create(
                                                     ScaleTo::create(0.3f, 1.2f),
                                                     ScaleTo::create(0.3f, 1.f),
                                                     CallFunc::create([=]
                    {
                        rope->setVisible(false);
                        _bagIndex++;
                        _bag->updateBowlConfig(StringUtils::format("res/cake_pop/step5_roll/%d/membrane%d_1.png",_bagIndex,_bagIndex), StringUtils::format("res/cake_pop/step5_roll/%d/membrane%d.png",_bagIndex,_bagIndex));
                        packBag();
                    }), NULL));
                });
            };
            ActionHelper::moveInToSceneBounce(rope, CMVisibleRect::getPosition(508, 568),kActionDirectionRight,inCall);
        }
            break;
        case 4:
        {
            _bagIndex++;
            packBag();
        }
            break;
        case 5:
        {
            TipsUIController::getInstance()->showMoveTip(this, CMVisibleRect::getPosition(225, 440), CMVisibleRect::getPosition(425, 440));

            this->initProgressTimer();
            
            vector<Sprite*> cutCakeVec;
            for (int i = 0 ; i<4;i++)
            {
                int index = kESDataManager->getRainbowcakepopFood().colorIndexVec.at(i);
                auto cake = Sprite::create(StringUtils::format("res/cake_pop/step4_push/push4_%d.png",index));
                _bag->addContentToMask(cake, StringUtils::format("m_cake%d",i), 1);
                cake->setPosition(Vec2(8,-70+i*60));
                cake->setScale(1.4f);
                cake->setOpacity(0);
//                cake->runAction(FadeIn::create(0.6f));
                cutCakeVec.push_back(cake);
            }

            
            _cTouch->touchMovedCall = [=](Touch* t)
            {
                log("======1=====");
                
                kSoundInstance->playLoopEffect(kSoundRollBag);
                TipsUIController::getInstance()->removeAllTips();
                _moveDis += t->getDelta().x;
                if (_moveDis>=gDis)
                    _moveDis = gDis;
                if (_moveDis<=-gDis)
                {
                    _moveDis = -gDis;
                }
                _bag->setRotation(90*_moveDis/80);
                
                _progressTimer->setPercentage(_totalDis/_maxLength*100);
                
                _totalDis += abs(t->getDelta().x);
                
                for (auto cake : _cakeVec)
                {
                    //                        cake->setVisible(false);
                    cake->setOpacity((_maxLength-_totalDis)/_maxLength*255);
                }
                for (auto cake : cutCakeVec)
                {
                    //                        cake->setVisible(false);
                    cake->setOpacity(_totalDis/_maxLength*255);
                }

                
                if(_totalDis>=_maxLength)
                {
                    log("======1=====");

                    for (auto cake : _cakeVec)
                    {
                        //                        cake->setVisible(false);
                        cake->setOpacity(0);
                    }
                    for (auto cake : cutCakeVec)
                    {
                        //                        cake->setVisible(false);
                        cake->setOpacity(255);
                    }
                    kSoundInstance->stopLoopEffect();

                    _cTouch->setEnabled(false);
                    this->removeProgressTimer();
//                    _cTouch->touchMovedCall = nullptr;
                    this->stepFinishEffect(_bag, [=]
                    {
                        this->showNextStep();
                    }, CMVisibleRect::getPosition(320, 220+30)),
                    _totalDis = 0.f;
                    _bag->setRotation(0);
                }
            };
            _cTouch->touchEndedCall = [=](Touch* t)
            {
                kSoundInstance->stopLoopEffect();
            };
        }
            break;
            
        default:
            break;
    }


}

void RainbowCakePopPressScene::stepChooseStick()
{
    if (!_bag)
    {
        _bag = BowlNode::create("res/cake_pop/step5_roll/1/membrane1.png","");
        this->addToContentLayer(_bag);
        _bag->initMaskNode("res/cake_pop/step5_roll/5/cakepop_shadow5.png",Vec2(0, -5+30));
        _bag->setPosition(CMVisibleRect::getPosition(320, 220+30));
        
        for (int i = 0 ; i<4;i++)
        {
            int index = kESDataManager->getRainbowcakepopFood().colorIndexVec.at(i);
            auto cake = Sprite::create(StringUtils::format("res/cake_pop/step4_push/push4_%d.png",index));
            _bag->addContentToMask(cake, StringUtils::format("m_cake%d",i), 1);
            cake->setPosition(Vec2(8,-70+i*60));
            cake->setScale(1.4f);
        }
        
    }
    kAudioUtil->playEffect(kSoundPlasticBag);
    kAudioUtil->playEffect(kVoiceDecChooseStick);
    _bag->updateBowlConfig("res/cake_pop/step5_roll/1/membrane1.png", "","res/cake_pop/step5_roll/5/cakepop_shadow5.png");
    _bag->getBottom()->setPosition(0, 50);
    DecorationConfig config = DecorationConfig(
                                               "rainbowcakepop",
                                               "stick",
                                               "",
                                               "",
                                               "res/cake_pop/step6_sticks/icon_sticks%d.png",
                                               "res/cake_pop/step6_sticks/stick%d.png",
                                               1,
                                               15
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
    
    this->onNextButtonTouchCallback = [=]
    {
        this->onNextButtonTouchCallback = nullptr;

        SceneManager::getInstance()->replaceWithDefaultTransition(RainbowCakePopDipScene::scene());
    };
    
    TipsUIController::getInstance()->showClickTip(this, CMVisibleRect::getPosition(320, 669+50));
    
    auto callback = [=](ui::Widget* item)
    {
        TipsUIController::getInstance()->removeAllTips();
        _scrollView->setEnabled(false);
        int itemTag = item->getTag();
        log("===item clicked %d====",itemTag);
        auto food = kESDataManager->getRainbowcakepopFood();
        food.stickIndex = itemTag;
        kESDataManager->setRainbowcakepopFood(food);
        
        Vec2 pos = CMVisibleRect::getPosition(320, 346);
        
        auto changeCall = [=]
        {
            this->showNextButton();
            
            _scrollView->setEnabled(true);
            
            updateStick();
            this->showParticle(_bag, Vec2::ZERO);
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

void RainbowCakePopPressScene::updateStick()
{
    if (!_stick)
    {
        _stick = Sprite::create();
        _bag->addChild(_stick,1);
        _stick->setPosition(Vec2(0, 180));
        _stick->setRotation(180);
    }
    int index = kESDataManager->getRainbowcakepopFood().stickIndex;
    auto path = StringUtils::format("res/cake_pop/step6_sticks/stick%d.png",index);
    _stick->setTexture(path);

}

