//
//  InterstitialAds.h
//  Ads_Framework
//
//  Created by Jimmy.Hu on 15/7/10.
//
//

#ifndef __Ads_Framework__InterstitialAds__
#define __Ads_Framework__InterstitialAds__

#include <stdio.h>
#include <functional>
#include <string>

class InterstitialAdsDelegate;

class InterstitialAds
{
protected:
    InterstitialAds()
#if __cplusplus > 201100L
    :onInterstitialLoaded(nullptr)
    ,onInterstitialFailed(nullptr)
    ,onInterstitialClicked(nullptr)
    ,onInterstitialExpanded(nullptr)
    ,onInterstitialCollapsed(nullptr)
    ,delegate(nullptr)
#else
    :delegate(NULL)
#endif
    {};
    ~InterstitialAds();
public:
    static InterstitialAds*  getInstance();
protected:
    static InterstitialAds* m_pInstance;
public:
    
    void preload();
    
    bool isPreloaded();
    
    bool show();
    
    void setAutoShow(bool bIsAuto);
    
    bool isAutoShow();

    void setDebugMode(bool bIsDebug);
    
public:
    
    /**
     *@brief bannner ads call back
     **/
    
    InterstitialAdsDelegate *delegate;
#if __cplusplus > 201100L
    std::function<void()> onInterstitialLoaded;
    std::function<void(std::string)> onInterstitialFailed;
    std::function<void()> onInterstitialClicked;
    std::function<void()> onInterstitialExpanded;
    std::function<void()> onInterstitialCollapsed;
#endif
};

class InterstitialAdsDelegate{
public:
    virtual void onInterstitialLoaded(void){};
    virtual void onInterstitialFailed(std::string error){};
    virtual void onInterstitialClicked(void){};
    virtual void onInterstitialExpanded(void){};
    virtual void onInterstitialCollapsed(void){};
};

#endif /* defined(__Ads_Framework__InterstitialAds__) */
