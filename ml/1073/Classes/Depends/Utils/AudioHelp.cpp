//
//  AudioHelp.cpp
//  Doctor
//
//  Created by tanshoumei on 13-8-23.
//
//

#include "AudioHelp.h"
//#include "MathUtil.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;
using namespace std;

#define RoleCount 4

AudioHelp::AudioHelp():
bgMusic("nomusic"),
pourmilkID(0),
pourcerealID(0),
loopID(0)
{
    
    canPlaySound = UserDefault::getInstance()->getBoolForKey("isSoundOpen", true);
    
    canPlayMusic = UserDefault::getInstance()->getBoolForKey("isMusicOpen", true);
    
    if (canPlayMusic) {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.8f);
    }else {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
    }
    if (canPlaySound) {
        SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
    }else {
        SimpleAudioEngine::getInstance()->setEffectsVolume(0.0f);
    }
    
}

void AudioHelp::switchMusic(){
    canPlayMusic = !canPlayMusic;
    
    UserDefault::getInstance()->setBoolForKey("isMusicOpen", canPlayMusic);
    UserDefault::getInstance()->flush();
    if (canPlayMusic) {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5f);
    }else {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
    }
    if (canPlaySound) {
        SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
    }else {
        SimpleAudioEngine::getInstance()->setEffectsVolume(0.0f);
    }
}
void AudioHelp::switchSound(){
    canPlaySound = !canPlaySound;
    UserDefault::getInstance()->setBoolForKey("isSoundOpen", canPlaySound);
    UserDefault::getInstance()->flush();
    if (canPlayMusic) {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5f);
    }else {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
    }
    if (canPlaySound) {
        SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
    }else {
        SimpleAudioEngine::getInstance()->setEffectsVolume(0.0f);
    }
}

void AudioHelp::switchVolume(){
    switchMusic();
    switchSound();
}

bool AudioHelp::isMusicOpen(){
    return canPlayMusic;
}
bool AudioHelp::isSoundOpen(){
    return canPlaySound;
}

#pragma mark 背景音效播放
void AudioHelp::playBackGroundMusic(const char *bgMusic) {
    std::stringstream s;
    s<<"sound/"<<bgMusic;
    if (this->bgMusic.compare(s.str()) == 0) {
        return;
    }
    this->bgMusic = s.str();
    if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }
    SimpleAudioEngine::getInstance()->playBackgroundMusic(s.str().c_str(), true);
    
}

void AudioHelp::stopBackGroundMusic(){
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}


void AudioHelp::pauseBackGroundMusic()
{
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void AudioHelp::resumeBackGroundMusic(){
    
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AudioHelp::stopAlleffect(){
    pourcerealID = 0;
    pourmilkID = 0;
    SimpleAudioEngine::getInstance()->stopAllEffects();
}

void AudioHelp::pauseEffect(){
    if (pourcerealID != 0) {
        SimpleAudioEngine::getInstance()->pauseEffect(pourcerealID);
    }
    if (pourmilkID != 0) {
        SimpleAudioEngine::getInstance()->pauseEffect(pourmilkID);
    }
    
    SimpleAudioEngine::getInstance()->pauseAllEffects();
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void AudioHelp::resumeEffect(){
    if (pourcerealID != 0) {
        SimpleAudioEngine::getInstance()->resumeEffect(pourcerealID);
    }
    if (pourmilkID != 0) {
        SimpleAudioEngine::getInstance()->resumeEffect(pourmilkID);
    }
    SimpleAudioEngine::getInstance()->resumeAllEffects();
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AudioHelp::pauseLoopEffect()
{
    SimpleAudioEngine::getInstance()->pauseEffect(loopID);
}
void AudioHelp::resumeLoopEffect()
{
    SimpleAudioEngine::getInstance()->resumeEffect(loopID);
}
#pragma mark 基本音效播放

void AudioHelp::playHomeEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/basic/select.mp3");
}

void AudioHelp::playNextEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/basic/next.mp3");
}

void AudioHelp::playPreviousEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/basic/previous.mp3");
}

void AudioHelp::playResetEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/basic/reset.mp3");
}

void AudioHelp::playStartEffect(){
    SimpleAudioEngine::getInstance()->playEffect("sound/basic/start.mp3");
}

void AudioHelp::playSelectedEffect()
{
    SimpleAudioEngine::getInstance()->playEffect("sound/basic/select.mp3");
}

void AudioHelp::playDeleteEffect()
{
    SimpleAudioEngine::getInstance()->playEffect("sound/basic/delete.mp3");
}

void AudioHelp::playAddEffect()
{
    SimpleAudioEngine::getInstance()->playEffect("sound/basic/add.mp3");
}

void AudioHelp::playCameraEffect()
{
    SimpleAudioEngine::getInstance()->playEffect("sound/basic/camera.mp3");
}

static int DrinkId = 0;

void AudioHelp::playDrinkEffect()
{
    if (DrinkId>0) {
        return;
    }
    DrinkId = SimpleAudioEngine::getInstance()->playEffect("sound/basic/drink.mp3", true);
}

