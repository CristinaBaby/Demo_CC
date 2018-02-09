//
//  SSCNativeAds.h
//  AdsDemo
//
//  Created by xuzepei on 16/1/6.
//
//

#ifndef __Ads_Framework__SSCNativeAds__
#define __Ads_Framework__SSCNativeAds__

#include <stdio.h>
#include <functional>
#include <string>

typedef enum
{
    NALTTopLeft = 0x000000A0,
    NALTTopCenter = 0x000000A1,
    NALTTopRight = 0x000000A2,
    
    NALTCenterLeft = 0x000000A3,
    NALTCenter = 0x000000A4,
    NALTCenterRight = 0x000000A5,
    
    NALTBottomLeft = 0x000000A6,
    NALTBottomCenter = 0x000000A7,
    NALTBottomRight = 0x000000A8
    
}NativeAdsLayoutType;

class SSCNativeAdsDelegate;

class SSCNativeAds
{
protected:
    /**
     *@brief constructor
     **/
    SSCNativeAds()
#if __cplusplus > 201100L
    :onNativeAdsLoaded(nullptr)
    ,onNativeAdsLoadFailed(nullptr)
    ,onNativeAdsClicked(nullptr)
    ,onNativeAdsExpanded(nullptr)
    ,onNativeAdsCollapsed(nullptr)
    ,delegate(nullptr)
#else
    :delegate(NULL)
#endif
    {
    };
    ~SSCNativeAds();
public:
    
    SSCNativeAdsDelegate *delegate;
    
    static SSCNativeAds* getInstance();
    
    void preload();
    void show();
    
    void setAutoShow(bool bIsAuto);
    bool isAutoShow();
    
    void setVisible(bool bVisibility);
    void remove();
    void setDebugMode(bool bIsDebug);
    void setLayout(NativeAdsLayoutType kType);
    
protected:
    static SSCNativeAds* m_pInstance;
    
public:
    
#if __cplusplus > 201100L
    /**
     *@brief bannner ads call back
     **/
    std::function<void()> onNativeAdsLoaded;
    std::function<void(std::string)> onNativeAdsLoadFailed;
    std::function<void()> onNativeAdsClicked;
    std::function<void()> onNativeAdsExpanded;
    std::function<void()> onNativeAdsCollapsed;
#endif
};
class SSCNativeAdsDelegate{
public:
    virtual void onNativeAdsLoaded(void){};
    virtual void onNativeAdsLoadFailed(std::string error){};
    virtual void onNativeAdsClicked(void){};
    virtual void onNativeAdsExpanded(void){};
    virtual void onNativeAdsCollapsed(void){};
};
#endif /* defined(__Ads_Framework__SSCNativeAds__) */