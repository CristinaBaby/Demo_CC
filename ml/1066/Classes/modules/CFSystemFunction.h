//
//  CFSystemFunction.h
//  MysteryGuardians
//
//  Created by Steven.Xc.Tian on 13-7-9.
//
//

#ifndef __KidsFramework_CocosV3_2__SystemFunction__
#define __KidsFramework_CocosV3_2__SystemFunction__
#include <string>
#include "cocos2d.h"

USING_NS_CC;

typedef enum {
    NewsModeLaunch = 0,
    NewsModeResume,
    NewsModeReturnHome,
    NewsModeCount,
}NewsBlastMode;

class CFSystemFunction {

public:
	CFSystemFunction();
	~CFSystemFunction();
public:
    /**
     *  @brief Send Email by system default.
     *
     *  @param subject email subject.
     *  @param content email content.
     */
    virtual void sendEmail(const char* subject, const char* body);
    
    /**
     * @brief pop a system default dialog
     *
     * @param message
     */
    virtual void popAlertDialog(const char* message);
    
    /**
     *  @brief  check network is available
     *
     *  @return true:network is connective.
     */
	virtual bool checkNetworkAvailable();
    
    /**
     *  @brief  check current device is tablet(android)/ipad(ios).
     *
     *  @return true:is tablet(android)/ipad(ios)
     */
	virtual bool isTablet();
    
    /**
     *  @brief save image to album
     *
     *  @param Image*   img->get from RenderTexture->newImage
     *  @param callback callback called after complete save img.
     */
    virtual void saveToAlbum(Image*,std::function<void(bool)> callback,std::string proDir = "default");
    
    /***********************some function for common libs.*********************/
    /**
     *  @brief show more game page.
     */
    virtual void showMoreGame();
    
    /**
     *  @brief show privacy page
     */
    virtual void showPrivacy();
    
    /**
     *  @brief show NewsBlast
     *
     *  @param NewsBlastMode  NewsModeLaunch  or  NewsModeResume
     */
    virtual void showNewsBlast(NewsBlastMode);
    
    /***********************some function only valid for Android.*********************/
    
    /**
     * @brief only valid for Android
     */
//    virtual void endAnalytics();
    virtual void  endSession();

    /**
     *  @brief  get SD card path.only valid for android OS.
     *
     *  @return SD card path.
     */
	std::string getSDCardPath();
    
    /**
     *  @brief only valid for android OS.
     *
     *  @param message
     */
    virtual void makeToast(const char* message);
    
    /**
     *  @brief refresh .only valid for Android OS.
     *
     *  @param sFileDir path
     */
    virtual void refreshDCIM(const std::string& sFileDir);
    
    /********************these functions will be CDEPRECATED_ATTRIBUTE*****************/
	virtual void rating();
	virtual void go2MarketDetail(std::string packagename);
	virtual void contactUs();
	virtual void sendEmailAndAssetPic(std::string subject, std::string message, std::string assetName);
	virtual void sendEmailAndFilePic(std::string subject, std::string message, std::string fileName);
	virtual float densityScale();
	virtual void rateUs();
    
//    void dismissRateUs();
};

#endif /* defined(__MysteryGuardians__SystemFunction__) */
