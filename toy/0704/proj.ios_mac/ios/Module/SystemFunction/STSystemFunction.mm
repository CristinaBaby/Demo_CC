//
//  STSystemFunction.cpp
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-9.
//
//

#include "STSystemFunction.h"  
#include "SystemFunctionIOSWrapper.h"
#include "MPReachability.h"

STSystemFunction::STSystemFunction()
{
}

STSystemFunction::~STSystemFunction()
{
}

void STSystemFunction::sendEmail(const char* subject, const char* body , cocos2d::CCImage* image)
{
    [[SystemFunctionManager sharedManager]sendEmail : [NSString stringWithCString:subject encoding:NSUTF8StringEncoding]
                                             Content:[NSString stringWithCString:body encoding:NSUTF8StringEncoding] : image];
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
    [[SystemFunctionManager sharedManager]popAlertDialog:[NSString stringWithCString:message encoding:NSUTF8StringEncoding]];
}

void STSystemFunction::rating(const char* appleID, const char* message)
{
    [[SystemFunctionManager sharedManager]rateUs:[NSString stringWithCString:appleID encoding:NSUTF8StringEncoding]
                                         Message:[NSString stringWithCString:message encoding:NSUTF8StringEncoding]];
}

bool  STSystemFunction::isRate()
{
    return [[SystemFunctionManager sharedManager]isRate];
}

void STSystemFunction::rating()
{
    // only for Android
}

void STSystemFunction::contactUs()
{
}

void STSystemFunction::sendEmailAndAssetPic(std::string subject, std::string message, std::string assetName)
{}

void STSystemFunction::sendEmailAndFilePic(std::string subject, std::string message, std::string fileName)
{}

std::string STSystemFunction::getPlatformTempPath()
{
     return [[SystemFunctionManager sharedManager]getTempPath];
}

bool STSystemFunction::checkNetworkAvailable()
{
    bool rtn = true;
    MPReachability *r = [MPReachability reachabilityWithHostName:@"www.apple.com"];
    if ([r currentReachabilityStatus] == MPNotReachable) {
        rtn =  false;
    }
    return rtn;
}

bool STSystemFunction::isTabletAvailable()
{
    return true;
}

float STSystemFunction::densityScale()
{
    return 0.0f;
}

void STSystemFunction::go2PrivacyPage()
{
    [[SystemFunctionManager sharedManager] go2PrivacyPage];
}

void STSystemFunction::openURL(std::string url)
{
    [[SystemFunctionManager sharedManager] openURL:[NSString stringWithUTF8String:url.c_str()]];
}

void STSystemFunction::saveLocalImage(cocos2d::CCImage* m_image)
{
    [[SystemFunctionManager sharedManager] saveLocalImage : m_image];
}

std::string STSystemFunction::getPlatformSavePath()
{
  return  [[SystemFunctionManager sharedManager] getSavePath];
}


bool STSystemFunction::isIosFiveUp()
{
   return  [[SystemFunctionManager sharedManager] isIosFiveUp];
}
