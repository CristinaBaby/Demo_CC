//
//  AudioController.cpp
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-8-26.
//
//

#include "AudioController.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#define isAudioOn_ UserDefault::getInstance() -> getBoolForKey(kAudioStatus)
#define isSoundOn_ UserDefault::getInstance() -> getBoolForKey(kSoundStatus)
#define isMusicOn_ UserDefault::getInstance() -> getBoolForKey(kMusicStatus)
#define kAudioStatus "audioStatus"
#define kSoundStatus "soundStatus"
#define kMusicStatus "musicStatus"
#define kMusicValue "musicValue"
#define kSoundValue "soundValue"

static AudioController *instance = NULL;

AudioController* AudioController::getInstance()
{
    if (instance == NULL)
    {
        instance = new AudioController();
    }
    return instance;
}

void AudioController::playBackGroundMusic()
{
    SimpleAudioEngine::getInstance() -> playBackgroundMusic("sound/bg_music.mp3",true);
}

void AudioController::playBackGroundMusic(const char* pFileName)
{
    SimpleAudioEngine::getInstance() -> playBackgroundMusic(pFileName,true);
}

unsigned int loopIdd = 0;
void AudioController::playLoopEffect(const char* pFileName)
{
    if (!loopIdd)
    {
        loopIdd = CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect(pFileName,true);
    }
    else
    {
        CocosDenshion::SimpleAudioEngine::getInstance() -> resumeEffect(loopIdd);
    }
}

void AudioController::stopLoopEffect()
{

    CocosDenshion::SimpleAudioEngine::getInstance() -> stopEffect(loopIdd);
    loopIdd = 0;
}



ui::Button* AudioController::initWithAudioBtn()
{
    audioBtn_ = ui::Button::create();
    if (isAudioOn_)
    {
        audioBtn_ -> loadTextureNormal("start/btn_sound.png");
    }
    else
    {
        audioBtn_ -> loadTextureNormal("start/btn_sound_h.png");
    }
    audioBtn_ -> addTouchEventListener(CC_CALLBACK_2(AudioController::touchEvent, this));
    audioBtn_ -> setTag(kButtonAudio);
    return audioBtn_;
}

ui::Button* AudioController::initWithMusicBtn()
{
    musicBtn_ = ui::Button::create();
    if (isMusicOn_)
    {
        musicBtn_ -> loadTextureNormal("button/setting_on.png");
    }
    else
    {
        musicBtn_ -> loadTextureNormal("button/setting_off.png");
    }
    musicBtn_ -> addTouchEventListener(CC_CALLBACK_2(AudioController::touchEvent, this));
    musicBtn_ -> setTag(kButtonMusic);
    musicBtn_ -> setPressedActionEnabled(false);

    return musicBtn_;
}

ui::Button* AudioController::initWithSoundBtn()
{
    soundBtn_ = ui::Button::create();
    if (isSoundOn_)
    {
        soundBtn_ -> loadTextureNormal("button/setting_on.png");
    }
    else
    {
        soundBtn_ -> loadTextureNormal("button/setting_off.png");
    }
    soundBtn_ -> addTouchEventListener(CC_CALLBACK_2(AudioController::touchEvent, this));
    soundBtn_ -> setTag(kButtonSound);
    soundBtn_ -> setPressedActionEnabled(false);
    return soundBtn_;
}



void AudioController::touchEvent(Ref *obj , ui::Widget::TouchEventType type)
{
    auto btn = dynamic_cast<ui::Button*>(obj);
    
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            switchBtnStatus(btn -> getTag());
        }
            break;
            
        default:
            break;
    }

}

void AudioController::switchBtnStatus(int eType)
{
    
    switch (eType) {
        case kButtonAudio:
        {
            if (isAudioOn_)
            {
                audioBtn_ -> loadTextureNormal("button/btn_sound_h.png");
                UserDefault::getInstance() -> setBoolForKey(kAudioStatus, false);
                muteAudio(eType);
            }
            else
            {
                audioBtn_ -> loadTextureNormal("button/btn_sound.png");
                UserDefault::getInstance() -> setBoolForKey(kAudioStatus, true);
                resumAudio(eType);
            }
        
        }
            break;
        case kButtonMusic:
        {
            if (isMusicOn_)
            {
                musicBtn_ -> loadTextureNormal("button/setting_off.png");
                UserDefault::getInstance() -> setBoolForKey(kMusicStatus, false);
                muteAudio(eType);
            }
            else
            {
                musicBtn_ -> loadTextureNormal("button/setting_on.png");
                UserDefault::getInstance() -> setBoolForKey(kMusicStatus, true);
                resumAudio(eType);
            }
            
        }
            break;
        case kButtonSound:
        {
            if (isSoundOn_)
            {
                soundBtn_ -> loadTextureNormal("button/setting_off.png");
                UserDefault::getInstance() -> setBoolForKey(kSoundStatus, false);
                muteAudio(eType);
            }
            else
            {
                soundBtn_ -> loadTextureNormal("button/setting_on.png");
                UserDefault::getInstance() -> setBoolForKey(kSoundStatus, true);
                resumAudio(eType);
            }
            
        }
            break;
        default:
            break;
    }
}

