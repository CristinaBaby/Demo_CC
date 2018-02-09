//
//  AdsManager.h
//
//  Created by hujie2 on 15/7/14.
//
//

#ifndef __AdsSDK__AdsManager__
#define __AdsSDK__AdsManager__

#include <stdio.h>
#include "BannerAds.h"
#include "InterstitialAds.h"
#include "CrosspromoAds.h"
#include "RewardedAds.h"
#include "RectAds.h"
#include "SSCNativeAds.h"
#include <functional>
#include <string>
using namespace std;

typedef  enum
{
    kTypeBannerAds = 0x00000AA0,
    kTypeRectAds = 0x00000AA1,
    kTypeInterstitialAds = 0x00000AA2,
    kTypeCrosspromoAds = 0x00000AA3,
    kTypeRewardedAds = 0x00000AA4,
    kTypeNativeAds = 0x00000AA5,
} ADS_TYPE;

class AdsManagerDelegate;

class AdsManager:public BannerAdsDelegate,public RectAdsDelegate,public InterstitialAdsDelegate,public CrosspromoAdsDelegate,public RewardedAdsDelegate,public SSCNativeAdsDelegate
{
protected:
    AdsManager();
    ~AdsManager();
public:
    static AdsManager* getInstance();
    
protected:
    static AdsManager* m_pInstance;
    
public:
    /** preload all ads **/
    void preloadAllAds();
    /**
     * 指定加载某一种类型的广告
     */
    void preloadAds(ADS_TYPE adType);
    /**
     * 用于展示某种广告
     * 对于banner 和 rect 广告,永远返回true
     * 对于全屏广告,如果当前正在显示其他的广告,或者显示成功,返回true,否则返回false
     */
    bool showAds(ADS_TYPE adType);
    
    /**
     * 去掉某种广告,仅适用于banner和rect
     */
    void removeAds(ADS_TYPE adType);
    
    /**
     * 设置是否显示某种广告,仅适用于banner和rect
     */
    void setVisiable(ADS_TYPE adType,bool visiable);
    
    /**
     * 设置暂停某种广告,仅适用于banner和rect, 用于弹出页面的
     */
    void pauseAds(ADS_TYPE adType);
    
    /**
     * 设置恢复某种广告,仅适用于banner和rect, 用于弹出页面的
     */
    void resumeAds(ADS_TYPE adType);
    
    /**
     *设置调试模式
     **/
    void setDebugMode(bool bIsDebug);
    /**
     *设置banner 显示位置
     **/
    void setLayoutBanner(LAYOUT_TYPE kType);
    
    bool isFullScreenShowing();
protected:
    
    virtual void initBannerAdsDelegate();
    
    virtual void initInterstitialAdsDelegate();
    
    virtual void initRewardedAdsDelegate();
    
    virtual void initCrosspromoAdsDelegate();
    
    virtual void initRectAdsDelegate();
    
    virtual void initNativeAdsDelegate();
    
protected:
//    virtual void onRewardedAdsCollapsed(string unitId, int amount, bool bIsSkipped);
    
    int m_iFullScreenAdsShowingTag;
    
    bool m_bVisibleFlagBannber;
    
    bool m_bVisibleFlagRect;
    
    int m_iPauseResumeTagBanner;
    
    int m_iPauseResumeTagRect;
public:
    
#pragma mark - 事件回调 Cocos2dx 3.x-
    
    
#if __cplusplus > 201100L
    /**
     * cocos3.x 由于可以支持C++11,可以使用std::function简化代码
     */
    std::function<void(ADS_TYPE)> onAdsLoaded;
    std::function<void(std::string, ADS_TYPE)> onAdsLoadFailed;
    std::function<void(ADS_TYPE)> onAdsClicked;
    std::function<void(ADS_TYPE)> onAdsExpanded;
    std::function<void(ADS_TYPE)> onAdsCollapsed;
    std::function<void(std::string, int, bool)> onAdsRewarded;
#endif
    
#pragma mark - 事件回调 Cocos2dx 2.x-
    /**
     * 以下代码用于兼容cocos2.x,如果不需要兼容cocos2.x,可以忽略以下代码
     */
    /**
     * cocos2.x 只能是使用delege传递事件
     */
    AdsManagerDelegate *delegate;
private:
    void onBannerLoaded(void);
    void onBannerLoadFailed(std::string error);
    void onBannerClicked(void);
    void onBannerExpanded(void);
    void onBannerCollapsed(void);
    
    void onRectLoaded(void);
    void onRectLoadFailed(std::string error);
    void onRectClicked(void);
    void onRectExpanded(void);
    void onRectCollapsed(void);
    void onInterstitialLoaded(void);
    
    void onInterstitialFailed(std::string error);
    void onInterstitialClicked(void);
    void onInterstitialExpanded(void);
    void onInterstitialCollapsed(void);
    
    void onCrosspromoLoaded(void);
    void onCrosspromoFailed(std::string error);
    void onCrosspromoClicked(void);
    void onCrosspromoExpanded(void);
    void onCrosspromoCollapsed(void);
    
    void onRewardedLoaded(void);
    void onRewardedFailed(std::string error);
    void onRewardedClicked(void);
    void onRewardedExpanded(void);
    void onRewarded(std::string, int, bool);
    void onRewardedCollapsed();
    
    void onNativeAdsLoaded(void);
    void onNativeAdsLoadFailed(std::string error);
    void onNativeAdsClicked(void);
    void onNativeAdsExpanded(void);
    void onNativeAdsCollapsed(void);

};


class AdsManagerDelegate{
public:
    virtual void onAdsLoaded(ADS_TYPE adType){};
    virtual void onAdsLoadFailed(std::string error, ADS_TYPE adType){};
    virtual void onAdsClicked(ADS_TYPE adType){};
    virtual void onAdsExpanded(ADS_TYPE adType){};
    virtual void onAdsCollapsed(ADS_TYPE adType){};
    virtual void onAdsRewarded(std::string, int, bool){};
};


#endif /* defined(__AdsSDK__AdsManager__) */