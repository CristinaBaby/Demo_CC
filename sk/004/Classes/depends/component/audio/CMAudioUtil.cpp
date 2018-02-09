//
//  CMAudioUtil.cpp
//  tangbowen
//
//  Created by tangbowen on 14-8-12.
//
//

#include "depends/component/audio/CMAudioUtil.h"

const char* KEY_ALL_AUDIO       = "AllAudio";
const char* KEY_VOICE_AUDIO     = "VoiceAudio";
const char* KEY_BG_MUSIC        = "BgMusic";
const char* KEY_EFFECT_MUSIC    = "EffectMusic";
const char* KEY_BG_VOLUME       = "BgVolume";
const char* KEY_EFFECT_VOLUME   = "EffectVolume";

CMAudioUtil* g_audioUtil = nullptr;

CMAudioUtil::CMAudioUtil()
{
    _currentBgMusicName = "\0";
    _audioEngine = SimpleAudioEngine::getInstance();
}

CMAudioUtil::~CMAudioUtil()
{
    
}

CMAudioUtil* CMAudioUtil::getInstance()
{
    if (!g_audioUtil)
    {
        g_audioUtil = new CMAudioUtil();
        g_audioUtil->loadAudioConfig();
    }
    return g_audioUtil;
}

void CMAudioUtil::preloadBackgroundMusic(const char *pszFilePath)
{
    _audioEngine->preloadBackgroundMusic(pszFilePath);
}

void CMAudioUtil::preloadEffect(const char *pszFilePath)
{
    _audioEngine->preloadEffect(pszFilePath);
}

void CMAudioUtil::playBackGroundMusic(const string& pszFilePath)
{
    bool change = false;
    if (_currentBgMusicName != pszFilePath)
    {
        _currentBgMusicName = pszFilePath;
        change = true;
    }
    if (!_allAudio ||
        !_playBgMusic)
        return;
    if (_currentBgMusicName == "\0")
    {
        _audioEngine->stopBackgroundMusic();
        return;
    }
    if (change)
    {
        _audioEngine->stopBackgroundMusic();
        _audioEngine->playBackgroundMusic(_currentBgMusicName.c_str(), true);
    }
}

void CMAudioUtil::stopBackGroundMusic()
{
    _audioEngine->stopBackgroundMusic();
}

int CMAudioUtil::playEffect(const char *pszFilePath, bool bLoop)
{
    if (!_allAudio || !_playEffect)
    {
        return -1;
    }
    return _audioEngine->playEffect(pszFilePath, bLoop);
}

int CMAudioUtil::playEffectWithStop(const char *pszFilePath, bool bLoop, const unsigned int nSoundId)
{
    if (!_allAudio || !_playEffect)
    {
        return -1;
    }
    _audioEngine->stopEffect(nSoundId);
    return _audioEngine->playEffect(pszFilePath, bLoop);
}

void CMAudioUtil::stopEffectByPath(const string& path)
{
}

void CMAudioUtil::stopEffectBySoundId(unsigned int nSoundId)
{
    _audioEngine->stopEffect(nSoundId);
}

void CMAudioUtil::stopAllEffect()
{
    _audioEngine->stopAllEffects();
}

void CMAudioUtil::playRandomEffect(std::vector<std::string> audiosStr)
{
    int index = rand() % audiosStr.size();
    log("================>index:%d", index);
    this->playEffect(audiosStr[index].c_str(), false);
}

void CMAudioUtil::exitStopAndSaveConfig()
{
    _audioEngine->stopAllEffects();
    _audioEngine->stopBackgroundMusic();
    this->saveAudioConfig();
}

