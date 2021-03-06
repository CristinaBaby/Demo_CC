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

USING_NS_CC;

typedef void (CCObject::*CallFunction)(void*,void*);
#define callBack(_SELECTOR) (CallFunction)(&_SELECTOR)

#define NotifyParentalGateAnswerRight "ParentGate"
#define Notify_CLoseMoreGame "closeMoreGame"


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
    virtual void saveImage(std::string fileCache,Ref*,CallFunction);
    virtual void goToPrivacy();
    virtual void showNewsBlast();
    virtual void showNewsBlast(NewsBlastMode);
    virtual void createDialog(std::string meaasge, std::string positive,std::string negative,Ref*,CallFunction);
    virtual void createDialog(std::string meaasge, std::string positive);
//    virtual void parentalGate();

    /*
     * 向服务器发送后台统计数据，在APP出的时候发送
     */
	void endSession();
    //
    void refreshDCIM(std::string sFileDir);
    
    void saveToAlumn(Image *apCCImg);
};

#endif /* defined(__MysteryGuardians__SystemFunction__) */
