//
//  CameraImageHelper.m
//  HelloWorld
//
//  Created by Erica Sadun on 7/21/10.
//  Copyright 2010 Up To No Good, Inc. All rights reserved.
//

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>
#import "CameraImageHelper.h"
#import <ImageIO/ImageIO.h>
#include "CameraHelper.h"

@implementation CameraImageHelper
@synthesize session,captureOutput,image,g_orientation;
@synthesize preview;
@synthesize input = _input;
static CameraImageHelper *sharedInstance = nil;


- (void) initialize
{
    //1.创建会话层
    self.session = [[[AVCaptureSession alloc] init] autorelease];
    [self.session setSessionPreset:AVCaptureSessionPresetPhoto];
    
    
    //2.创建、配置输入设备
    AVCaptureDevice *device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
	NSError *error;
	AVCaptureDeviceInput *captureInput = [AVCaptureDeviceInput deviceInputWithDevice:device error:&error];
	if (!captureInput)
	{
		NSLog(@"Error: %@", error);
		return;
	}
    _input = captureInput;
    [self.session addInput:captureInput];
    
    
    //3.创建、配置输出    
    captureOutput = [[AVCaptureStillImageOutput alloc] init];//qz XXX 可能解决图片不能更新的问题
    NSDictionary *outputSettings = [[NSDictionary alloc] initWithObjectsAndKeys:AVVideoCodecJPEG,AVVideoCodecKey,nil];
    [captureOutput setOutputSettings:outputSettings];

    [outputSettings release];
	[self.session addOutput:captureOutput];
    
    NSNotificationCenter *notificationCenter = [NSNotificationCenter defaultCenter];
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    [notificationCenter addObserver:self selector:@selector(deviceOrientationChanged) name:UIDeviceOrientationDidChangeNotification object:nil];

}

- (id) init
{
	if (self = [super init])
        [self initialize];
	return self;
}

- (void)swapFrontAndBackCameras {
    // Assume the session is already running
    NSArray *inputs = self.session.inputs;
    for ( AVCaptureDeviceInput *input in inputs ) {
        AVCaptureDevice *device = input.device;
        if ( [device hasMediaType:AVMediaTypeVideo] ) {
            AVCaptureDevicePosition position = device.position;
            AVCaptureDevice *newCamera = nil;
            AVCaptureDeviceInput *newInput = nil;
            if (position == AVCaptureDevicePositionFront)
                newCamera = [self cameraWithPosition:AVCaptureDevicePositionBack];
            else
                newCamera = [self cameraWithPosition:AVCaptureDevicePositionFront];
            newInput = [AVCaptureDeviceInput deviceInputWithDevice:newCamera error:nil];
            // beginConfiguration ensures that pending changes are not applied immediately
            [self.session beginConfiguration];
            [self.session removeInput:input];
            [self.session addInput:newInput];
            self.input = newInput;
            // Changes take effect once the outermost commitConfiguration is invoked.
            [self.session commitConfiguration];
            break;
        }
    } 
}

- (AVCaptureDevice *)cameraWithPosition:(AVCaptureDevicePosition)position
{
    NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    for ( AVCaptureDevice *device in devices )
        if ( device.position == position )
            return device;
    return nil;
}

- (void)deviceOrientationChanged
{
    
//    if (preview) {
//        UIDeviceOrientation deviceOrientation = [[UIDevice currentDevice] orientation];
//        switch (deviceOrientation) {
//            case UIDeviceOrientationLandscapeLeft:
//            case UIDeviceOrientationFaceDown:
//                [preview setOrientation:AVCaptureVideoOrientationLandscapeRight];
//                break;
//                
//            case UIDeviceOrientationLandscapeRight:
//            case UIDeviceOrientationFaceUp:
//                NSLog(@"preview = %@",preview);
//                [preview setOrientation:AVCaptureVideoOrientationLandscapeLeft];
//                break;
//                
//            default:
//                break;
//        }
//    }
    
}

-(void) embedPreviewInView: (UIView *) aView {
    if (!session) return;
    
    self.preview = [AVCaptureVideoPreviewLayer layerWithSession: session];
    preview.frame = aView.bounds;
    preview.videoGravity = AVLayerVideoGravityResizeAspectFill; 
    [aView.layer addSublayer: preview];
    
//    [preview retain];//?
}

