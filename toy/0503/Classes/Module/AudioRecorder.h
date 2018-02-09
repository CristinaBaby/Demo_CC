//
//  AudioRecorder.h
//  AudioRecorder
//
//  Created by maxiang on 8/28/15.
//
//

#ifndef __AudioRecorder__AudioRecorder__
#define __AudioRecorder__AudioRecorder__

#include "cocos2d.h"

class AudioRecorder : public cocos2d::Ref
{
public:
    enum class AudioRecordPermission
    {
        Granted = 0,
        Denied,
        Undetermined
    };
   
    static AudioRecorder* getInstance();
    
    //
    bool setupNewRecord(const std::string& toFile);
    
    AudioRecordPermission getAudioRecordPermission();
    
    float getAveragePower();

    bool prepareToRecord();
    
    bool record();
    
    bool recordForDuration(float duration);

    void pause();
  
    void stop();
    
    bool deleteRecording();
};

#endif /* defined(__AudioRecorder__AudioRecorder__) */
