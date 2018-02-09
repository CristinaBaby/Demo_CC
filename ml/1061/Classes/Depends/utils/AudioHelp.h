//
//  AudioHelp.h
//  MACY013
//
//  Created by wuguiling on 6/27/14.
//
//

#ifndef __MACY013__AudioHelp__
#define __MACY013__AudioHelp__

#include <iostream>
#include "SingleTon.h"

using namespace std;

class AudioHelp:public SingleTon<AudioHelp>{
    
private:
    AudioHelp();
    ~AudioHelp();
    friend class SingleTon<AudioHelp>;
    
    const char* _getSoundPath(const char* name);
public:
    void setAudioVolumeMode(bool volumeMode = false){m_bVolumeMode = volumeMode;};
    
    bool getSoundIsOn(){return m_bEffectOpen;}
    void switchEffect();
    void switchSound();
    void setEffectVolume(float volume);
    void setSoundVolume(float volume);
    float getEffectVolume();
    float getSoundVolume();
    void playBackgroundMusic(const char* name);
    void StopBackgroundMusic();
    void PauseBackgroundMusic();
    void ResumeBackgroundMusic();
    
    void playEffect(const char* name);
    void playLoofEffectOnce(const char* name);
    void playLoofEffect(const char* name);
    void stopLoofEffect();
    void pauseLoofEffect();
    void resumeLoofEffect();
//    ***************** base ****************
    void playStartEffect();
    void playNextEffect();
    void playPreEffect();
    void playResetEffect();
    void playSelectEffect();
    void playCameraEffect();
    void playDeleteEffect();
//    *****************************************
    
//    ***************** car ********************
    void playWaterLoopEffect();
    void stopWaterEffect();
    
    void playBrushLoopEffect();
    void playBigBrushLoopEffect();
    void playSpongeLoopEffect();
    void playTowelLoopEffect();
    void playShowerLoopEffect();
    void playCarRunLoopEffect();
    
    void playCraneLoopEffect();
    
    void playWaxEffect();
    void playJumpEffect();
    void playPumpEffect();
    void playHonkEffect();
    
    void playChooseTruckEffect();
    void playCongratulationEffect();
    void playRightEffect();
    void playWrongEffect();
    
private:
    
    bool m_bEffectOpen;
    bool m_bMusicOpen;
    unsigned int m_nLoopId;
    bool m_bVolumeMode;
    float m_fMusicVolume;
    float m_fEffectVolume;
};
#endif /* defined(__MACY013__AudioHelp__) */
