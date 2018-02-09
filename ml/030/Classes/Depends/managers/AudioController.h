//
//  AudioController.h
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-8-26.
//
//

#ifndef __MakeSnowMan__AudioController__
#define __MakeSnowMan__AudioController__

#include <iostream>
#include "./extensions/cocos-ext.h"
#include "ui/cocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;

enum
{
    kButtonAudio = 10,
    kButtonMusic,
    kButtonSound,
};

class AudioController
{
public:
    static AudioController* getInstance();
    void init();
    void stopEffect();
    void playBackGroundMusic();
    void playJumpEffect();
    void playFallEffect();
    void playStartEffect();
    void playNextEffect();
    void playSlideEffect();
    void playChosenEffect();
    void playResetEffect();
    void playPreviousEffect();
    void playDeleteEffect();
    void playFaucetEffect();
    
    //effect
    void playNoseEffect();
    void playFlowerEffect();
    void playPepperEffect();
    void playExplodeEffect();
    void playPourEffect();
    void playFreezeEffect();
    void playSnowEffect();
    void playRecorderEffect();
    void playLightFireEffect();
    void playFireworkEffect();
    void playKissEffect();
    void playUnFreezeEffect();
    void playWaveEffect();
    
    ui::Button* initWithAudioBtn();
    ui::Button* initWithSoundBtn();
    ui::Button* initWithMusicBtn();
private:
    void touchEvent(Ref *obj , ui::Widget::TouchEventType type);
    void switchBtnStatus(int eType);
    void checkFirstLaunch();
    void muteAudio(int eType);
    void resumAudio(int eType);
    ui::Button* audioBtn_;
    ui::Button* soundBtn_;
    ui::Button* musicBtn_;
    
    CC_SYNTHESIZE(float, effctVolume_, EffectVolume);
    CC_SYNTHESIZE(float, musicVolume_, MusicVolume);
    
    void updateMusic();
};

#endif /* defined(__MakeSnowMan__AudioController__) */
