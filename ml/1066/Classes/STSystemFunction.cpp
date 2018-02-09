/*
 * STSystemFunction.cpp
 *
 *  Created on: 2013-9-2
 *      Author: Steven.Xc.Tian
 */
#include "STSystemFunction.h"
#include "ST_SystemFunction_Android.h"
//#include "STFileUtility.h"
//#include "Dialog.h"

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

void STSystemFunction::sendEmailAndAssetPic(std::string subject, std::string message, std::string assetName)
	{
		SystemFunctionManager::getInstance()->sendEmailAndAssetPic(subject,message,assetName);
	}
void STSystemFunction::sendEmailAndFilePic(std::string subject, std::string message,std::string fileName)
{
	SystemFunctionManager::getInstance()->sendEmailAndFilePic(subject,message,fileName);

}
std::string STSystemFunction::getSdCardPath()
{
	return SystemFunctionManager::getInstance()->getSdCardPath();
}

bool STSystemFunction::checkNetworkAvailable()
{
	return SystemFunctionManager::getInstance()->checkNetworkAvailable();

}

bool STSystemFunction::isTabletAvailable()
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
//	SystemFunctionManager::getInstance()->rateUs();
}
void STSystemFunction::dismissRateUs(){
//	SystemFunctionManager::getInstance()->dismissRateUs();
}

void STSystemFunction::saveImage(std::string fileCache,CCObject* selector,CallFunction callback){


}

void STSystemFunction::saveToAlumn(Image *apCCImg)
{
//	struct timeval tv;
//	gettimeofday(&tv,NULL);
//	long time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
//
//    String* _time=String::createWithFormat("%ld", time);
//    STFileUtility::createDirectory((getSdCardPath()+"/Animal fireman").c_str());
//    string path=getSdCardPath()+"/Animal fireman/"+_time->_string+".jpg";
//
//    CCLOG("Android: AlumnAdapter::saveToAlumn at %s", path.c_str());
//    bool issuccess=apCCImg->saveToFile(path.c_str());
//    if (issuccess)
//    {
////        _saveTxt=dm->getLanguage("photo_save",Util::getLanguageType()).c_str();
////        popAlertDialog("Your image was saved to your Camera Roll!");
//
//        Dialog* dialog= Dialog::create(Size(650,400), (void*)kDialogNotice, Dialog::DIALOG_TYPE_SINGLE);
//        dialog->setContentText("Your image was saved to your Camera Roll!");
//        dialog->setPositiveBtnText("OK");
//        Director::getInstance()->getRunningScene()->addChild(dialog, 9999);
//    }else
//    {
////        popAlertDialog("Your image was unable to be saved. Please check your device storage and try again.");
////        _saveTxt=dm->getLanguage("photo_save_failed",Util::getLanguageType()).c_str();
//
//        Dialog* dialog= Dialog::create(Size(650,400), (void*)kDialogNotice, Dialog::DIALOG_TYPE_SINGLE);
//        dialog->setContentText("Your image was unable to be saved. Please check your device storage and try again.");
//        dialog->setPositiveBtnText("OK");
//        Director::getInstance()->getRunningScene()->addChild(dialog, 9999);
//    }
//    refreshDCIM(path);
}

void STSystemFunction::goToPrivacy()
{
    
}

void STSystemFunction::showNewsBlast()
{
    CCLOG("%s", __func__);
    SystemFunctionManager::getInstance()->doNewsBlast();
}


void STSystemFunction::showNewsBlast(NewsBlastMode mode){
    SystemFunctionManager::getInstance()->doNewsBlast();
}

void STSystemFunction::endSession()
{
//    CCLOG("%s", __func__);
    SystemFunctionManager::getInstance()->endSession();
}

void STSystemFunction::refreshDCIM(std::string sFileDir)
{
    SystemFunctionManager::getInstance()->refreshDCIM(sFileDir);
}

void STSystemFunction::createDialog(std::string meaasge, std::string positive,std::string negative,Ref*,CallFunction)
{

}

void STSystemFunction::createDialog(std::string meaasge, std::string positive)
{

}