- (void)changePreviewOrientation:(UIInterfaceOrientation)interfaceOrientation
{
     [CATransaction begin];
//    if (interfaceOrientation == UIInterfaceOrientationLandscapeRight) {
//        g_orientation = UIImageOrientationUp;
//        preview.orientation = AVCaptureVideoOrientationLandscapeRight;
//        
//    }else if (interfaceOrientation == UIInterfaceOrientationLandscapeLeft){
//        g_orientation = UIImageOrientationDown;
//        preview.orientation = AVCaptureVideoOrientationLandscapeLeft;
//    }
//    else if (interfaceOrientation == UIInterfaceOrientationPortrait){
//        g_orientation = UIImageOrientationUp;
//        preview.orientation = AVCaptureVideoOrientationPortrait;
//    }
    g_orientation = UIImageOrientationUp;
    preview.orientation = AVCaptureVideoOrientationPortrait;
    [CATransaction commit];
}

-(void)Captureimage
{
    //get connection
    AVCaptureConnection *videoConnection = nil;
    for (AVCaptureConnection *connection in captureOutput.connections) {
        for (AVCaptureInputPort *port in [connection inputPorts]) {
            if ([[port mediaType] isEqual:AVMediaTypeVideo] ) {
                videoConnection = connection;
                break;
            }
        }
        if (videoConnection) { break; }
    }
    
    //get UIImage
    [captureOutput captureStillImageAsynchronouslyFromConnection:videoConnection completionHandler:
     ^(CMSampleBufferRef imageSampleBuffer, NSError *error)
    {
        if (imageSampleBuffer == nil) {
             return ;
         }
         
         // Continue as appropriate.
        NSData *imageData = [AVCaptureStillImageOutput jpegStillImageNSDataRepresentation:imageSampleBuffer];
        UIImage *t_image = [[UIImage alloc] initWithData:imageData];
        NSLog(@"source img:%@",NSStringFromCGSize(t_image.size));
        self.image = [self scaleAndRotateImage:t_image];
        [t_image release];
        CameraHelper::getInstance()->captureEnd();
        
    }];
}

static inline double radians (double degrees) {return degrees * M_PI/180;}

- (UIImage *)scaleAndRotateImage:(UIImage *)AVimage{
    
    int kMaxResolution = [UIScreen mainScreen].bounds.size.height * [UIScreen mainScreen].scale; // Or whatever
    
    CGImageRef imgRef = AVimage.CGImage;
    
    CGFloat width = CGImageGetWidth(imgRef);
    CGFloat height = CGImageGetHeight(imgRef);
    CCLOG("%d %.2f %.2f", kMaxResolution, width, height);
    CGAffineTransform transform = CGAffineTransformIdentity;
    CGRect bounds = CGRectMake(0, 0, width, height);
//    if (width > kMaxResolution || height > kMaxResolution)
    {
        CGFloat ratio = width/height;
        if (ratio > 1) {
            bounds.size.width = kMaxResolution;
            bounds.size.height = bounds.size.width / ratio;
        }
        else {
            bounds.size.height = kMaxResolution;
            bounds.size.width = bounds.size.height * ratio;
        }
    }
    
    CGFloat scaleRatio = bounds.size.width / width;
    CGSize imageSize = CGSizeMake(CGImageGetWidth(imgRef), CGImageGetHeight(imgRef));
    CGFloat boundHeight;
    UIImageOrientation orient = AVimage.imageOrientation;
    switch(orient) {
            
        case UIImageOrientationUp: //EXIF = 1
            transform = CGAffineTransformIdentity;
            break;
            
        case UIImageOrientationUpMirrored: //EXIF = 2
            transform = CGAffineTransformMakeTranslation(imageSize.width, 0.0);
            transform = CGAffineTransformScale(transform, -1.0, 1.0);
            break;
            
        case UIImageOrientationDown: //EXIF = 3
            transform = CGAffineTransformMakeTranslation(imageSize.width, imageSize.height);
            transform = CGAffineTransformRotate(transform, M_PI);
            break;
            
        case UIImageOrientationDownMirrored: //EXIF = 4
            transform = CGAffineTransformMakeTranslation(0.0, imageSize.height);
            transform = CGAffineTransformScale(transform, 1.0, -1.0);
            break;
            
        case UIImageOrientationLeftMirrored: //EXIF = 5
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(imageSize.height, imageSize.width);
            transform = CGAffineTransformScale(transform, -1.0, 1.0);
            transform = CGAffineTransformRotate(transform, 3.0 * M_PI / 2.0);
            break;
            
        case UIImageOrientationLeft: //EXIF = 6
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(0.0, imageSize.width);
            transform = CGAffineTransformRotate(transform, 3.0 * M_PI / 2.0);
            break;
            
        case UIImageOrientationRightMirrored: //EXIF = 7
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeScale(-1.0, 1.0);
            transform = CGAffineTransformRotate(transform, M_PI / 2.0);
            break;
            
        case UIImageOrientationRight: //EXIF = 8
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(imageSize.height, 0.0);
            transform = CGAffineTransformRotate(transform, M_PI / 2.0);
            break;
            
        default:
            [NSException raise:NSInternalInconsistencyException format:@"Invalid image orientation"];
            
    }
    
    if(_input && _input.device.position == AVCaptureDevicePositionFront)
    {
        transform = CGAffineTransformTranslate(transform, 0, imageSize.height);
        transform = CGAffineTransformScale(transform, 1.0, -1.0);
    }
    
    UIGraphicsBeginImageContext(bounds.size);
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    if (orient == UIImageOrientationRight || orient == UIImageOrientationLeft) {
        CGContextScaleCTM(context, -scaleRatio, scaleRatio);
        CGContextTranslateCTM(context, -height, 0);
    }
    else {
        CGContextScaleCTM(context, scaleRatio, -scaleRatio);
        CGContextTranslateCTM(context, 0, -height);
    }
    
    CGContextConcatCTM(context, transform);
    
    CGContextDrawImage(UIGraphicsGetCurrentContext(), CGRectMake(0, 0, width, height), imgRef);
    UIImage *imageCopy = UIGraphicsGetImageFromCurrentImageContext();
    
    UIGraphicsEndImageContext();
//    imageCopy = [UIImage imageWithCGImage:imageCopy.CGImage scale:imageCopy.scale orientation:UIImageOrientationRightMirrored | UIImageOrientationUpMirrored];
    
    return imageCopy;
}