void AudioController::init()
{

    
    checkFirstLaunch();
    if (UserDefault::getInstance() -> getBoolForKey("firstLaunchA"))
    {
        UserDefault::getInstance() -> setBoolForKey(kAudioStatus, true);
        UserDefault::getInstance() -> setBoolForKey(kMusicStatus, true);
        UserDefault::getInstance() -> setBoolForKey(kSoundStatus, true);
        musicVolume_ = 1.f;
        effctVolume_ = 1.f;
        UserDefault::getInstance() -> setFloatForKey(kMusicValue, musicVolume_);
        UserDefault::getInstance() -> setFloatForKey(kSoundValue, effctVolume_);

    }
    if(!isAudioOn_)
    {
        muteAudio(kButtonAudio);
    }
    if(!isMusicOn_)
    {
        muteAudio(kButtonMusic);
    }
    if(!isSoundOn_)
    {
        muteAudio(kButtonSound);
    }
//    musicVolume_ = UserDefault::getInstance() -> getFloatForKey(kMusicValue);
//    effctVolume_ = UserDefault::getInstance() -> getFloatForKey(kSoundValue);
//    SimpleAudioEngine::getInstance() -> setBackgroundMusicVolume(musicVolume_);
//    SimpleAudioEngine::getInstance() -> setEffectsVolume(effctVolume_);

}

void AudioController::updateMusic()
{

    UserDefault::getInstance() -> setFloatForKey(kMusicValue, musicVolume_);
    UserDefault::getInstance() -> setFloatForKey(kSoundValue, effctVolume_);
    
}

void AudioController::checkFirstLaunch()
{
    if (!UserDefault::getInstance() -> getBoolForKey("everLaunchA"))
    {
        UserDefault::getInstance() -> setBoolForKey("everLaunchA", true);
        UserDefault::getInstance() -> setBoolForKey("firstLaunchA", true);
    }
    else
        UserDefault::getInstance() -> setBoolForKey("firstLaunchA", false);
}

void AudioController::muteAudio(int eType)
{
    
    switch (eType) {
        case kButtonAudio:
        {
            SimpleAudioEngine::getInstance() -> setBackgroundMusicVolume(0);
            SimpleAudioEngine::getInstance() -> setEffectsVolume(0);
        }
            break;
        case kButtonMusic:
        {
            SimpleAudioEngine::getInstance() -> setBackgroundMusicVolume(0);
        }
            break;
        case kButtonSound:
        {
            SimpleAudioEngine::getInstance() -> setEffectsVolume(0);
        }
            break;
    }
}

void AudioController::resumAudio(int eType)
{
    switch (eType) {
        case kButtonAudio:
        {
            SimpleAudioEngine::getInstance() -> setBackgroundMusicVolume(1);
            SimpleAudioEngine::getInstance() -> setEffectsVolume(1);
        }
            break;
        case kButtonMusic:
        {
            SimpleAudioEngine::getInstance() -> setBackgroundMusicVolume(1);
        }
            break;
        case kButtonSound:
        {
            SimpleAudioEngine::getInstance() -> setEffectsVolume(1);
        }
            break;
    }
}
void AudioController::stopEffect()
{
    SimpleAudioEngine::getInstance() -> stopAllEffects();
}

void AudioController::playFaucetEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("water_shuilogntou.mp3");
}

void AudioController::playJumpEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("monster_jump.mp3");
}

void AudioController::playFallEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("casedrop.mp3");
}

void AudioController::playStartEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("play.mp3");
}

void AudioController::playNextEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("next page.mp3");
}

void AudioController::playSlideEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("slide.mp3");
}

void AudioController::playChosenEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("choose.mp3");
}

void AudioController::playNoseEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("nose.mp3");
}

void AudioController::playFlowerEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("flower.mp3");
}

void AudioController::playPepperEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("pepper.mp3");
}

void AudioController::playExplodeEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("open the bottle.mp3");
}

void AudioController::playPourEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("soda2.mp3");
}

void AudioController::playFreezeEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("freezing.mp3");
}

void AudioController::playSnowEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("snow_wind.mp3");
}

void AudioController::playRecorderEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("speaker.mp3");
}

void AudioController::playLightFireEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("party firework.mp3");

}
void AudioController::playFireworkEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("firework2.mp3");
}

void AudioController::playKissEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("kiss2.mp3");

}

void AudioController::playUnFreezeEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("melting.mp3");

}
void AudioController::playResetEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("reset.mp3");

}

void AudioController::playPreviousEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("previous page.mp3");

}

void AudioController::playDeleteEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("delete.mp3");
    
}

void AudioController::playWaveEffect()
{
    SimpleAudioEngine::getInstance() -> playEffect("seawave.wav", true);
    
}