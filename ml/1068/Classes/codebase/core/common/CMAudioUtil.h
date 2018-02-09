//
//  CMAudioUtil.h
//  tangbowen
//
//  Created by tangbowen on 14-8-12.
//
//

#ifndef CODEBASE_CORE_COMMON_CMAUDIOUTIL_H_
#define CODEBASE_CORE_COMMON_CMAUDIOUTIL_H_
#include <iostream>
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

USING_NS_CC;

using namespace CocosDenshion;
using namespace std;

#define         userDefault             UserDefault::getInstance()
#define         kAudioUtil              CMAudioUtil::getInstance()

class CMAudioUtil {

private:
    CMAudioUtil();
    
public:
    virtual ~CMAudioUtil();

public:
    static CMAudioUtil*   m_pCMAudioUtil;
    static CMAudioUtil*   getInstance();
    
protected:
    bool    m_bAllAudio;
    bool    m_bVoiceAudio;
    bool    m_bPlayBgMusic;
    bool    m_bPlayEffect;
public:
    SimpleAudioEngine* m_pAudioEngine;
    string  m_cCurBgMusicName;
public:
    void    closeAllAudio();
    void    closeBgMusic();
    void    closeAllEffect();
    void    openAllAudio();
    void    openBgMusic();
    void    openAllEffect();
    
public:
    /**/
    void    switchAllAudio();
    /*通过控制音量来关闭音效*/
    void    switchAllAudioByVoice();
    /*获取当前音效的开关状态*/
    bool    getAudioState();
    
    bool    getBgMusicState() const
    {
        return m_bPlayBgMusic;
    }
    
    bool    getEffectState() const
    {
        return m_bPlayEffect;
    }
public:
    void    preloadBackgroundMusic(const char *pszFilePath);
    void    preloadEffect(const char *pszFilePath);
    void    playBackGroundMusic(const string& pszFilePath);
    void    stopBackGroundMusic();
    void    exitStopAndSaveConfig();
    int     playEffect(const char* pszFilePath, bool bLoop = false);
    int     playEffectWithStop(const char* pszFilePath, bool bLoop, const unsigned int nSoundId);
    void    stopEffectBySoundId(unsigned int nSoundId);
    void    stopEffectByPath(const string& path);
    void    stopAllEffect();
    void    playRandomEffect(std::vector<std::string> audiosStr);
    
public:
    void    pauseBackgroundMusic();
    void    resumeBackgroundMusic();
    
public:
    void    loadAudioConfig();
    void    saveAudioConfig();
    
};

CC_DEPRECATED_ATTRIBUTE typedef CMAudioUtil AudioUtil;

#endif /* defined(CODEBASE_CORE_COMMON_CMAUDIOUTIL_H_) */
