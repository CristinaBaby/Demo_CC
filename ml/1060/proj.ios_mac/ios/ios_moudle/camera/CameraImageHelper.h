//
//  CameraImageHelper.h
//  HelloWorld
//
//  Created by Erica Sadun on 7/21/10.
//  Copyright 2010 Up To No Good, Inc. All rights reserved.
//

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

@interface CameraImageHelper : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate>
{
	AVCaptureSession *session;
    AVCaptureStillImageOutput *captureOutput;
	UIImage *image;
    AVCaptureVideoPreviewLayer *preview;
    UIImageOrientation g_orientation;
    AVCaptureDeviceInput *_input;
}
@property (retain) AVCaptureSession *session;
@property (retain) AVCaptureOutput *captureOutput;
@property (retain) UIImage *image;
@property (assign) UIImageOrientation g_orientation;
@property (retain) AVCaptureVideoPreviewLayer *preview;
@property (assign) AVCaptureDeviceInput *input;

+ (id) sharedInstance;
+ (void) startRunning;
+ (void) stopRunning;
+ (UIImage *) image;

+ (void)embedPreviewInView: (UIView *) aView;
+ (void)CaptureStillImage;
+ (void)changePreviewOrientation:(UIInterfaceOrientation)interfaceOrientation;

-(void)captureEnd;

- (void)swapFrontAndBackCameras;
@end

#endif
