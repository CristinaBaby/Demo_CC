//
//  SodaChooseStepScene.cpp
//  CCATS022
//
//  Created by liji on 16/11/16.
//
//

#include "SodaChooseStepScene.h"
#include "ESDataManager.h"
#include "SceneHeader.h"
SodaChooseStepScene::SodaChooseStepScene():
_npc(nullptr)
{
    cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("boy/boy.ExportJson");

}

SodaChooseStepScene::~SodaChooseStepScene()
{
    cocostudio::ArmatureDataManager::getInstance()->removeArmatureFileInfo("boy/boy.ExportJson");

}

bool SodaChooseStepScene::init()
{
    if (!ESMakeBaseScene::init())
    {
        return false;
    }
    
    this->setBackground("ui/choose/bg.jpg");
    
    kESDataManager->setShowRate(true);

    _npc = Charactor::createModel("boy");
    this->addToContentLayer(_npc,2);
    _npc->setPosition(CMVisibleRect::getPosition(478+800, 283+50));
    _npc->setScale(0.9f);

    auto npcWalkIn = [=]
    {
//        kAudioUtil->playEffect(StringUtils::format("sound/npc/hello%d.mp3",2).c_str());
        _npc->getAnimation()->stop();
        _npc->getAnimation()->play("stand by",-1,1);
    };
    _npc->walkTo(CMVisibleRect::getPosition(320, 283+50), 3.5f, npcWalkIn);

    auto desk = Sprite::create("ui/choose/desk.jpg");
    desk->setPosition(CMVisibleRect::getPosition(320, -120,kBorderNone,kBorderBottom));
    desk->setAnchorPoint(Vec2(0.5, 0));
    this->addToContentLayer(desk,4);
    
    vector<Vec2> posVec =
    {
        CMVisibleRect::getPosition(153, 573),
        CMVisibleRect::getPosition(498, 573),
        CMVisibleRect::getPosition(153, 313),
        CMVisibleRect::getPosition(498, 313),
    };
    
    auto call = [=](Ref* ref,Widget::TouchEventType type)
    {
        auto btn = dynamic_cast<Button*>(ref);
        if (type == Widget::TouchEventType::ENDED)
        {
            _director->getEventDispatcher()->removeAllEventListeners();
            CMAudioUtil::getInstance()->playBackGroundMusic(kBGMInModule);
//            kAudioUtil->setBgMusicVolume(1.f);

            switch (btn->getTag())
            {
                case 0:// next
                {
//                    kSoundInstance->playEffectWithCallback(kVoiceJuiceStep1, 2.f, []
//                    {
//                        SceneManager::getInstance()->replaceWithDefaultTransition(SodaCutFruitScene::scene());
//                    });
                    SceneManager::getInstance()->replaceWithDefaultTransition(SodaCutFruitScene::scene());
                }
                    break;
                case 1:// next
                {
//                    kSoundInstance->playEffectWithCallback(kVoiceJuiceStep2, 2.3f, []
//                                                           {
//                                                               SceneManager::getInstance()->replaceWithDefaultTransition(SodaMakeIceScene::scene());
//                                                           });
                    SceneManager::getInstance()->replaceWithDefaultTransition(SodaMakeIceScene::scene());

                }
                    break;
                case 2:// next
                {
//                    kSoundInstance->playEffectWithCallback(kVoiceJuiceStep3, 2.3f, []
//                                                           {
//                                                               SceneManager::getInstance()->replaceWithDefaultTransition(SodaBlendScene::scene());
//                                                           });
                    SceneManager::getInstance()->replaceWithDefaultTransition(SodaBlendScene::scene());

                }
                    break;
                case 3:// next
                {
                    kESDataManager->removePaint();
                    SceneManager::getInstance()->replaceWithDefaultTransition(SodaDecScene::scene());

//                    kSoundInstance->playEffectWithCallback(kVoiceJuiceStep4, 2.3f, []
//                                                           {
//                                                               SceneManager::getInstance()->replaceWithDefaultTransition(SodaDecScene::scene());
//                                                           });
                }
                    break;
            }
        }
    };
    
    vector<string> soundnameVec =
    {
        kVoiceJuiceStep1,
        kVoiceJuiceStep2,
        kVoiceJuiceStep3,
        kVoiceJuiceStep4,
        
    };
    
    for (int i = 0; i<4; i++)
    {
        auto btn = Button::create(StringUtils::format("ui/choose/step%d/bg.png",i+1));
        btn->loadTexturePressed(StringUtils::format("ui/choose/step%d/bg.png",i+1));
        this->addToUILayer(btn,2,i);
        btn->setPosition(posVec.at(i));
        btn->setOpacity(0);
        int stepIndex = kESDataManager->getNextStep();
        
        if (!kESDataManager->getSodaStepUnlocked(i))
        {
//            btn->setBright(false);
            btn->setEnabled(false);
        }
        this->showContentsInBtns(btn, kESDataManager->getSodaStepUnlocked(i));
        btn->addTouchEventListener(call);
        
        if (stepIndex == i)
        {
            this->runAction(Sequence::create(DelayTime::create(0.6f),
                                             CallFunc::create([=]
            {
                if (!kESDataManager->getSodaStepUnlocked(4))
                {
                    auto p = ParticleSystemQuad::create("particle/decorateParticle.plist");
                    this->addToUILayer(p,999);
                    p->setPosition(btn->getPosition());
                    p->setDuration(1.f);
                    kAudioUtil->playEffect(kSoundShow);
                    kAudioUtil->playEffect(soundnameVec.at(i).c_str());
                }
            }),
                                             NULL));

        }
    }
    

    
    return true;
}

