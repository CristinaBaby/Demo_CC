
#ifndef __AudioHelp__
#define __AudioHelp__

#include <iostream>
#include <sstream>
#include "SingleTon.h"
#include "cocos2d.h"

class AudioHelp: public SingleTon<AudioHelp>
{
    friend class SingleTon<AudioHelp>;
private:
    AudioHelp();
public:
    //背景音乐
    void switchMusic();
    void switchSound();
    void switchVolume();
    bool isMusicOpen();
    bool isSoundOpen();
#pragma mark 背景音效播放
    void playBackGroundMusic(const char* bgMusic);
    void stopBackGroundMusic();
    void pauseBackGroundMusic();
    void resumeBackGroundMusic();
    
    void stopAlleffect();
    void pauseEffect();
    void resumeEffect();
    void pauseLoopEffect();
    void resumeLoopEffect();
#pragma mark 基本音效比如：返回，首页，下一步等等
    void playResetEffect();
    void playNextEffect();
    void playHomeEffect();
    void playStartEffect();
    void playPreviousEffect();
    void playSelectedEffect();
    void playDeleteEffect();
    void playAddEffect();
    void playCameraEffect();
    void playDrinkEffect();
    void stopDrinkEffect();
    
    void playLoopEffect(const std::string& effect);
    void stopLoopEffect();
    
    void playEffect(std::string name);
    void stopEffect(std::string name);
    void registerEffectScene(std::string name);
    void removeEffectScene(std::string name);
    void resetEffect(){
        m_PreSceneVector.clear();
    }
#pragma mark 项目需要音效
    void playMapButtonEffect();
    
    void playWrongEffect();
    void playRightEffect();
    
    void playFryEffect();
    void stopFryEffect();
    
    void playBabyEatEffect();
    
    
    void playPourMilkEffect();
    void playPourWaterEffect();
    void stopPourWaterEffect();
    void playPourPowderEffect();
    void playAddEggEffect();
    void playOKEffect();
    void playCutEffect();
    //    void playShakeEffect();
    
    void playSodaMachineEffect();
    void stopSodaMachineEffect();
private:
    unsigned int loopID;
    unsigned int pourmilkID;
    unsigned int pourcerealID;
    std::string bgMusic;
    bool canPlayMusic;
    bool canPlaySound;
    
    cocos2d::ValueMap m_EffectMap;
    cocos2d::ValueVector m_SceneVector;
    cocos2d::ValueVector m_PreSceneVector;
    
    //    bool haveVolume;
    
    int _playEffect(std::string name);
    int _playLoopEffect(std::string name);
    std::string  _getPath(std::string name){return "sound/"+name;};
};

#define AudioHelpInstance AudioHelp::getInstance()

#endif /* defined(__Doctor__AudioHelp__) */
