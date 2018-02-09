//
//  Charactor.cpp
//  BHUG1062
//
//  Created by liji on 16/4/27.
//
//

#include "Charactor.h"
#include "SoundConfig.h"
#include "ActionHelper.h"



Charactor::Charactor()
{
    _angryTime = ANGRY_TIME;
    _requiredFoodType = 0;
};



Charactor* Charactor::createModel(string name){
    Charactor *armature = new Charactor();
    if (armature && armature->init(name))
    {
        armature->autorelease();
        armature->_name=name;
        return armature;
    }
    CC_SAFE_DELETE(armature);
    return nullptr;
}


bool Charactor::init(string name)
{
    if (!Armature::init(name))
    {
        return false;
    }

    
    return true;
}

void Charactor::initProgressTimer()
{
    _progressTimer = ProgressTimer::create(Sprite::create("ui/ui_png/choose1/box_bg1_1.png"));
    _progressTimer->setType(ProgressTimer::Type::BAR);
    _progressTimer->setMidpoint(Vec2(0.0, 0.0));
    _progressTimer->setBarChangeRate(Vec2(0, 1));
    float percent = (float)(ANGRY_TIME-_angryTime)/15*100;
    _progressTimer->setPercentage(percent);

    _progressBg = Sprite::create("ui/ui_png/choose1/box_bg1.png");
    _progressBg->setPosition(Vec2(getContentSize().width + _progressBg->getContentSize().width/2, getContentSize().height/2));
    _progressTimer->setPosition(_progressBg->getContentSize()/2);
    _progressBg->addChild(_progressTimer,1);
    this->getParent()->addChild(_progressBg, 999);
    _progressBg->setPosition(450,571);
    _progressBg->setVisible(false);
    
    string foodPath;
    switch (_requiredFoodType)
    {
        case 0:
            foodPath = "ui/ui_png/choose1/dount.png";
            break;
        case 1:
            foodPath = "ui/ui_png/choose1/snow_cone.png";
            break;
        case 2:
            foodPath = "ui/ui_png/choose1/cotton_candy.png";
            break;
        case 3:
            foodPath = "ui/ui_png/choose1/drink.png";
            break;
        default:
            break;
    }
    
    auto food = Button::create(foodPath);
    food->setAnchorPoint(Vec2::ZERO);
    _progressBg->addChild(food,2,_requiredFoodType);
    food->runAction(RepeatForever::create(Sequence::create(DelayTime::create(2),
                                                           ActionHelper::createRollJelly(),
                                                           NULL)));
    food->addTouchEventListener([=](Ref* obj,Widget::TouchEventType type)
    {
        if (type == Widget::TouchEventType::ENDED)
        {
            if (_foodButtonClicked)
                _foodButtonClicked(dynamic_cast<Button*>(obj)->getTag());
        }
    
    });
    
    

    _emoji = Sprite::create(getEmojiPath());
    _emoji->setAnchorPoint(Vec2::ZERO);
    _progressBg->addChild(_emoji,3);


}

string Charactor::getEmojiPath()
{
    //ui/ui_png/choose1/expression_1.png
    int idx = _angryTime/(ANGRY_TIME/EMOTION_STEP);

    return StringUtils::format("ui/ui_png/choose1/expression_%d.png",4-idx);
}

//void Charactor::playAction(const string &name)
//{
//    this->getAnimation()->play(name,-1,1);
//}

void Charactor::playAction(const string &name ,function<void()> callback)
{
    this->getAnimation()->play(name,-1,1);
    
    this->getAnimation()->setMovementEventCallFunc([=](Armature * model,MovementEventType type, const std::string name)
    {
        if (type == MovementEventType::LOOP_COMPLETE)
        {
            if (callback)
                callback();

        }
    });


}


void Charactor::updateEmoji()
{
    if (!_emoji)
        return;
    _emoji->setPosition(0, -5);
    _emoji->setTexture(getEmojiPath());
    _emoji->runAction(MoveBy::create(0.2, Vec2(0, 5)));

}