void SodaChooseStepScene::showContentsInBtns(Widget* item, bool unlocked)
{
    int index = item->getTag();
    auto word = Sprite::create(StringUtils::format("ui/choose/step%d/word.png",index+1));
    item->addChild(word,10);
    word->setAnchorPoint(Vec2(0.5, 0.12));
    word->setPosition(Vec2(item->getContentSize().width/2,30));
    
    GLProgram *glp = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_GRAYSCALE);

    auto bg = Sprite::create(StringUtils::format("ui/choose/step%d/bg.png",index+1));
    bg->setPosition(item->getContentSize()/2);
    item->addChild(bg);

    if (index == kESDataManager->getNextStep())
    {
        word->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.2f, 1.15f),
                                                             ScaleTo::create(0.2f, 1.f),
                                                             DelayTime::create(1.3f),
                                                             NULL)));
    }
    
    switch (index)
    {
        case 0:
        {
            auto board = ImageView::create("ui/choose/step1/board.png");
            board->setPosition(item->getContentSize()/2);
            item->addChild(board);
            
            for (int i = 1; i<4; i++)
            {
                auto fruit = ImageView::create(StringUtils::format("ui/choose/step1/fruit%d.png",i));
                item->addChild(fruit,2);
                fruit->setPosition(item->getContentSize()/2);
            }
            
            auto knife = Sprite::create("ui/choose/step1/knife.png");
            knife->setPosition(item->getContentSize()/2);
            item->addChild(knife,3);
            
            if (kESDataManager->getNextStep() == index)
            {
                knife->runAction(RepeatForever::create(Sequence::create(Place::create(item->getContentSize()/2),
                                                                        MoveTo::create(0.8f, Vec2(144, 109)),
                                                                        DelayTime::create(0.3f),
                                                                        NULL)));
            }
        }
            break;
        case 1:
        {
            auto board = Sprite::create("ui/choose/step2/refrigerator_ok.png");
            board->setPosition(item->getContentSize()/2);
            item->addChild(board);
            if (!unlocked)
            {
                board->setGLProgram(glp);
            }

            for (int i = 1; i<4; i++)
            {
                auto fruit = Sprite::create(StringUtils::format("ui/choose/step2/ice%d.png",i));
                item->addChild(fruit,2,i);
                fruit->setPosition(item->getContentSize()/2);
                if (!unlocked)
                {
                    fruit->setGLProgram(glp);
                }
                if (kESDataManager->getNextStep() == index)
                {
                    fruit->runAction(RepeatForever::create(Sequence::create(
                                                                            DelayTime::create(0.2f*i),
                                                                            JumpBy::create(0.6f, Vec2(0, 0), 60, 1),
                                                                            NULL)));
                }
            }
        }
            break;
        case 2:
        {
            auto board = Sprite::create("ui/choose/step3/stir.png");
            board->setPosition(item->getContentSize()/2);
            item->addChild(board);
            if (!unlocked)
            {
                board->setGLProgram(glp);
            }

            for (int i = 1; i<3; i++)
            {
                auto fruit = Sprite::create(StringUtils::format("ui/choose/step3/fruit%d.png",i));
                item->addChild(fruit,2);
                fruit->setPosition(item->getContentSize()/2);
                if (!unlocked)
                {
                    fruit->setGLProgram(glp);
                }
                else
                {
                    if (kESDataManager->getNextStep() == index)
                    {
                        fruit->runAction(RepeatForever::create(Sequence::create(
                                                                                DelayTime::create(0.4f*i),
                                                                                Place::create(item->getContentSize()/2+Size(0, 60+i*10)),
                                                                                MoveBy::create(0.6f, Vec2(0, -(60+i*10))),
                                                                                NULL)));
                    }
                }
            }
        }
            break;
        case 3:
        {
            auto board = Sprite::create("ui/choose/step4/milkshake.png");
            board->setPosition(item->getContentSize()/2);
            item->addChild(board,4);
            if (!unlocked)
            {
                board->setGLProgram(glp);
            }

            for (int i = 1; i<=4; i++)
            {
                auto fruit = Sprite::create(StringUtils::format("ui/choose/step4/dec%d.png",i));
                item->addChild(fruit,5);
                fruit->setPosition(item->getContentSize()/2);
                if (i==1)
                {
                    fruit->setZOrder(3);
                }
                if (!unlocked)
                {
                    fruit->setGLProgram(glp);
                }
                else
                {
                    if (kESDataManager->getNextStep() == index)
                    {
                        fruit->runAction(RepeatForever::create(Sequence::create(
                                                                                DelayTime::create(0.2*i),
                                                                                RotateTo::create(0, 0),
                                                                                ActionHelper::createShakeAction(),
                                                                                NULL)));
                    }
                }
            }
        }
            break;
            
        default:
            break;
    }

    if (unlocked)
    {
//        word->setScale(0.f);
//        word->runAction(Sequence::create(DelayTime::create(0.1*index),
//                                         EaseBackInOut::create(ScaleTo::create(0.6f, 1)),
//                                         NULL));
    }
    else
    {
        word->setGLProgram(glp);
        bg->setGLProgram(glp);
    }
}

