//
//  UserDataManager.cpp
//  Doctor2
//
//  Created by tanshoumei on 13-9-26.
//
//

#include "UserDataManager.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

#define kGameSound "kGameSound"
#define kGameMusic "kGameMusic"

void UserDataManager::openMusic()
{
    SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
    UserDefault::getInstance()->setBoolForKey(kGameMusic, false);
    UserDefault::getInstance()->flush();
}

void UserDataManager::closeMusic()
{
    SimpleAudioEngine::getInstance()->setEffectsVolume(0);
    UserDefault::getInstance()->setBoolForKey(kGameMusic, true);
    UserDefault::getInstance()->flush();
}

bool UserDataManager::isOpenMusic()
{
    bool isOpen = UserDefault::getInstance()->getBoolForKey(kGameMusic);
    return !isOpen;
}

void UserDataManager::openSound(const char *pBGMusic)
{
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0f);
    UserDefault::getInstance()->setBoolForKey(kGameSound, false);
    UserDefault::getInstance()->flush();
    if (strlen(pBGMusic) > 0) {
        SimpleAudioEngine::getInstance()->playBackgroundMusic(pBGMusic, true);
    }
}

void UserDataManager::closeSound()
{
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
    UserDefault::getInstance()->setBoolForKey(kGameSound, true);
    UserDefault::getInstance()->flush();
    SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}

bool UserDataManager::isOpenSound()
{
    bool isOpen = UserDefault::getInstance()->getBoolForKey(kGameSound);
    return !isOpen;
}