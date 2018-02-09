/*
 * STSystemFunction.cpp
 *
 *  Created on: 2013-9-2
 *      Author: Steven.Xc.Tian
 */
#include "STSystemFunction.h"
#include "SystemFunctionManager_Android.h"
#include "FileUtility.h"


STSystemFunction::STSystemFunction()
{
}
STSystemFunction::~STSystemFunction()
{
}

void STSystemFunction::sendEmail(const char* subject, const char* body)
{
	SystemFunctionManager::getInstance()->sendEmail(subject, body);
}

void STSystemFunction::popAlertDialog(const char* message)
{
	SystemFunctionManager::getInstance()->popAlertDialog(message);
}

void STSystemFunction::showMoreGame()
{
	SystemFunctionManager::getInstance()->showMoreGame();
}

int STSystemFunction::listAssetFiles(const std::string& path)
{
    return SystemFunctionManager::getInstance()->listAssetFiles(path);
}

void STSystemFunction::rating()
{
	SystemFunctionManager::getInstance()->rating();
}

void STSystemFunction::go2MarketDetail(std::string packagename)
{
	SystemFunctionManager::getInstance()->go2MarketDetail(packagename);
}

void STSystemFunction::contactUs()
{
	SystemFunctionManager::getInstance()->contactUs();
}

void STSystemFunction::sendEmailAndFilePic(const char* subject, const char* message,const char* fileName)
{
	SystemFunctionManager::getInstance()->sendEmailAndFilePic(subject,message,fileName);
}

std::string STSystemFunction::getSDCardPath()
{
	return SystemFunctionManager::getInstance()->getSdCardPath();
}

bool STSystemFunction::checkNetworkAvailable()
{
	return SystemFunctionManager::getInstance()->checkNetworkAvailable();

}

bool STSystemFunction::isTablet()
{
	return SystemFunctionManager::getInstance()->isTabletAvailable();

}

float STSystemFunction::densityScale()
{
	return SystemFunctionManager::getInstance()->densityScale();
}

void STSystemFunction::makeToast(const char* message)
{
	
}

void STSystemFunction::rateUs(){
	SystemFunctionManager::getInstance()->rateUs();
}

void STSystemFunction::saveToAlbum(Image* img,std::function<void(bool)> callback,std::string proDir)
{
	FileUtility::createDirectory((this->getSDCardPath()+"/"+proDir).c_str());
    struct timeval tv;
    gettimeofday(&tv,NULL);
    long time =  tv.tv_sec * 1000 + tv.tv_usec / 1000;
	string path = this->getSDCardPath()+"/"+ proDir + "/"+ StringUtils::format("%ld.jpg",time);

	bool issuccess= img->saveToFile(path.c_str());
	callback(issuccess);
	this->refreshDCIM(path);
}

void STSystemFunction::showPrivacy()
{
    
}

void STSystemFunction::showNewsBlast(NewsBlastMode mode){
    CCLOG("%s", __func__);
    SystemFunctionManager::getInstance()->doNewsBlast();
}

void STSystemFunction::refreshDCIM(const std::string& sFileDir)
{
    CCLOG("%s", __func__);
    SystemFunctionManager::getInstance()->refreshDCIM(sFileDir);
}
