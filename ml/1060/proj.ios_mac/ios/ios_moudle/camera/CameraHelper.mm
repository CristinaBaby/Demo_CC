//
//  CameraHelper.mm
//  KidsFrameWorkTest
//
//  Created by zhangguangzong1 on 5/4/14.
//
//

#include "CameraHelper.h"
#include "CameraImageHelper.h"
#include "AppController.h"
#import "RootViewController.h"
#include "ImageTool.h"

#define kCameraViewTag 10001

static CameraHelper *s_pCamera = NULL;

CameraHelper *CameraHelper::getInstance()
{
    if (!s_pCamera) {
        s_pCamera = new CameraHelper();
    }
    return s_pCamera;
}

void CameraHelper::open()
{
    UIView* cameraView = [[UIView alloc]initWithFrame:((RootViewController *)([AppController sharedAppController].viewController)).view.bounds];
    cameraView.tag  = kCameraViewTag;
    
    [CameraImageHelper startRunning];
    [CameraImageHelper embedPreviewInView:cameraView];
    
    [[AppController sharedAppController].window addSubview:cameraView];
    [[AppController sharedAppController].window sendSubviewToBack:cameraView];
    [cameraView release];
}

void CameraHelper::close()
{
    
    [CameraImageHelper stopRunning];
    
    UIView *view = [[AppController sharedAppController].window viewWithTag:kCameraViewTag];
    if([view.layer sublayers])
    {
        NSLog(@"layer count:%d", [[view.layer sublayers] count]);
//        for (int i = [[view.layer sublayers] count] - 1; i >=0; --i) {
//            [[[view.layer sublayers] objectAtIndex:i] removeFromSuperlayer];
//        }
    }
    
    [[[AppController sharedAppController].window viewWithTag:kCameraViewTag].layer removeAllAnimations];
//    [[[AppController sharedAppController].window viewWithTag:kCameraViewTag].layer removeFromSuperlayer];
    
    [[[AppController sharedAppController].window viewWithTag:kCameraViewTag] removeFromSuperview];
}

bool CameraHelper::swapFrontAndBackCameras()
{
    [[CameraImageHelper sharedInstance] swapFrontAndBackCameras];
    return true;
}

void CameraHelper::capture()
{
    [CameraImageHelper CaptureStillImage];
}

void CameraHelper::captureEnd()
{
    if (m_Callback)
    {
        m_Callback->completeCapture(this->getCaptureTex());
    }
}

void CameraHelper::saveCall(bool isSuccess)
{
    if (m_Callback)
    {
        m_Callback->saveToAlbum(isSuccess);
    }
}

CCTexture2D* CameraHelper::getCaptureTex()
{
    UIImage* image = [CameraImageHelper image];
    
    CCTexture2D* tex = new CCTexture2D();
    tex->autorelease();
    
    CGImageRef imageRef = [image CGImage];
    NSUInteger width = CGImageGetWidth(imageRef);
    NSUInteger height = CGImageGetHeight(imageRef);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    unsigned char* rawData = (unsigned char*) calloc(height * width * 4, sizeof(unsigned char));
    NSUInteger bytesPerPixel = 4;
    NSUInteger bytesPerRow = bytesPerPixel * width;
    NSUInteger bitsPerComponent = 8;
    CGContextRef context = CGBitmapContextCreate(rawData, width, height, bitsPerComponent, bytesPerRow, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(colorSpace);
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
    CGContextRelease(context);
//    tex->initWithData(rawData, kCCTexture2DPixelFormat_RGBA8888, width, height, CCSizeMake(width, height));
    tex->initWithData(rawData, sizeof(rawData), Texture2D::PixelFormat::RGBA8888, width, height, cocos2d::Size(width, height));
    
    free(rawData);
    
    CameraImageHelper *lHelper = [CameraImageHelper sharedInstance];
    lHelper.image = nil;
    
    return tex;
}

void CameraHelper::saveToAlbum(CCImage* img)
{
    UIImage* saveImg = [[ImageTool sharedImageTool] CCImageToUIImage:img];
    UIImageWriteToSavedPhotosAlbum(saveImg, [ImageTool sharedImageTool], @selector(image:didFinishSavingWithError:contextInfo:), nil);
}

bool CameraHelper::isCameraEnable()
{
    __block BOOL ret = true;
    if (![[AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo]
          supportsAVCaptureSessionPreset:AVCaptureSessionPresetPhoto])
    {
        ret = false;
    }
    
    if ( [[UIDevice currentDevice].systemVersion floatValue] >= 7.0)
    {
        NSString *mediaType = AVMediaTypeVideo;
        AVAuthorizationStatus authStatus = [AVCaptureDevice authorizationStatusForMediaType:mediaType];
      
        if(authStatus ==AVAuthorizationStatusRestricted){
      
        }else if(authStatus == AVAuthorizationStatusDenied){
            ret = false;
        }
        else if(authStatus == AVAuthorizationStatusAuthorized){
            ret = true;
        }else if(authStatus == AVAuthorizationStatusNotDetermined){
            
            [AVCaptureDevice requestAccessForMediaType:mediaType completionHandler:^(BOOL granted) {
                if(granted){
                    ret = true;
                }
                else {
                    ret = false;
                }
                
            }];
        }
    }
    return ret;
}

