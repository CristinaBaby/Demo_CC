//
//  CFSystemFunction.cpp
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-9.
//
//

#include "CFSystemFunction.h"
#include "SystemFunctionManager_IOS.h"

CFSystemFunction::CFSystemFunction()
{
}

CFSystemFunction::~CFSystemFunction()
{
}

void CFSystemFunction::sendEmail(const char* subject, const char* body)
{
    [[SystemFunctionManager sharedManager]sendEmail : [NSString stringWithCString:subject encoding:NSUTF8StringEncoding]
                                             Content:[NSString stringWithCString:body encoding:NSUTF8StringEncoding]];
}

void CFSystemFunction::popAlertDialog(const char *message)
{
    [[SystemFunctionManager sharedManager]popAlertDialog:[NSString stringWithCString:message encoding:NSUTF8StringEncoding]];
}

void CFSystemFunction::showMoreGame()
{
    [[SystemFunctionManager sharedManager]showMorePage];
}

void CFSystemFunction::go2MarketDetail(std::string packagename)
{
//    [[SystemFunctionManager sharedManager]go2Market:[NSString stringWithCString:packagename.c_str() encoding:NSUTF8StringEncoding]];
}

std::string CFSystemFunction::getSDCardPath()
{
    return "";
}

bool CFSystemFunction::checkNetworkAvailable()
{
    return [[SystemFunctionManager sharedManager] isNetworkAvaliable];
}

bool CFSystemFunction::isTablet()
{
    return false;
}

void CFSystemFunction::showPrivacy()
{
    [[SystemFunctionManager sharedManager]showPrivacy];
}

void CFSystemFunction::showNewsBlast(NewsBlastMode mode)
{
    [[SystemFunctionManager sharedManager] showNewsBlast:mode];
}

void CFSystemFunction::rateUs()
{
    [[SystemFunctionManager sharedManager] rateUs];
}

void CFSystemFunction::makeToast(const char* message)
{
}

void CFSystemFunction::rating()
{
}

void CFSystemFunction::contactUs()
{
}

void CFSystemFunction::sendEmailAndAssetPic(std::string subject, std::string message, std::string assetName)
{
}

void CFSystemFunction::sendEmailAndFilePic(std::string subject, std::string message, std::string fileName)
{
    [[SystemFunctionManager sharedManager]sendEmailWithPic:[NSString stringWithCString:subject.c_str() encoding:NSUTF8StringEncoding] Content:[NSString stringWithCString:message.c_str() encoding:NSUTF8StringEncoding] Path:[NSString stringWithCString:fileName.c_str() encoding:NSUTF8StringEncoding]];
    
}

float CFSystemFunction::densityScale()
{
    return 0.0f;
}


void CFSystemFunction::saveToAlbum(Image* img,std::function<void(bool)> callback, std::string proDir)
{
    [[SystemFunctionManager sharedManager]saveToAlbum:img withCallback:callback];
}

//void CFSystemFunction::endAnalytics()
//{
//    
//}
void CFSystemFunction::endSession()
{
    
}
void CFSystemFunction::refreshDCIM(const std::string &sFileDir)
{
    
}