void Charactor::playRandomAction()
{
    vector<string> stringVec =
    {
//        NPC_EAT,
        NPC_JUMP,
        NPC_GOOD,
//        NPC_BAD,
    };

    int randIndex = rand()%2;
    
    this->getAnimation()->play(stringVec.at(randIndex),-1,1);
    
    this->getAnimation()->setMovementEventCallFunc([=](Armature * model,MovementEventType type, const std::string name)
                                                   {
                                                       if (type == MovementEventType::LOOP_COMPLETE)
                                                       {
                                                           this->getAnimation()->play(NPC_STAND,0,1);
                                                       }
                                                   });
    
}



void Charactor::initProgressTimerHeartStyle()
{
    _progressTimer = ProgressTimer::create(Sprite::create("ui/res/food/heart_bg_1.png"));
    _progressTimer->setType(ProgressTimer::Type::BAR);
    _progressTimer->setMidpoint(Vec2(0.0, 0.0));
    _progressTimer->setBarChangeRate(Vec2(1.0, 0));
    float percent = (float)_angryTime/ANGRY_TIME*100;
    _progressTimer->setPercentage(percent);
    
    _progressBg = Sprite::create("ui/res/food/heart_bg.png");
    _progressBg->setPosition(Vec2(getContentSize().width + _progressBg->getContentSize().width/2, getContentSize().height/2));
    _progressTimer->setPosition(_progressBg->getContentSize()/2);
    _progressBg->addChild(_progressTimer,1);
    this->getParent()->addChild(_progressBg, 999);
    _progressBg->setPosition(844, 648);
    _progressBg->setVisible(false);
    
    int heartCount = int(_angryTime/7)+1>3?3:int(_angryTime/7)+1;
    
    for (int i=0; i<heartCount; i++)
    {
        auto heart = Sprite::create("ui/res/food/heart.png");
        heart->setPosition(134-(3-heartCount)*55-i*65, 10);
        _progressBg->addChild(heart,2);
        heart->setScale(0);
        heart->runAction(Sequence::create(DelayTime::create(0.3-i*0.1),
                                          ScaleTo::create(0.3, 1),
                                          NULL));
        _heartsVec.pushBack(heart);
    }


}



void Charactor::walkTo(Vec2 endPos,float duration,function<void()> call)
{
    this->getAnimation()->play(NPC_WALK);
    this->runAction(Sequence::create(MoveTo::create(duration, endPos),
                                     CallFunc::create(call),
                                     NULL));
    

}

void Charactor::standBy()
{
//    srand(time(nullptr));
//    
//    int i = rand()%2;
//    
//    string s;
//    switch (i)
//    {
//        case 0:
//            s = "wait";
//            break;
//        case 1:
//            s = "good";
//            break;
//            
//        default:
//            break;
//    }
    this->getAnimation()->play(NPC_STAND,-1,0);
    this->getAnimation()->setMovementEventCallFunc([&](Armature * model,MovementEventType type, const std::string name){
        if (type == MovementEventType::COMPLETE) {
            this->standBy();
        }
    });

}


void Charactor::laugh()
{
    this->getAnimation()->play(NPC_HAPPY,-1,0);
    this->getAnimation()->setMovementEventCallFunc([&](Armature * model,MovementEventType type, const std::string name){
        if (type == MovementEventType::COMPLETE) {
            this->laugh();
        }
    });
}


void Charactor::checkStatus(float dt)
{
//    if (_heartsVec.size()==0)
//        return;
    
    _angryTime--;
    if (_angryTime<=0)
    {
        this->unschedule(schedule_selector(Charactor::checkStatus));
        _angryTime=0;
    }
    
    if (_angryTime%(ANGRY_TIME/EMOTION_STEP) == 0 && _angryTime != ANGRY_TIME)
    {
        if (_heartsVec.size()!=0)
        {
            if (_heartIndex>=_heartsVec.size())
            {
                _heartIndex = 0;
            }
            _heartsVec.at(_heartIndex)->runAction(ScaleTo::create(0.4, 0));
            _heartIndex++;
        }
        else
        {
            this->updateEmoji();
        }
    }
    if(_angryTime == ANGRY_TIME/EMOTION_STEP*2)
    {
        this->standBy();
    }
    
    if (_angryTime == ANGRY_TIME/EMOTION_STEP)
    {
        string s = NPC_DISAPPOINT;
        this->getAnimation()->stop();
        this->getAnimation()->play(s,-1,3);
    }
    if (_angryTime == 0)
    {
        this->getAnimation()->play(NPC_ANGRY,-1,3);
        this->runAction(Sequence::create(DelayTime::create(2),
                                         CallFunc::create([=]
        {
            this->leave();
        }),
                                         NULL));
    }
}

