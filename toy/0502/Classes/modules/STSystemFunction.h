//
//  STSystemFunction.h
//  MysteryGuardians
//
//  Created by Steven.Xc.Tian on 13-7-9.
//
//

#ifndef __MysteryGuardians__SystemFunction__
#define __MysteryGuardians__SystemFunction__
#include <string>
#include "cocos2d.h"

class STSystemFunction
{
public:
	STSystemFunction();
	~STSystemFunction();
public:
    /**
     *  @brief Send Email by system default.
     *
     *  @param subject email subject.
     *  @param content email subject.
     */
    virtual void sendEmail(const char* subject, const char* body);
    
    /**
     *
     */
    virtual void popAlertDialog(const char* message);
    virtual void makeToast(const char* message);
    
    /**
     *
     */
    virtual void showMoreGame();
	virtual void rating(const char* appleID, const char* message);
    virtual void rating();
	virtual void go2MarketDetail(std::string packagename);
	virtual void contactUs();
	virtual void sendEmailAndAssetPic(std::string subject, std::string message, std::string assetName);
	virtual void sendEmailAndFilePic(std::string subject, std::string message, std::string fileName);
	std::string getSdCardPath();
	virtual bool checkNetworkAvailable();
	virtual bool isTabletAvailable();
	virtual float densityScale();
    
    virtual void go2PrivacyPage();
    virtual void openURL(std::string url);
    virtual void endSession();
    
    //add for jigsaw
    std::string getPlatformTempPath();
    
    void saveLocalImage(cocos2d::CCImage *image);
    void onFlurryEvent(const std::string& eventName);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // Android
    void refreshDCIM(std::string filename);
#endif
};

#endif /* defined(__MysteryGuardians__SystemFunction__) */