- (void)saveImage:(UIImage *)imageSave WithName:(NSString *)imageName
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
    NSString *productName = [[NSString alloc] initWithFormat:@"%@--.png",imageName];
    NSString *uniquePath=[[paths objectAtIndex:0] stringByAppendingPathComponent:productName];
    //将图片写到Documents文件中
    [UIImagePNGRepresentation(imageSave)writeToFile: uniquePath    atomically:YES];
    NSLog(@"%@",uniquePath);
}
UIImage *scaleAndRotateImage(UIImage *image)
{
    return image;
    
    float imgWidth=CGImageGetWidth(image.CGImage);
    float imgHeight=CGImageGetHeight(image.CGImage);
    CGSize realSize=CGSizeMake(imgWidth, imgHeight);
    UIImageOrientation orient = image.imageOrientation;
    CGAffineTransform transform = CGAffineTransformIdentity;
    switch(orient) {
        case UIImageOrientationRightMirrored:
            transform = CGAffineTransformMakeScale(-1.0, 1.0);
            transform = CGAffineTransformRotate(transform, M_PI / 2.0);
            break;
            
        default:
            break;
            
    }
    
    UIGraphicsBeginImageContext(realSize);
    CGContextRef context=UIGraphicsGetCurrentContext();
    CGContextTranslateCTM(context, 0, realSize.height);
    CGContextScaleCTM(context, 1, -1);
    CGContextConcatCTM(context, transform);
    CGContextDrawImage(context, CGRectMake(0, 0, realSize.width, realSize.height), image.CGImage);
    UIImage *scaleImg=UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return scaleImg;
}

- (void) dealloc
{
    [captureOutput release];
    captureOutput = nil;
	self.session = nil;
	self.image = nil;
	[super dealloc];
}

#pragma mark Class Interface

+ (id) sharedInstance // private
{
	if(!sharedInstance)
        sharedInstance = [[self alloc] init];
    return sharedInstance;
}

+ (void) startRunning
{
    [[self sharedInstance] initialize];
    
	[[[self sharedInstance] session] startRunning];	
}

+ (void) stopRunning
{
	[[[self sharedInstance] session] stopRunning];
    CameraImageHelper *lHelper = [self sharedInstance];
    [lHelper.session removeInput:lHelper.input];
    lHelper.image = nil;
    lHelper.session = nil;
    lHelper.captureOutput = nil;
    lHelper.preview = nil;
}

+ (UIImage *) image
{
	return (UIImage *)[[self sharedInstance] image];
}

+(void)CaptureStillImage
{
    [[self sharedInstance] Captureimage];
}

+ (void)embedPreviewInView: (UIView *) aView
{
    [[self sharedInstance] embedPreviewInView:aView];
}

+ (void)changePreviewOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    [[self sharedInstance] changePreviewOrientation:(UIInterfaceOrientation)interfaceOrientation];
}

@end

#endif
