//
//  AdHelper.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-25.
//
//

#ifndef __LVUP002__AdHelper__
#define __LVUP002__AdHelper__

#include "../layer/AdsLoadingLayer.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
class AdHelper : public STAdsDelegate
#else
class AdHelper
#endif
{
    AdHelper(){}
    
public:
    virtual ~AdHelper(){};
    
    static AdHelper* getInstance();
    static void destroy();
    
public:
    static void requestAds();
    
    static void requestInterstitialAds();
    
	static void removeAds();
    
public:
    /* derived from STAdsDelegate */
    virtual void onBannerLoadSuccessfully();
    
	virtual void onBannerLoadFailed(int errorCode);
    
    virtual void onBannerClicked();
    
	virtual void onBannerDismissed();
    
    virtual void onInterstitialAdLoaded();
    
    virtual void onInterstitialAdFailed(int errorCode);
    
    virtual void onInterstitialShown();
    
    virtual void onInterstitialDismissed();
};

#endif /* defined(__LVUP002__AdHelper__) */
