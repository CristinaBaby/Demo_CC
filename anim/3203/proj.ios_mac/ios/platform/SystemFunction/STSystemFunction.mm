//
//  STSystemFunction.cpp
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-9.
//
//

#include "STSystemFunction.h"
#include "SystemFunctionIOSWrapper.h"
#import "ImageSave.h"
#import "NewsBlast.h"

STSystemFunction::STSystemFunction()
{
}

STSystemFunction::~STSystemFunction()
{
}

void STSystemFunction::sendEmail(const char* subject, const char* body)
{
    [[SystemFunctionManager sharedManager]sendEmail : [NSString stringWithCString:subject encoding:NSUTF8StringEncoding]
                                             Content:[NSString stringWithCString:body encoding:NSUTF8StringEncoding]];
}

void STSystemFunction::popAlertDialog(const char *message)
{
    [[SystemFunctionManager sharedManager]popAlertDialog:[NSString stringWithCString:message encoding:NSUTF8StringEncoding]];
}

void STSystemFunction::showMoreGame()
{
    [[SystemFunctionManager sharedManager]showMorePage];
}

void STSystemFunction::go2MarketDetail(std::string packagename)
{
    [[SystemFunctionManager sharedManager]go2Market:[NSString stringWithCString:packagename.c_str() encoding:NSUTF8StringEncoding]];
}

void STSystemFunction::makeToast(const char* message)
{
}

void STSystemFunction::rating()
{
}

bool STSystemFunction::isRate()
{
    return [[SystemFunctionManager sharedManager] isRateUs];
}

void STSystemFunction::contactUs()
{
}

void STSystemFunction::sendEmailAndAssetPic(std::string subject, std::string message, std::string assetName)
{}

void STSystemFunction::sendEmailAndFilePic(std::string subject, std::string message, std::string fileName)
{}

std::string STSystemFunction::getSdCardPath()
{
    return "";
}

bool STSystemFunction::checkNetworkAvailable()
{
    return true;
}

bool STSystemFunction::isTabletAvailable()
{
    return true;
}

float STSystemFunction::densityScale()
{
    return 0.0f;
}

void STSystemFunction::rateUs(){
    [[SystemFunctionManager sharedManager] rateUs:@"1022298696" Message:@"If you like this app, please rate our game!"];
}

void STSystemFunction::dismissRateUs(){
    
}

void STSystemFunction::saveImage(std::string fileCache){
    [[SystemFunctionManager sharedManager] saveLocalImage : [NSString stringWithUTF8String:fileCache.c_str()]];
}

void STSystemFunction::saveImageWithImage(cocos2d::Image *aImage)
{
    int bitsPerComponent = 8;
    int bitsPerPixel = 32;
    int bytesPerRow    = (bitsPerPixel/8) * aImage->getWidth();
    int myDataLength = bytesPerRow * aImage->getHeight();
    unsigned char *pixels    = aImage->getData();
    // make data provider with data.
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
    CGDataProviderRef provider        = CGDataProviderCreateWithData(NULL, pixels, myDataLength, NULL);
    CGColorSpaceRef colorSpaceRef    = CGColorSpaceCreateDeviceRGB();
    CGImageRef iref                    = CGImageCreate(aImage->getWidth(), aImage->getHeight(),
                                                       bitsPerComponent, bitsPerPixel, bytesPerRow,
                                                       colorSpaceRef, bitmapInfo, provider,
                                                       NULL, false,
                                                       kCGRenderingIntentDefault);
    UIImage* image                    = [[[UIImage alloc] initWithCGImage:iref] autorelease];
    CGImageRelease(iref);
    
    [[SystemFunctionManager sharedManager] saveLocalImageWithImage:image];
}

void STSystemFunction::goToPrivacy()
{
    [[SystemFunctionManager sharedManager]goToPrivacy];
}

void STSystemFunction::showNewsBlast()
{
    [[SystemFunctionManager sharedManager] showNewsBlast];
}


void STSystemFunction::showNewsBlast(NewsBlastMode mode){
    [[SystemFunctionManager sharedManager] showNewsBlast:(InvokeMode)mode];
}

void STSystemFunction::endAnalytics()
{
    
}