//
//  AudioRecorderImpiOS.h
//  MXTableView
//
//  Created by maxiang on 8/28/15.
//
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@interface AudioRecorderImpiOS : NSObject
{
    AVAudioRecorder *_audioRecorder;
    
}

+ (AudioRecorderImpiOS *)sharedInstance;

- (AVAudioSessionRecordPermission)audioRecordPermission;

- (float)averagePower;

- (BOOL)requestPermission;

- (BOOL)setupNewRecord:(NSString *)toFile;

- (BOOL)prepareToRecord;;

- (BOOL)record;

- (BOOL)recordForDuration:(float )duration;

- (void)pause;

- (void)stop;

- (BOOL)deleteRecording;

@end
