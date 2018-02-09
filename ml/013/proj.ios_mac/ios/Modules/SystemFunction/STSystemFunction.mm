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
#import <sys/xattr.h>
#import "Email.h"
#import <UIKit/UIKit.h>


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
    //[[SystemFunctionManager sharedManager]go2Market:[NSString stringWithCString:packagename.c_str() encoding:NSUTF8StringEncoding]];
}

void STSystemFunction::makeToast(const char* message)
{
}

void STSystemFunction::rating()
{
}

void STSystemFunction::contactUs()
{
}

void STSystemFunction::sendEmailAndAssetPic(std::string subject, std::string message, std::string assetName)
{}

void  STSystemFunction::sendEmailAndFilePic(std::string subject, std::string message, std::string fileName)
{
    Email* email = [[Email alloc] init];
    [email sendEmailWithTitle:subject.c_str() withContent:message.c_str() withImage:fileName.c_str() withDelegate:email];
    
}


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
    
}

void STSystemFunction::dismissRateUs(){
    
}

void STSystemFunction::saveImage(std::string fileCache,Ref* selector,CallFunction callback){
    ImageSave *lImageSave = [[ImageSave alloc] init];
    [lImageSave saveImage:(char *)fileCache.c_str() withTarget:selector withCallFun:callback];
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
    [[SystemFunctionManager sharedManager] showNewsBlast:mode];
}

void STSystemFunction::endSession()
{

}

bool STSystemFunction::addSkipBackupAttributeToItemAtURL(const char* URL)
{
    NSURL *URL1 = [NSURL fileURLWithPath: [NSString stringWithUTF8String:URL]];

    assert([[NSFileManager defaultManager] fileExistsAtPath: [URL1 path]]);
    
    NSError *error = nil;
    BOOL success = [URL1 setResourceValue: [NSNumber numberWithBool: YES]
                                  forKey: NSURLIsExcludedFromBackupKey error: &error];
    if(!success){
        NSLog(@"Error excluding %@ from backup %@", [URL1 lastPathComponent], error);
    }
    return success;
}

bool STSystemFunction::addSkipBackupAttributeToItemAtURL501(const char *URL)
{
    NSURL *URL1 = [NSURL fileURLWithPath: [NSString stringWithUTF8String:URL]];

    assert([[NSFileManager defaultManager] fileExistsAtPath: [URL1 path]]);
    
    const char* filePath = [[URL1 path] fileSystemRepresentation];
    
    const char* attrName = "com.apple.MobileBackup";
    u_int8_t attrValue = 1;
    
    int result = setxattr(filePath, attrName, &attrValue, sizeof(attrValue), 0, 0);
    return result == 0;
}

void STSystemFunction::URLmark(const char* str)
{
    
    NSString *osVersion=[[UIDevice currentDevice] systemVersion];
    NSLog(@"%@", osVersion);
    //    NSString *fristVersion=[[osVersion componentsSeparatedByString:@"."] objectAtIndex:0];
    
    NSString *str1 = @"5.0.1";
    //    int num=0;
    //    num=[fristVersion intValue];
    
    if ([str1 isEqualToString:osVersion])
    {
        addSkipBackupAttributeToItemAtURL(str);
    }
    else
    {
        addSkipBackupAttributeToItemAtURL(str);
    }
}


void STSystemFunction::openURL(std::string url)
{
    [[SystemFunctionManager sharedManager] openURL:[NSString stringWithUTF8String:url.c_str()]];
}

