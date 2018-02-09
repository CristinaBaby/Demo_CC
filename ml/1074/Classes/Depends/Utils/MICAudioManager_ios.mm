//
//  MICAudioManager_ios.m
//  mlabs1074
//
//  Created by wuguiling on 17/4/17.
//
//

#import "MICAudioManager_ios.h"
#include "MICAudioManager.h"

static MICAudioManager_ios *s_instance=nil;
@implementation MICAudioManager_ios


+(MICAudioManager_ios*)sharedManager
{
    if(s_instance==nil){
        s_instance=[[MICAudioManager_ios alloc] init];
    }
    return s_instance;
}

+(void)purgeManager
{
    if (s_instance) {
        [s_instance release],s_instance=nil;
    }
}

-(void)dealloc
{
    s_instance=0;
    [super dealloc];
}

#pragma mark -- 获取当前时间本地的字符串
- (NSString *)getTimeString{
    //1.获取本地时间
    NSDate * date = [NSDate date];
    NSDateFormatter * format = [[NSDateFormatter alloc]init];
    //年YYYY,月MM,日dd,小时HH,分钟mm,秒ss,毫秒SSS
    format.dateFormat = @"YYYYMMddHHmmssSSS";
    NSString * str = [format stringFromDate:date];
    return str;
}

-(void) requestPermision
{
    _hasPermision = true;
    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 7) {
        if([[AVAudioSession sharedInstance] respondsToSelector:@selector(requestRecordPermission:)])
        {
            [[AVAudioSession sharedInstance] requestRecordPermission:^(BOOL granted) {
                NSLog(@"permission : %d", granted);
                if (!granted) {
                    
                    [[[UIAlertView alloc] initWithTitle:nil
                                                message:
                      [NSString stringWithFormat:@"This app does not have access to your microphone.You can enable access in Privacy Setting."]
                                               delegate:nil
                                      cancelButtonTitle:@"OK"
                                      otherButtonTitles:nil] show];
                    _hasPermision = false;
                }else{
                    _hasPermision = true;
                }
                if (MICAudioManager::getInstance()->onRequestPermisionCallback) {
                    MICAudioManager::getInstance()->onRequestPermisionCallback(_hasPermision);
                }
            }];
        }
    }
}
-(bool) deviceReady
{
    if (!_hasPermision) {
        return false;
    }
    NSError *setCategoryError = nil;
    NSLog(@"======%@",[[AVAudioSession sharedInstance] category]);
    BOOL success = [[AVAudioSession sharedInstance]
                    
                    setCategory:
                    //                        AVAudioSessionCategoryRecord
                    
                    //或者
                    AVAudioSessionCategoryPlayAndRecord
                    
                    error: &setCategoryError];
    if (!success) {
        return false;
    }
    //为空,需要初始化
    if (!_recorder) {
        //1).录制的音频保存的路径(沙盒下的Documents)
//        NSString * path = [NSString stringWithFormat:@"%@/Documents/%@.caf",NSHomeDirectory(),[self getTimeString]];
//        NSLog(@"%@",path);
        NSString* dir=[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
        NSArray *pathComponents = [NSArray arrayWithObjects:dir,@"blow_candle",nil];
        NSURL* recordedAudioURL = [NSURL fileURLWithPathComponents:pathComponents];
        //2).音频的相关设置
        NSDictionary *dic = @{AVEncoderAudioQualityKey:@(AVAudioQualityLow),AVEncoderBitRateKey:@(16),AVNumberOfChannelsKey:@(2),AVSampleRateKey:@(44100.0)};
        //dic 里面包含的信息 主要是对录制的音频做设置
        //AVEncoderAudioQualityKey 音频编码质量
        //AVEncoderBitRateKey 采样位数 默认16位
        //AVNumberOfChannelsKey 音乐通道数
        //AVSampleRateKey 采样率
        
        //3).初始化_recorder对象
        //参数1:音频保存的路径
        //参数2:音频的相关设置
        //参数3:对象创建发生错误时的信息
        NSError *error = nil;
        _recorder = [[AVAudioRecorder alloc]initWithURL:recordedAudioURL settings:dic error:&error];
        
        //            _recorder虽然写代码创建了,但是不代表就一定可以分配到相应的硬件资源(硬件资源本身损坏,硬件资源紧缺)
        
        //4).判断创建的_recorder是否为空,不为空时才能录制
        
        if (error) {
            NSLog(@"创建录音对象时发生错误，错误信息：%@",error.localizedDescription);
            return false;
        }
        if (_recorder) {
            //准备录制
            [_recorder prepareToRecord];
        }
    }
    _isRecord = false;
    return true;
}
-(void) startRecord
{
    if (!_recorder) {
        return;
    }
    NSError *error = nil;
    [[AVAudioSession sharedInstance] setActive:YES error:&error];
    if (error) {
        NSLog(@"setActive时发生错误，错误信息：%@",error.localizedDescription);
        return;
    }
    _recorder.meteringEnabled = true;
    //开始录制
    if (![_recorder isRecording]) {
        _isRecord = [_recorder record];
        if (!_isRecord) {
            NSLog(@"录音时时发生错误，错误信息：%@",error.localizedDescription);
        }
    }
}

-(void) stopRecord
{
    [[AVAudioSession sharedInstance]
     
     setCategory:
     AVAudioSessionCategorySoloAmbient
     error: nil];
    
    if (!_recorder) {
        return;
    }
    //停止录制
    if ([_recorder isRecording]){
        [_recorder stop];
    }
    
    [_recorder release];
    //释放
    _recorder = nil;
    
}

-(float) getPower
{
    if (!_recorder) {
        return 0;
    }
    //更新测量值
    [_recorder updateMeters];
    //取得第一个通道的音频，注意音频强度范围时-160到0
    float power = [_recorder averagePowerForChannel:0];
    return power;
}
@end
