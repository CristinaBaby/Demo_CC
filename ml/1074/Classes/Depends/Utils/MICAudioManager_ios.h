//
//  MICAudioManager_ios.h
//  mlabs1074
//
//  Created by wuguiling on 17/4/17.
//
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@interface MICAudioManager_ios : NSObject<AVAudioRecorderDelegate>
{
    AVAudioRecorder *_recorder;
    NSTimer *_levelTimer;
    BOOL _isRecord;
    BOOL _hasPermision;
}
+(MICAudioManager_ios*)sharedManager;

-(void) requestPermision;
-(bool) deviceReady;
-(void) startRecord;
-(void) stopRecord;
-(float) getPower;

/* audioRecorderDidFinishRecording:successfully: is called when a recording has been finished or stopped. This method is NOT called if the recorder is stopped due to an interruption. */
//- (void)audioRecorderDidFinishRecording:(AVAudioRecorder *)recorder successfully:(BOOL)flag;
//
///* if an error occurs while encoding it will be reported to the delegate. */
//- (void)audioRecorderEncodeErrorDidOccur:(AVAudioRecorder *)recorder error:(NSError * __nullable)error;
@end
