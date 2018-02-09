//
//  AppManager.h
//  PaintDress
//
//  Created by maxiang on 6/16/15.
//
//

#ifndef __PaintDress__AppManager__
#define __PaintDress__AppManager__

#include "../Module/AdsLoading/AdCross/STAds.h"
#include "ResManager.h"

class AppManager
{
public:
    static AppManager* getInstance();
    virtual ~AppManager();
    AppManager();
    
    void setOpenSound(bool isOpen);
    bool isOpenSound(){return _isOpenSound;};
    bool isFirstTimeLaunchApp(){return _isFirstTimeLaunchApp;};
    void setIsFirstTimeLaunchApp(bool is){_isFirstTimeLaunchApp = is;};
    bool isRemoveAds(){return _hasRemoveAds;};

    void playColorBgm();
    void playStartBgm();
    void playSelectBgm();
    void playDressBgm();

    void playPlayEffect();
    void playButtonEffect();
    void playDressEffect();
    void playPopupEffect();
    void playColoringEffect();
    void pauseColoringEffect();
    void playCameraEffect();

    //coins
    int getCoin(){return _coin;};
    int addCoin(const int coin);
    bool purchaseRes(Res& res);
    
    void removeAds();
    
    //ads
    void requestBannerAd();
    void requestFullScreenAd();
    void requestCrossPromoAd();

    void showBannerAd();
    void hideBannerAd();
    
    //rate us
    void rateUsLogic();

protected:
    bool _isOpenSound;
    bool _hasRemoveAds;
    bool _isFirstTimeLaunchApp;
    
    int _coin;
    
    //bgm id
    int _coloringEffectId;

    STAds _stAds;
};

#endif /* defined(__PaintDress__AppManager__) */
