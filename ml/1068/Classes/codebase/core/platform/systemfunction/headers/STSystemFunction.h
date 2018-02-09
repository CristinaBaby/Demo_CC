//
//  STSystemFunction.h
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

class STSystemFunction {

public:
	STSystemFunction();
	~STSystemFunction();
public:
    /**
     *  @brief Send Email by system default.
     *
     *  @param subject email subject.
     *  @param content email content.
     */
    virtual void sendEmail(const char* subject, const char* body);
    
    /**
     *  @brief Send Email with pic by system default.
     *
     *  @param subject email subject.
     *  @param content email content.(html style)
     *  @param content email fileName.
     */
    virtual void sendEmailAndFilePic(const char* subject, const char* message, const char* fileName);
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
    
    virtual int listAssetFiles(const std::string& path);
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
    
    /**
     *  @brief rateUs.
     */
    virtual void rateUs();
    /********************these functions will be CDEPRECATED_ATTRIBUTE*****************/
	virtual void rating();
	virtual void go2MarketDetail(std::string packagename);
	virtual void contactUs();
	virtual float densityScale();
};

#endif /* defined(__MysteryGuardians__SystemFunction__) */
