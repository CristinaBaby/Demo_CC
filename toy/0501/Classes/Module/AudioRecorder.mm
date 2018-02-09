//
//  AudioRecorder.cpp
//  AudioRecorder
//
//  Created by maxiang on 8/28/15.
//
//

#include "AudioRecorder.h"
#include "AudioRecorderImpiOS.h"

static AudioRecorder* singletonInstance = nullptr;

AudioRecorder* AudioRecorder::getInstance()
{
    if (singletonInstance == nullptr) {
        
        singletonInstance = new AudioRecorder();
    }
    
    return singletonInstance;
}

AudioRecorder::AudioRecordPermission AudioRecorder::getAudioRecordPermission()
{
    return (AudioRecorder::AudioRecordPermission)[[AudioRecorderImpiOS sharedInstance] audioRecordPermission];
}

bool AudioRecorder::setupNewRecord(const std::string& toFile)
{
    NSString *fileString = [NSString stringWithCString:toFile.c_str() encoding:[NSString defaultCStringEncoding]];
    return [[AudioRecorderImpiOS sharedInstance] setupNewRecord:fileString];
}

float AudioRecorder::getAveragePower()
{
    return [[AudioRecorderImpiOS sharedInstance] averagePower];
}

bool AudioRecorder::prepareToRecord()
{
    
    return true;
}

bool AudioRecorder::record()
{
    return [[AudioRecorderImpiOS sharedInstance] record];
}

bool AudioRecorder::recordForDuration(float duration)
{
    return [[AudioRecorderImpiOS sharedInstance] recordForDuration:duration];
}

void AudioRecorder::pause()
{
    [[AudioRecorderImpiOS sharedInstance] pause];
}

void AudioRecorder::stop()
{
    [[AudioRecorderImpiOS sharedInstance] stop];
}

bool AudioRecorder::deleteRecording()
{
    return [[AudioRecorderImpiOS sharedInstance] deleteRecording];
}




