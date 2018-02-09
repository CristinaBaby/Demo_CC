//
//  ImageTool.m
//  KidsFrameWorkTest
//
//  Created by zhangguangzong1 on 5/5/14.
//
//

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import "ImageTool.h"
#include "CameraHelper.h"

@implementation ImageTool

static ImageTool *instance=nil;

+(ImageTool *)sharedImageTool
{
    @synchronized(self)
    {
        if (instance==nil) {
            instance=[[ImageTool alloc] init];
        }
    }
    return instance;
}

-(UIImage *)CCImageToUIImage:(CCImage*)apImg
{
    int bitsPerComponent = 8;
    int bitsPerPixel = 32;
    int bytesPerRow    = (bitsPerPixel/8) * apImg->getWidth();
    int myDataLength = bytesPerRow * apImg->getHeight();
    unsigned char *pixels    = apImg->getData();
    // make data provider with data.
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
    CGDataProviderRef provider        = CGDataProviderCreateWithData(NULL, pixels, myDataLength, NULL);
    
    CGColorSpaceRef colorSpaceRef    = CGColorSpaceCreateDeviceRGB();
    CGImageRef iref                  = CGImageCreate(apImg->getWidth(), apImg->getHeight(),
                                                       bitsPerComponent, bitsPerPixel, bytesPerRow,
                                                       colorSpaceRef, bitmapInfo, provider,
                                                       NULL, false,
                                                       kCGRenderingIntentDefault);
    UIImage* image                   = [[[UIImage alloc] initWithCGImage:iref] autorelease];
    
    CGImageRelease(iref);
    CGColorSpaceRelease(colorSpaceRef);
    CGDataProviderRelease(provider);
    
    return image;
}

- (void)image:(UIImage *)image didFinishSavingWithError:(NSError *)error contextInfo:(void *)contextInfo
{
    bool bRet = true;
    if (error != nil)
    {
        bRet = false;
    }
    CameraHelper::getInstance()->saveCall(bRet);
}
@end

#endif