void CMAudioUtil::loadAudioConfig()
{
    _allAudio = userDefault->getBoolForKey(KEY_ALL_AUDIO, true);
    _voiceAudio = userDefault->getBoolForKey(KEY_VOICE_AUDIO, true);
    _playBgMusic = userDefault->getBoolForKey(KEY_BG_MUSIC, true);
    _playEffect = userDefault->getBoolForKey(KEY_EFFECT_MUSIC, true);
    _bgVolume = userDefault->getFloatForKey(KEY_BG_VOLUME, 1.0f);
    _effectVolume = userDefault->getFloatForKey(KEY_EFFECT_VOLUME, 1.0f);
    if (_voiceAudio)
    {
        _audioEngine->setBackgroundMusicVolume(_bgVolume);
        _audioEngine->setEffectsVolume(_voiceAudio);
    }
    else
    {
        _audioEngine->setBackgroundMusicVolume(0.0f);
        _audioEngine->setEffectsVolume(0.0f);
    }
}

void CMAudioUtil::saveAudioConfig()
{
    userDefault->setBoolForKey(KEY_ALL_AUDIO, _allAudio);
    userDefault->setBoolForKey(KEY_VOICE_AUDIO, _voiceAudio);
    userDefault->setBoolForKey(KEY_BG_MUSIC, _playBgMusic);
    userDefault->setBoolForKey(KEY_EFFECT_MUSIC, _playEffect);
    userDefault->setFloatForKey(KEY_BG_VOLUME,_bgVolume);
    userDefault->setFloatForKey(KEY_EFFECT_VOLUME, _effectVolume);
    userDefault->flush();
}

void CMAudioUtil::closeAllAudio()
{
    this->closeBgMusic();
    this->closeAllEffect();
    _allAudio = false;
    this->saveAudioConfig();
}

void CMAudioUtil::closeBgMusic()
{
    _audioEngine->stopBackgroundMusic();
    _currentBgMusicName = "\0";
    _playBgMusic = false;
    this->saveAudioConfig();
}

void CMAudioUtil::closeAllEffect()
{
    _audioEngine->stopAllEffects();
    _playEffect = false;
    this->saveAudioConfig();
}

void CMAudioUtil::openAllAudio()
{
    _allAudio = true;
    this->openBgMusic();
    this->openAllEffect();
    saveAudioConfig();
}

void CMAudioUtil::setBgMusicVolume(float value)
{
    if (!_voiceAudio)
        return;//通过音量大小关闭音效后，设置无效
    _bgVolume = MAX(MIN(1.0f,value),0.0f); //range [0,1]
    _audioEngine->setBackgroundMusicVolume(_bgVolume);
}

void CMAudioUtil::setEffectMusicVolume(float value)
{
    if (!_voiceAudio)
        return;//通过音量大小关闭音效后，设置无效
    _voiceAudio = MAX(MIN(1.0f,value),0.0f); //range [0,1]
    _audioEngine->setEffectsVolume(_voiceAudio);
}

void CMAudioUtil::switchAllAudio()
{
    _allAudio = !_allAudio;
    if (_allAudio)
    {
        _audioEngine->playBackgroundMusic(_currentBgMusicName.c_str(), true);
    }
    else
    {
        this->stopBackGroundMusic();
        this->stopAllEffect();
    }
    this->saveAudioConfig();
}

void CMAudioUtil::switchAllAudioByVoice()
{
    _voiceAudio = !_voiceAudio;
    if (_voiceAudio)
    {
        _audioEngine->setBackgroundMusicVolume(_bgVolume);
        _audioEngine->setEffectsVolume(_voiceAudio);
    }
    else
    {
        _audioEngine->setBackgroundMusicVolume(0.0f);
        _audioEngine->setEffectsVolume(0.0f);
    }
    this->saveAudioConfig();
}

bool CMAudioUtil::getAudioState() const
{
    if (!_allAudio)
    {
        return false;
    }
    if (!_voiceAudio)
    {
        return false;
    }
    return true;
}

void CMAudioUtil::openBgMusic()
{
    _playBgMusic = true;
    saveAudioConfig();
}

void CMAudioUtil::openAllEffect()
{
    _playEffect = true;
    saveAudioConfig();
}

void CMAudioUtil::pauseBackgroundMusic()
{
    _audioEngine->pauseBackgroundMusic();
}

void CMAudioUtil::resumeBackgroundMusic()
{
    if (!_allAudio || !_playBgMusic)
    {
        return;
    }
    _audioEngine->resumeBackgroundMusic();
}