void AudioHelp::stopDrinkEffect()
{
    SimpleAudioEngine::getInstance()->stopEffect(DrinkId);
    DrinkId = 0;
}

void AudioHelp::playLoopEffect(const string &effect) {
    if (loopID) {
        return;
    }
    loopID = SimpleAudioEngine::getInstance()->playEffect(_getPath(effect).c_str(), true);
}

void AudioHelp::playEffect(std::string name)
{
    int index = _playEffect(name);
    auto it = m_EffectMap.find(name);
    log("=%s==%s==%d",__func__,name.c_str(),index);
    if (it!=m_EffectMap.end()){
        it->second = Value(index);
    }else{
        m_EffectMap.insert(ValueMap::value_type(name,Value(index)));
    }
    it = m_EffectMap.find(name);
    log("===%s==",it->second.asString().c_str());
    m_SceneVector.push_back(Value(name));
}

void AudioHelp::stopEffect(std::string name)
{
    auto it = m_EffectMap.find(name);
    if (it!=m_EffectMap.end()) {
        int effectID = it->second.asInt();
        log("=%s==%s==%d",__func__,name.c_str(),effectID);
        if (effectID) {
            SimpleAudioEngine::getInstance()->stopEffect(effectID);
            effectID = 0;
            it->second = Value(0);
        }
    }
}
void AudioHelp::stopLoopEffect()
{
    if (loopID) {
        SimpleAudioEngine::getInstance()->stopEffect(loopID);
        loopID = 0;
    }
}

void AudioHelp::registerEffectScene(std::string name)
{
    log("===%s",name.c_str());
    if (m_SceneVector.size()>0) {
        m_PreSceneVector.clear();
        m_PreSceneVector.operator=(m_SceneVector);
    }
    m_SceneVector.clear();
}
void AudioHelp::removeEffectScene(std::string name)
{
    for_each(m_PreSceneVector.begin(), m_PreSceneVector.end(), [=](Value data){
        stopEffect(data.asString());
    });
}
#pragma mark 项目需要音效

void AudioHelp::playMapButtonEffect()
{
    _playEffect("button.mp3");
}

void AudioHelp::playWrongEffect()
{
    _playEffect("puzzle_wrong.mp3");
}

void AudioHelp::playRightEffect()
{
    _playEffect("puzzle_right.mp3");
}


static int gFryId = 0;
void AudioHelp::playFryEffect()
{
    if (gFryId) {
        return;
    }
    gFryId = _playLoopEffect("fry.mp3");
}

void AudioHelp::stopFryEffect()
{
    if (gFryId) {
        SimpleAudioEngine::getInstance()->stopEffect(gFryId);
        gFryId = 0;
    }
}

static int gBurnedId = 0;
bool AudioHelp::playFryBurnedEffect()
{
    if (gBurnedId) {
        return false;
    }
    gBurnedId = _playEffect("vo_burned.mp3");
    return true;
}
void AudioHelp::stopFryBurnedEffect()
{
    
    if (gBurnedId) {
        SimpleAudioEngine::getInstance()->stopEffect(gBurnedId);
        gBurnedId = 0;
    }
}
void AudioHelp::playBabyEatEffect()
{
    _playEffect("baby_eat.mp3");
    
}
void AudioHelp::playPourMilkEffect()
{
    _playEffect("add_water.mp3");
}

static int gPourWaterId = 0;
void AudioHelp::playPourWaterEffect()
{
    if (gPourWaterId) {
        return;
    }
    gPourWaterId = _playEffect("add_water.mp3");
}

void AudioHelp::stopPourWaterEffect()
{
    if (gPourWaterId) {
        SimpleAudioEngine::getInstance()->stopEffect(gPourWaterId);
        gPourWaterId = 0;
    }
}
static int gSodaMachineId = 0;
void AudioHelp::playSodaMachineEffect()
{
    if (gSodaMachineId) {
        return;
    }
    gSodaMachineId = SimpleAudioEngine::getInstance()->playEffect("sound/soda2.mp3", true);
    
}
void AudioHelp::stopSodaMachineEffect()
{
    if (gSodaMachineId) {
        SimpleAudioEngine::getInstance()->stopEffect(gSodaMachineId);
        gSodaMachineId = 0;
    }
    
}
void AudioHelp::playPourPowderEffect()
{
    _playEffect("add_powder.mp3");
}

void AudioHelp::playAddEggEffect()
{
    _playEffect("add_egg.mp3");
}

void AudioHelp::playOKEffect()
{
    _playEffect("corndogOK.mp3");
}
void AudioHelp::playCutEffect()
{
    _playEffect("cut.mp3");
}
#pragma mark private

int AudioHelp::_playEffect(std::string name)
{
    log("========Volume=====%f=======", SimpleAudioEngine::getInstance()->getEffectsVolume());
    return SimpleAudioEngine::getInstance()->playEffect(std::string("sound/"+name).c_str());
}

int AudioHelp::_playLoopEffect(std::string name)
{
    return SimpleAudioEngine::getInstance()->playEffect(std::string("sound/"+name).c_str(),true);
}