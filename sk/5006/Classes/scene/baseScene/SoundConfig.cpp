//
//  SoundConfig.cpp
//  BHUG1062
//
//  Created by liji on 16/5/16.
//
//

#include "SoundConfig.h"

static SoundConfig* s_instance = nullptr;

SoundConfig* SoundConfig::getInstance()
{
    if (!s_instance)
    {
        s_instance = new SoundConfig();
    }
    
    return s_instance;
}


void SoundConfig::playLoopEffect(const char* pszFilePath)
{
    if (_loopID == -1)
    {
        _loopID = kAudioUtil->playEffect(pszFilePath,true);
    }
    
}

void SoundConfig::stopLoopEffect()
{
    if (_loopID != -1)
    {
        kAudioUtil->stopEffectBySoundId(_loopID);
        _loopID = -1;
    }

}

void SoundConfig::playVoiceOver(int idx)
{
    kAudioUtil->playEffect(StringUtils::format("sound/voice over/%d.mp3",idx).c_str());
}

void SoundConfig::playRandomVoice()
{

    int chance = rand()%4;
    
    if (chance == 2)
    {
        SoundConfig::getInstance()->playVoiceOver(rand()%9+100); 
    }

}

void SoundConfig::playRandomStepFinishVoice()
{
    vector<string> pathVec =
    {
//        "sound/voice over1/good/Creative.mp3",
        "sound/voice over1/good/Fabulous.mp3",
        "sound/voice over1/good/Good.mp3",
//        "sound/voice over1/good/Great choice.mp3",
        "sound/voice over1/good/Great.mp3",
        "sound/voice over1/good/Good job.mp3",
        "sound/voice over1/good/Nice.mp3",
        "sound/voice over1/good/Well done.mp3",
    };

    int chance = random(0, (int)pathVec.size()-1);
    
    kAudioUtil->playEffect(pathVec.at(chance).c_str());
}

void SoundConfig::playRandomEatVoice()
{
    vector<string> pathVec =
    {
        "sound/voice over1/tasty/tasty.mp3",
        "sound/voice over1/tasty/yummy.mp3",
    };
    
    int chance = rand()%2;
    
    
    kAudioUtil->playEffect(pathVec.at(chance).c_str());
}


void SoundConfig::playRandomIcing()
{
    int idx = rand()%3;
    
    this->playLoopEffect(StringUtils::format("sound/other/icing_0%d.mp3",idx+1).c_str());
    
}

void SoundConfig::playEffectWithCallback(string pszFilePath,float duration,function<void()> callback)
{
    kAudioUtil->playEffect(pszFilePath.c_str());
    Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(duration),
                                                                           CallFunc::create(callback),
                                                                           NULL));
}



