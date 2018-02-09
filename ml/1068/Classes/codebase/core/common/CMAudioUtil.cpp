//
//  CMAudioUtil.cpp
//  tangbowen
//
//  Created by tangbowen on 14-8-12.
//
//

#include "CMAudioUtil.h"

CMAudioUtil* CMAudioUtil::m_pCMAudioUtil = nullptr;

CMAudioUtil::CMAudioUtil()
{
    m_cCurBgMusicName = "";
    m_pAudioEngine = SimpleAudioEngine::getInstance();
}

CMAudioUtil::~CMAudioUtil()
{
    
}

CMAudioUtil* CMAudioUtil::getInstance()
{
    if (!m_pCMAudioUtil) {
        m_pCMAudioUtil = new CMAudioUtil();
        m_pCMAudioUtil->loadAudioConfig();
    }
    return m_pCMAudioUtil;
}

void CMAudioUtil::preloadBackgroundMusic(const char *pszFilePath)
{
    m_pAudioEngine->preloadBackgroundMusic(pszFilePath);
}

void CMAudioUtil::preloadEffect(const char *pszFilePath)
{
    m_pAudioEngine->preloadEffect(pszFilePath);
}

void CMAudioUtil::playBackGroundMusic(const string& pszFilePath)
{
    bool change = false;
    if (m_cCurBgMusicName != pszFilePath)
    {
        m_cCurBgMusicName = pszFilePath;
        change = true;
    }
    if (!m_bAllAudio ||
        !m_bPlayBgMusic)
        return;
    if (m_cCurBgMusicName == "")
    {
        m_pAudioEngine->stopBackgroundMusic();
        return;
    }
    if (change)
    {
        m_pAudioEngine->stopBackgroundMusic();
        m_pAudioEngine->playBackgroundMusic(m_cCurBgMusicName.c_str(), true);
    }
}

void CMAudioUtil::stopBackGroundMusic()
{
    m_pAudioEngine->stopBackgroundMusic();
}

int CMAudioUtil::playEffect(const char *pszFilePath, bool bLoop)
{
    if (!m_bAllAudio || !m_bPlayEffect) {
        return -1;
    }
    return m_pAudioEngine->playEffect(pszFilePath, bLoop);
}

int CMAudioUtil::playEffectWithStop(const char *pszFilePath, bool bLoop, const unsigned int nSoundId)
{
    if (!m_bAllAudio || !m_bPlayEffect) {
        return -1;
    }
    m_pAudioEngine->stopEffect(nSoundId);
    return m_pAudioEngine->playEffect(pszFilePath, bLoop);
}

void CMAudioUtil::stopEffectByPath(const string& path)
{
//    m_pAudioEngine->stope
}

void CMAudioUtil::stopEffectBySoundId(unsigned int nSoundId)
{
    m_pAudioEngine->stopEffect(nSoundId);
}

void CMAudioUtil::stopAllEffect()
{
    m_pAudioEngine->stopAllEffects();
}

void CMAudioUtil::playRandomEffect(std::vector<std::string> audiosStr)
{
    int index = rand() % audiosStr.size();
    log("================>index:%d", index);
    this->playEffect(audiosStr[index].c_str(), false);
}

void CMAudioUtil::exitStopAndSaveConfig()
{
    m_pAudioEngine->stopAllEffects();
    m_pAudioEngine->stopBackgroundMusic();
    this->saveAudioConfig();
}

void CMAudioUtil::loadAudioConfig()
{
    m_bAllAudio = userDefault->getBoolForKey("AllAudio", true);
    m_bVoiceAudio = userDefault->getBoolForKey("VoiceAudio", true);
    m_bPlayBgMusic = userDefault->getBoolForKey("BgMusic", true);
    m_bPlayEffect = userDefault->getBoolForKey("EffectMusic", true);
    if (m_bVoiceAudio)
    {
        m_pAudioEngine->setBackgroundMusicVolume(1.0f);
        m_pAudioEngine->setEffectsVolume(1.0f);
    }else{
        m_pAudioEngine->setBackgroundMusicVolume(0.0f);
        m_pAudioEngine->setEffectsVolume(0.0f);
    }

}

void CMAudioUtil::saveAudioConfig()
{
    userDefault->setBoolForKey("AllAudio", m_bAllAudio);
    userDefault->setBoolForKey("VoiceAudio", m_bVoiceAudio);
    userDefault->setBoolForKey("BgMusic", m_bPlayBgMusic);
    userDefault->setBoolForKey("EffectMusic", m_bPlayEffect);
    userDefault->flush();
}


void CMAudioUtil::closeAllAudio()
{
    this->closeBgMusic();
    this->closeAllEffect();
    m_bAllAudio = false;
    this->saveAudioConfig();
}

void CMAudioUtil::closeBgMusic()
{
    m_pAudioEngine->stopBackgroundMusic();
    m_cCurBgMusicName = "";
    m_bPlayBgMusic = false;
    this->saveAudioConfig();
}

void CMAudioUtil::closeAllEffect()
{
    m_pAudioEngine->stopAllEffects();
    m_bPlayEffect = false;
    this->saveAudioConfig();
}

void CMAudioUtil::openAllAudio()
{
    m_bAllAudio = true;
    this->openBgMusic();
    this->openAllEffect();
    saveAudioConfig();
}

void CMAudioUtil::switchAllAudio()
{
    m_bAllAudio = !m_bAllAudio;
    if (m_bAllAudio) {
        m_pAudioEngine->playBackgroundMusic(m_cCurBgMusicName.c_str(), true);
    }else{
        this->stopBackGroundMusic();
        this->stopAllEffect();
    }
    this->saveAudioConfig();
}

void CMAudioUtil::switchAllAudioByVoice()
{
    m_bVoiceAudio = !m_bVoiceAudio;
    if (m_bVoiceAudio)
    {
        m_pAudioEngine->setBackgroundMusicVolume(1.0f);
        m_pAudioEngine->setEffectsVolume(1.0f);
    }else{
        m_pAudioEngine->setBackgroundMusicVolume(0.0f);
        m_pAudioEngine->setEffectsVolume(0.0f);
    }
    this->saveAudioConfig();
}

bool CMAudioUtil::getAudioState()
{
    if (!m_bAllAudio)
    {
        return false;
    }
    if (!m_bVoiceAudio)
    {
        return false;
    }
    return true;
}

void CMAudioUtil::openBgMusic()
{
    m_bPlayBgMusic = true;
    saveAudioConfig();
}

void CMAudioUtil::openAllEffect()
{
    m_bPlayEffect = true;
    saveAudioConfig();
}

void CMAudioUtil::pauseBackgroundMusic()
{
    m_pAudioEngine->pauseBackgroundMusic();
}

void CMAudioUtil::resumeBackgroundMusic()
{
    if (!m_bAllAudio || !m_bPlayBgMusic) {
        return;
    }
    m_pAudioEngine->resumeBackgroundMusic();
}