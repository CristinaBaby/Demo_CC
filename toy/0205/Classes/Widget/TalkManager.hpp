//
//  TalkManager.hpp
//  ColorBook0818
//
//  Created by huangwen on 15/10/14.
//
//

#ifndef TalkManager_hpp
#define TalkManager_hpp

#include "cocos2d.h"
#include "ArmatureManager.h"
using namespace std;
USING_NS_CC;

#define xTalk       (TalkManager::getTalkManger())
class ArmatureManager;

class TalkManager : public Node {
public:
    struct TalkKey
    {
        string   key;
        float    time;
    };
    
    enum class Mode
    {
        Normal = 0,
        Intro,
        WelcomeBack
    };
    
    enum class Time
    {
        Morning = 0,
        Afternoon,
        Evening
    };
    
    void clear();
    
    static TalkManager* getTalkManger();
    vector<TalkKey> getSoundKey();
    void playTalk();
    void update(float dt);
    
    void initIntro(ArmatureManagerPtr armature);
    TalkManager::Time getCurrentTime();
    ~TalkManager();
    
    void startTiming();
    void stopTiming();
    
    bool hasBeenPlay(int index);
    
private:
    TalkManager();
    float getSoundTime(const string & name);
    
private:
    static TalkManager*     _instance;
    
    vector<TalkKey>         _talkKey;
    float                   _acc = 0;
    bool                    _started = false;
    unsigned int            _soundID = 0;
    
    ArmatureManagerPtr      _armature = nullptr;
    Mode                    _mode = Mode::Normal;
    int                     _introIndex = 0;
    float                   _terminalTime = 0;
    bool                    _viewIntro = false;
    vector<int>             _randomPlayed;
};

#endif /* TalkManager_hpp */
