//
//  AudioRecorderImpiOS.m
//  MXTableView
//
//  Created by maxiang on 8/28/15.
//
//

#import "AudioRecorderImpiOS.h"

@interface AudioRecorderImpiOS()
{
    NSString *_soundFilePath;
    BOOL _hasPermission;
}

@end

@implementation AudioRecorderImpiOS

static AudioRecorderImpiOS* singletonAudioRecorderImpiOS = nil;

- (void)dealloc
{
    [_audioRecorder release], _audioRecorder = nil;
    [super dealloc];
}

+ (AudioRecorderImpiOS *)sharedInstance
{
    static long onceToken;
    dispatch_once(&onceToken, ^{
        
        singletonAudioRecorderImpiOS = [[AudioRecorderImpiOS alloc] init];
    });
    
    return singletonAudioRecorderImpiOS;
}

- (AVAudioSessionRecordPermission)audioRecordPermission
{
    /* check permission of micphone */
    return [[AVAudioSession sharedInstance] recordPermission];
}

- (BOOL)requestPermission
{
    if ([[AVAudioSession sharedInstance] respondsToSelector:@selector(requestRecordPermission:)])
    {
        __block dispatch_semaphore_t sem = dispatch_semaphore_create(0);
        [[AVAudioSession sharedInstance] requestRecordPermission:^(BOOL available) {
            
            if (available)
            {
                _hasPermission = YES;
                dispatch_semaphore_signal(sem);
            }
            else
            {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [[[UIAlertView alloc] initWithTitle:@"Microphone Access Required" message:@"Please allow the our app access your microphone at \"Settings - Priacy - Microphone\"" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] show];
                });
                
                _hasPermission = NO;
                dispatch_semaphore_signal(sem);
            }
        }];
        dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);

        return _hasPermission;
    }
    else
    {
        return YES;
    }
}

- (BOOL)setupNewRecord:(NSString *)toFile
{
    if (_audioRecorder) {
        [_audioRecorder stop];
        [_audioRecorder release];
        _audioRecorder = nil;
    }

    if (![self requestPermission])
        return NO;
    
    /* set audio session to record */
    BOOL rcode = [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayAndRecord error:nil];
    if (!rcode)
    {
        //防止录音过后声音变小
        [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayback error:nil];
        return NO;
    }

    /* init audio recorder */
    _soundFilePath = toFile;
    NSLog(@"%@",_soundFilePath);
    NSURL *url = [NSURL URLWithString:_soundFilePath];
    
    NSMutableDictionary *settings = [NSMutableDictionary dictionaryWithCapacity:5];
    [settings setValue:[NSNumber numberWithInt:kAudioFormatLinearPCM] forKey:AVFormatIDKey];
    [settings setValue:[NSNumber numberWithFloat:10000.0] forKey:AVSampleRateKey];
    [settings setObject:[NSNumber numberWithInt:AVAudioQualityMax] forKey: AVEncoderAudioQualityKey];
    NSError *error = nil;
    _audioRecorder = [[AVAudioRecorder alloc] initWithURL:url settings:settings error:&error];
    _audioRecorder.meteringEnabled = YES;
  
    if (error)
    {
        NSLog(@"%@",error.description);
        //防止录音过后声音变小
        [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayback error:nil];
        return NO;
    }
    
    /* active the audio session */
    [[AVAudioSession sharedInstance] setActive:YES error:nil];
    
    return YES;
}

- (float)averagePower
{
    [_audioRecorder updateMeters];
    float originalPower = [_audioRecorder averagePowerForChannel:0];
    
    return originalPower;
}

- (BOOL)prepareToRecord
{
    return [_audioRecorder prepareToRecord];
}

- (BOOL)record
{
    return [_audioRecorder record];
}

- (BOOL)recordForDuration:(float )duration
{
    return [_audioRecorder recordForDuration:duration];
}

- (void)pause
{
    [_audioRecorder pause];
}

- (void)stop
{
    [_audioRecorder stop];
    
    [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayback error:nil];
    
    NSDictionary *dic = [[NSFileManager defaultManager] attributesOfItemAtPath:_soundFilePath error:nil];
    NSLog(@"%@",dic);
}

- (BOOL)deleteRecording
{
    return [_audioRecorder deleteRecording];
}


@end
