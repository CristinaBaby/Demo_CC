#ifndef __AudioRecorder_H__
#define __AudioRecorder_H__

#include "cocos2d.h"

#define xRecorder       (AudioRecorder::getInstance())
#define DEFAULT_NAME    ("strecord")                        //不要加后缀

using std::string;

class AudioRecorder
{
public:
    static AudioRecorder* getInstance();
    void destroyInstance();
    
    // 录音, 录音时不会停止背景音乐, 需要手动 stop/pause background music
    bool start();
    bool start(const string &filename);                     //filename 不要加后缀, ios下为.caf, android下为.mp3
    
    // 停止录音
    void stop();
    
    // 播放录音
    unsigned int play();
    unsigned int play(const string &filename);
    
    bool isFileExist(const std::string &filename);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    bool canRecord();
#endif
    
    bool isFinishStop();
    void stopPlay();
    
private:
    AudioRecorder();

private:
    string _lastFullPath;
};

#endif // __HELLOWORLD_SCENE_H__
