//
//  RewardedAds.h
//  Ads_Framework
//
//  Created by hujie2 on 15/7/10.
//
//

#ifndef __Ads_Framework__RewardedAds__
#define __Ads_Framework__RewardedAds__

#include <stdio.h>
#include <functional>
#include <string>

class RewardedAdsDelegate;

class RewardedAds
{
protected:
    RewardedAds()
#if __cplusplus > 201100L
    :onRewardedLoaded(nullptr)
    ,onRewardedFailed(nullptr)
    ,onRewardedClicked(nullptr)
    ,onRewardedExpanded(nullptr)
    ,onRewardedCollapsed(nullptr)
    ,delegate(nullptr)
#else
    :delegate(NULL)
#endif
    {};
    ~RewardedAds();
public:
    static RewardedAds*  getInstance();
protected:
    static RewardedAds* m_pInstance;
public:
    
    void preload();
    
    bool isPreloaded();
    
    bool show();
    
    void setAutoShow(bool);
    
    bool isAutoShow();

    void setDebugMode(bool bIsDebug);
    
public:
    
    /**
     *@brief bannner ads call back
     **/

    RewardedAdsDelegate *delegate;
    
#if __cplusplus > 201100L
    std::function<void()> onRewardedLoaded;
    std::function<void(std::string)> onRewardedFailed;
    std::function<void()> onRewardedClicked;
    std::function<void()>onRewardedExpanded;
    std::function<void(std::string, int, bool)> onRewarded;
    std::function<void()>onRewardedCollapsed;
#endif
};

class RewardedAdsDelegate{
public:
    virtual void onRewardedLoaded(void){};
    virtual void onRewardedFailed(std::string error){};
    virtual void onRewardedClicked(void){};
    virtual void onRewardedExpanded(void){};
    virtual void onRewarded(std::string, int, bool){};
    virtual void onRewardedCollapsed(){};
};

#endif /* defined(__Ads_Framework__RewardedAds__) */
