//
//  STSystemFunction.h
//  MysteryGuardians
//
//  Created by Steven.Xc.Tian on 13-7-9.
//
//

#ifndef __MysteryGuardians__SystemFunction__
#define __MysteryGuardians__SystemFunction__
#include "cocos2d.h"


USING_NS_CC;

typedef void (CCObject::*CallFunction)(void*,void*);
#define callBack(_SELECTOR) (CallFunction)(&_SELECTOR)

typedef enum {
    NewsModeLaunch = 0,
    NewsModeResume,
    NewsModeReturnHome,
    NewsModeCount,
}NewsBlastMode;

class STSystemFunction {
    
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
	virtual void rating();
    virtual bool isRate();
	virtual void go2MarketDetail(std::string packagename);
	virtual void contactUs();
	virtual void sendEmailAndAssetPic(std::string subject, std::string message, std::string assetName);
	virtual void sendEmailAndFilePic(std::string subject, std::string message, std::string fileName);
	std::string getSdCardPath();
	virtual bool checkNetworkAvailable();
	virtual bool isTabletAvailable();
	virtual float densityScale();
	virtual void rateUs();
	virtual void dismissRateUs();
    virtual void saveImage(std::string fileCache);
    virtual void saveImageWithImage(Image *aImage);
    virtual void goToPrivacy();
    virtual void showNewsBlast();
    virtual void showNewsBlast(NewsBlastMode);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    virtual void saveLocalImage(Image* image);
#endif
    /*
     * 向服务器发送后台统计数据，在APP出的时候发送
     */
	void endAnalytics();
    void refreshDCIM(std::string sFileDir);
};

#endif /* defined(__MysteryGuardians__SystemFunction__) */
