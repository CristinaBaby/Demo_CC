//
//  STSystemFunction.cpp
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-9.
//
//

#include "STSystemFunction.h"
#include "SystemFunctionManager_IOS.h"

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

void STSystemFunction::sendEmailAndFilePic(const char* subject, const char* message, const char* fileName)
{
    [[SystemFunctionManager sharedManager]sendEmailWithPic:[NSString stringWithCString:
                                                            subject encoding:NSUTF8StringEncoding]
                                                   Content:[NSString stringWithCString:
                                                            message encoding:NSUTF8StringEncoding]
                                                      Path:[NSString stringWithCString:
                                                            fileName encoding:NSUTF8StringEncoding]];
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
    log("no implementation on IOS");
}

std::string STSystemFunction::getSDCardPath()
{
    log("no implementation on IOS");
    return "";
}

bool STSystemFunction::checkNetworkAvailable()
{
    return [[SystemFunctionManager sharedManager] isNetworkAvaliable];
}

bool STSystemFunction::isTablet()
{
    log("no implementation on IOS");
    return false;
}

void STSystemFunction::showPrivacy()
{
    [[SystemFunctionManager sharedManager]showPrivacy];
}

void STSystemFunction::showNewsBlast(NewsBlastMode mode)
{
    [[SystemFunctionManager sharedManager] showNewsBlast:mode];
}

void STSystemFunction::rateUs()
{
    [[SystemFunctionManager sharedManager] rateUs];
}

void STSystemFunction::makeToast(const char* message)
{
    log("no implementation on IOS");
}

void STSystemFunction::rating()
{
    log("no implementation on IOS");
}

void STSystemFunction::contactUs()
{
    log("no implementation on IOS");
}

float STSystemFunction::densityScale()
{
    log("no implementation on IOS");
    return 0.0f;
}

void STSystemFunction::saveToAlbum(Image* img,std::function<void(bool)> callback, std::string proDir)
{
    [[SystemFunctionManager sharedManager]saveToAlbum:img withCallback:callback];
}

int STSystemFunction::listAssetFiles(const std::string& path)
{
    log("no implementation on IOS");
    return 0;
}

void STSystemFunction::refreshDCIM(const std::string &sFileDir)
{
    log("no implementation on IOS");
}