void Charactor::startWaiting()
{
    this->schedule(schedule_selector(Charactor::checkStatus), 1);
    _progressTimer->runAction(ProgressTo::create(_angryTime, 100));
    if(_angryTime>(ANGRY_TIME/EMOTION_STEP))
    {
        this->playVoiceStand();
        this->laugh();
    }
    else
    {
        this->angry();
    }
    _progressBg->setVisible(true);

}
void Charactor::pauseWaiting()
{
    this->unschedule(schedule_selector(Charactor::checkStatus));
    _actionManager->pauseTarget(_progressTimer);
}

void Charactor::resumeWaiting()
{
    this->schedule(schedule_selector(Charactor::checkStatus), 1);
    _actionManager->resumeTarget(_progressTimer);
}

void Charactor::eat(function<void()> eatCall , function<void()> leaveCall)
{
    this->getAnimation()->play(NPC_EAT,-1,0);
    this->getAnimation()->setMovementEventCallFunc([=](Armature * model,MovementEventType type, const std::string name){
        if (type == MovementEventType::COMPLETE) {
            
            if (eatCall)
                eatCall();
            this->playVoiceEat();
            
            
            this->leave(leaveCall);
            
        }
    });
}

void Charactor::angry()
{
    string s = NPC_ANGRY;
    this->getAnimation()->stop();
    this->getAnimation()->play(s,-1,3);
}


void Charactor::leave(function<void()> call)
{
    this->pauseWaiting();
    this->getAnimation()->stop();
    this->setScale(this->getScale()*-1, this->getScale());
    if(_progressBg)
    {
        _progressBg->runAction(Sequence::create(ScaleTo::create(0.5, 0),
                                                RemoveSelf::create(),
                                                NULL));
        _progressBg = nullptr;
    }

    this->runAction(Sequence::create(DelayTime::create(2.5),
                                     CallFunc::create([=]
                                                      {
                                                          if (call)
                                                          {
                                                              call();
                                                          }
                                                          
                                                      }),
                                     NULL));

    if (_walkedOutScreen)
    {
        this->walkTo(getPosition()+Vec2(800, 0), 2.5,_walkedOutScreen);
    }
    else
    {
        this->walkTo(getPosition()+Vec2(800, 0), 2.5,nullptr);
    }
}


void Charactor::playVoiceEat()
{
    if (_npcIndex<5)
    {
        SoundConfig::getInstance()->playVoiceOver(7+_npcIndex*2);
    }
    else
    {
        SoundConfig::getInstance()->playVoiceOver(17);
    }
    
    
}

void Charactor::playVoiceStand()
{
    if (_npcIndex<5)
    {
        SoundConfig::getInstance()->playVoiceOver(6+_npcIndex*2);
    }
    else
    {
        SoundConfig::getInstance()->playVoiceOver(16);
    }
}

void Charactor::onEnter()
{
    Armature::onEnter();
    
}

void Charactor::sayHi(function<void()> call)
{
    this->getAnimation()->play(NPC_SAYHI,-1,1);
    this->runAction(Sequence::create(DelayTime::create(0.8),
                                     CallFunc::create([=]
    {
        if (call)
        {
            call();
        }
    
    }),
                                     NULL));

}

void Charactor::happy(function<void()> call)
{
    this->getAnimation()->play(NPC_HAPPY,-1,1);

    this->runAction(Sequence::create(DelayTime::create(2.2),
                                     CallFunc::create([=]
                                                      {
                                                          if (call)
                                                          {
                                                              call();
                                                          }
                                                          
                                                      }),
                                     NULL));

}

void Charactor::foodReady(function<void()> call )
{
    this->getAnimation()->play(NPC_RUN,-1,1);
    
    this->runAction(Sequence::create(
                                     Place::create(Vec2(1136/2+1000, 768/2-600)),
                                     MoveTo::create(2.2, Vec2(1136/2, 768/2-600)),
                                     CallFunc::create([=]
                                                      {
                                                          this->getAnimation()->play(NPC_STAND,-1,1);
                                                          if (call)
                                                          {
                                                              call();
                                                          }
                                                          
                                                      }),
                                     DelayTime::create(1),
                                     Place::create(Vec2(8888, 8888)),
                                     NULL));

}



