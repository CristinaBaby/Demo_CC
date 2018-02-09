//
//  CBAppManager.h
//  ColorBook
//
//  Created by maxiang on 4/21/15.
//
//

#ifndef __ColorBook__CBAppManager__
#define __ColorBook__CBAppManager__

#include "cocos2d.h"
#include "CBAppGlobal.h"

CB_BEGIN_NAMESPACE

class AppManager
{
public:
    static AppManager* getInstance();
    virtual ~AppManager();
    AppManager();

    void setOpenBgm(bool isOpen);
    void setOpenEffect(bool isOpen);
    
    bool isOpenBgm(){return _isOpenBgm;};
    bool isOpenEffect(){return _isOpenEffect;}; 
    
    void playBgm();
    void playButtonEffect();
    void playPensChangeEffect();
    void playFinishColorEffect();
    
    //first launch app
    bool isFirstLaunchApp(){return _isFirstLaunchApp;};
    void setIsFirstLaunchApp(bool isFirstLaunch){_isFirstLaunchApp = isFirstLaunch;};

    //ads
    void requestFullScreenAd();
    void requestCrossPromoAd();
    void requestBannerAd();
    void setBannerAdVisible(bool visible);
    
    /* Rate us logic, when user save picture 3 times, show rate us */
    void rateUsLogic();

protected:
    bool _isOpenBgm;
    bool _isOpenEffect;
    
    int _saveTimes;
    
    bool _isShowBannerAds;
    bool _isFirstLaunchApp;
};



CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBAppManager__) */
