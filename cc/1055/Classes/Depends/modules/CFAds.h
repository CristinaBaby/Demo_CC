//
//  CFAds.h
//  MysteryGuardians
//
//  Created by Steven.Xc.Tian on 13-7-3.
//
//

#ifndef __MysteryGuardians__CFAds__
#define __MysteryGuardians__CFAds__

#include "cocos2d.h"
#include "IAPManager.h"

typedef enum
{
    TypeDefault = 0,     // 显示系统广告
    TypeCrossPromo = 1,  // 显示交叉推广广告
}InterstitialType;

class CFAdsDelegate;

class CFAds
{
public:
	CFAds();
	virtual ~CFAds();
    
    void setAdsDelegate(CFAdsDelegate* delegate);
    
public:
	/**
	 * @brief request and show banner ads
	 */
	void requestAds();
    
    void requestInterstitialAds();
    /**
	 * @brief request a interstitial ads
	 */
    void requestInterstitialAds(InterstitialType type);
    
	/**
	 * @brief remove shown ads or cancel suspending ads
	 */
	void removeAds();
    
    /**
	 * @brief set visibility of ad view, not remove ad from parent node
	 */
	void setAdsVisibility(bool visibility);
	/**
	*    get banner ad size                                                                  
	*/
	void getBannerSize(float[]);
    
public:
    /**
     * @brief destroy environment about ads
     */
    static void destroy();
    
    // when interstitial ads shown, set it true
    static bool interstitialShown;
    
    // when user not purchased anything can remove ads, set it true;
    static bool willShowAd;

	// indicate current banner ad status
	static bool bannerAdVisibility;
};

class CFAdsDelegate
{
public:
    /**
	 * @brief This event is called once an ad loads successfully.
	 */
	virtual void onBannerLoadSuccessfully() = 0;
    
	/**
	 * @brief This event is called if an ad fails to load.
	 */
	virtual void onBannerLoadFailed(int errorCode) = 0;
    
    /**
     * @brief Sent when the banner has just taken over the screen.
     */
    virtual void onBannerClicked() = 0;
    
    /**
     * @brief Sent when an expanded banner has collapsed back to its original size.
     */
	virtual void onBannerDismissed() = 0;
    
    /**
	 * @brief This event is called once an interstitial ad loads successfully.
	 */
    virtual void onInterstitialAdLoaded() = 0;
    
    /**
	 * @brief This event is called if an interstitial ad fails to load.
	 */
    virtual void onInterstitialAdFailed(int errorCode) = 0;
    
    /**
	 * @brief This event is called if an interstitial ad shown.
	 */
    virtual void onInterstitialShown() = 0;
    
    /**
	 * @brief This event is called if an interstitial ad closed.
	 */
    virtual void onInterstitialDismissed() = 0;
};

#endif /* defined(__MysteryGuardians__CFAds__) */
