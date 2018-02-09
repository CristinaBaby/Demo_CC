//
//  TalkManager.cpp
//  ColorBook0818
//
//  Created by huangwen on 15/10/14.
//
//

#include "TalkManager.hpp"
#include "UITools.h"
#include "AudioHelper.h"
#include "ArmatureManager.h"
#include "CSVParse.h"

#define     TALk_TIME      5.7

TalkManager* TalkManager::_instance = nullptr;

TalkManager* TalkManager::getTalkManger(){
    if (!_instance) {
        _instance = new (std::nothrow)TalkManager();
    }
    return _instance;
}

TalkManager::TalkManager(){
    
    CSVParse* csv = CSVParse::create("talk.csv");
    int rows = csv->getRows();
    
    for (int r = 0; r < rows; r++)
    {
        TalkKey talkKey;
        talkKey.key = csv->getData(r, 0);
        if (!talkKey.key.empty()) {
            string str = csv->getData(r, 1);
            talkKey.time = std::stof(csv->getData(r, 1));
            _talkKey.push_back(talkKey);
        }
    }
    
    xScheduler->scheduleUpdate(this, 0, false);
}

TalkManager::~TalkManager(){
    xScheduler->unscheduleAllForTarget(this);
}

//操作全部放到计时器里面操作, 这里只是让计时器启动.
void TalkManager::playTalk(){
    
    switch (_mode) {
        case Mode::Normal:
        {
            if (_armature)
            {
                //随便放一句, 放完后, 就switch
                int index = quickRandom(0, 10);
                while (hasBeenPlay(index)) {
                    index = quickRandom(0, 10);
                }
                _randomPlayed.push_back(index);
                if (_randomPlayed.size() > 9) {
                    _randomPlayed.clear();
                }
                
                xAudio->playSound("random/" + _talkKey.at(index).key + ".mp3");
                _terminalTime = _talkKey.at(index).time + 1;
                startTiming();
            }
        }
            break;
        case Mode::Intro:
        {
            startTiming();
        }
            break;
        case Mode::WelcomeBack:
        {
            if (!_started)
            {
                string name;
                switch (getCurrentTime()) {
                    case Time::Morning:
                    {
                        name = "sound_goodmorning";
                    }
                        break;
                    case Time::Afternoon:
                    {
                        name = "sound_goodafternoon";
                    }
                        break;
                    case Time::Evening:
                    {
                        name = "sound_googevening";
                    }
                        break;
                    default:
                        break;
                }
                
                _soundID = xAudio->playSound(string("welcomeback/" + name + ".mp3").c_str());
                _terminalTime = getSoundTime(name) + 1;
                
                startTiming();
            }
        }
            break;
        default:
            break;
    }
}

void TalkManager::startTiming()
{
    if (!_started)
    {
        _acc = 0;
        _started = true;
    }
}

void TalkManager::stopTiming()
{
    _started = false;
}

void TalkManager::update(float dt)
{
    if(_started)
    {
        _acc += dt;
        
        switch (_mode) {
            case Mode::Normal:
            {
                if (_acc > _terminalTime) {
                    _armature->switchArmature();
                    stopTiming();
                }
            }
                break;
            case Mode::Intro:
            {
                if (_acc > _terminalTime) {
                    _acc = 0;
                    _introIndex++;
                    
                    if (_introIndex == 9)   //intro 结束了
                    {
                        _mode = Mode::Normal;
                        _armature->setKeepTalking(false);
                        _armature->switchArmature();
                        
                        stopTiming();
                    }
                    else
                    {
                        xAudio->playSound(string("tutorial/sound_intro_0" + std::to_string(_introIndex) + ".mp3").c_str());
                        _terminalTime = getSoundTime("sound_intro_0"  + std::to_string(_introIndex)) + 2;
                    }
                }
            }
                break;
            case Mode::WelcomeBack:
            {
                if (_acc > _terminalTime) {
                    _acc = 0;
                    _introIndex++;
                    
                    if (_introIndex == 2) {
                        
                        _mode = Mode::Normal;
                        _armature->setKeepTalking(false);
                        _armature->switchArmature();
                        
                        stopTiming();
                    }
                    else
                    {
                        xAudio->playSound("welcomeback/sound_welcomeback.mp3");
                        _terminalTime = getSoundTime("sound_welcomeback");
                    }
                }
            }
                break;
            default:
                break;
        }
    }
}

void TalkManager::clear()
{
    CC_SAFE_DELETE(_instance);
    Director::getInstance()->getScheduler()->unscheduleUpdate(this);
}

void TalkManager::initIntro(ArmatureManagerPtr armature)
{
    _armature = armature;
    
    if (_viewIntro == true)
        return;
    else
        _viewIntro = true;
    
    bool viewIntro = UserDefault::getInstance()->getBoolForKey("UserDefaultKey_ViewIntro", false);
    if (!viewIntro)
    {
        UserDefault::getInstance()->setBoolForKey("UserDefaultKey_ViewIntro", true);
        UserDefault::getInstance()->flush();
        _mode = Mode::Intro;
    }
    else
    {
        _mode = Mode::WelcomeBack;
    }
    
    _introIndex = 0;
    _armature->setKeepTalking(true);
    _armature->switchArmature();
}

TalkManager::Time TalkManager::getCurrentTime()
{
    time_t now_time_t = time(NULL);
    tm* now = localtime(&now_time_t);
    
    if (now->tm_hour > 0 && now->tm_hour <= 12)
    {
        return Time::Morning;
    }
    else if (now->tm_hour > 12 && now->tm_hour < 18)
    {
        return Time::Afternoon;
    }
    else
    {
        return Time::Evening;
    }
}

float TalkManager::getSoundTime(const string & name)
{
    for (auto talk : _talkKey) {
        if (talk.key == name) {
            return talk.time;
        }
    }
    
    CCASSERT(false, "错误的名称");
    return 0;
}

bool TalkManager::hasBeenPlay(int index)
{
    for(auto played : _randomPlayed)
    {
        if(index == played)
        {
            return true;
        }
    }

    return false;
}
