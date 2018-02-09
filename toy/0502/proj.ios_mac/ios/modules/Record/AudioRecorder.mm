#import "AudioRecorder.h"
#import <AVFoundation/AVFoundation.h>
#import <CoreAudio/CoreAudioTypes.h>

#include "cocos2d.h"

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

AVAudioSession *_session;
AVAudioRecorder *_recorder;
AVAudioPlayer *_audioPlayer = nil;

static AudioRecorder *_sharedAudioRecorder = nullptr;

AudioRecorder* AudioRecorder::getInstance()
{
    if (! _sharedAudioRecorder)
    {
        _sharedAudioRecorder = new AudioRecorder();
    }
    
    return _sharedAudioRecorder;
}

void AudioRecorder::destroyInstance()
{
    [_recorder release];
    CC_SAFE_DELETE(_sharedAudioRecorder);
}

AudioRecorder::AudioRecorder()
{
    _session = [AVAudioSession sharedInstance];
}

bool AudioRecorder::start()
{
    return start(DEFAULT_NAME);
}

bool AudioRecorder::start(const std::string &filename)
{
    //检查是否正在录音
    if (_recorder && _recorder.recording)
    {
        return true;
    }
    
    if (canRecord() == false) {
        return false;
    }
    
    //[_session setActive:NO error:nil]; ios 8 在任何地方都不能用这句话, 不然会报错, 而且category必须是 PlayAndRecord, 不能是单独的Record
    [_session setCategory:AVAudioSessionCategoryPlayAndRecord error: nil];
    [_session setActive:YES error:nil];
    
    //录音参数, 文件地址
    string fullPath = FileUtils::getInstance()->getWritablePath() + filename + ".caf";
    
    if (_lastFullPath.compare(fullPath) != 0) {

        _lastFullPath = fullPath;
        NSString * nsFullPath = [NSString stringWithUTF8String:_lastFullPath.c_str()];
        NSURL *soundFileURL = [NSURL fileURLWithPath: nsFullPath];
    
        NSDictionary *recordSettings = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithInt:AVAudioQualityMedium],
                                    AVEncoderAudioQualityKey,
                                    [NSNumber numberWithInt:16],
                                    AVEncoderBitRateKey,
                                    [NSNumber numberWithInt: 2],
                                    AVNumberOfChannelsKey,
                                    [NSNumber numberWithFloat:44100.0],
                                    AVSampleRateKey,
                                    nil];
    
        NSError *error = nil;
        _recorder = [[AVAudioRecorder alloc]
                     initWithURL:soundFileURL
                     settings:recordSettings
                     error:&error];
        
    
        if (error)
        {
            NSLog(@"error: %@", [error localizedDescription]);
        } else {
            [_recorder prepareToRecord];
        }
    }

    NSLog(@"start");
    [_recorder record];
    
    return true;
}

void AudioRecorder::stop()
{
    if (_recorder.recording)
    {
        NSLog(@"stop");
        [_recorder stop];
        [_session setCategory:AVAudioSessionCategoryPlayback error:nil];
    }
}

unsigned int AudioRecorder::play()
{
    return play(_lastFullPath);
}

unsigned int AudioRecorder::play(const string &filename)
{
    if (!_recorder.recording)
    {
        NSLog(@"play");
        
        if (_audioPlayer) {
            if (_audioPlayer.playing) {
                [_audioPlayer stop];
            }
            [_audioPlayer release];
        }
        
        string fullPath = FileUtils::getInstance()->getWritablePath() + filename + ".caf";
        NSString * nsFullPath = [NSString stringWithUTF8String:fullPath.c_str()];
        NSURL *soundFileURL = [NSURL fileURLWithPath: nsFullPath];
        NSError *error;
        _audioPlayer = [[AVAudioPlayer alloc]
                       initWithContentsOfURL:soundFileURL
                       error:&error];
        
        //_audioPlayer.delegate = self;
        
        if (error)
            NSLog(@"Error: %@", [error localizedDescription]);
        else
            [_audioPlayer play];
    }
    
    return 0;

//    SimpleAudioEngine::getInstance()->setEffectsVolume(1);
//    string fullPath = FileUtils::getInstance()->getWritablePath() + filename + ".caf";
//    log("%s", fullPath.c_str());
//    return SimpleAudioEngine::getInstance()->playEffect(fullPath.c_str());
}

void AudioRecorder::stopPlay()
{
    if (_audioPlayer) {
        if (_audioPlayer.playing) {
            [_audioPlayer stop];
        }
        [_audioPlayer release];
        _audioPlayer = nil;
    }
}

bool AudioRecorder::isFileExist(const std::string &filename)
{
    string fullPath = FileUtils::getInstance()->getWritablePath() + filename + ".caf";
    return FileUtils::getInstance()->isFileExist(fullPath);
}

bool AudioRecorder::canRecord()
{
    __block BOOL bCanRecord = YES;
    //if ([[[UIDevice currentDevice] systemVersion] compare:@"7.0"] != NSOrderedAscending)
    {
        AVAudioSession *audioSession = [AVAudioSession sharedInstance];
        if ([audioSession respondsToSelector:@selector(requestRecordPermission:)]) {
            [audioSession performSelector:@selector(requestRecordPermission:) withObject:^(BOOL granted) {
                if (granted) {
                    bCanRecord = YES;
                }
                else {
                    bCanRecord = NO;
                }
            }];
        }
    }
    
    if (bCanRecord == YES) {
        return true;
    }
    else
    {
        return false;
    }
}

//only for android
bool AudioRecorder::isFinishStop()
{
    return true;
}