//
//  BannerAds.h
//
//  Created by hujie2 on 15/7/9.
//
//

#ifndef __Ads_Framework__BannerAds__
#define __Ads_Framework__BannerAds__

#include <stdio.h>
#include <functional>
#include <string>

typedef enum
{
    kLayoutTopLeft = 0x000000A0,
    kLayoutTopCenter = 0x000000A1,
    kLayoutTopRight = 0x000000A2,
    
    kLayoutCenterLeft = 0x000000A3,
    kLayoutCenter = 0x000000A4,
    kLayoutCenterRight = 0x000000A5,
    
    kLayoutBottomLeft = 0x000000A6,
    kLayoutBottomCenter = 0x000000A7,
    kLayoutBottomRight = 0x000000A8
    
}LAYOUT_TYPE;

class BannerAdsDelegate;

class BannerAds
{
protected:
    /**
     *@brief constructor
     **/
    BannerAds()
    
    
#if __cplusplus > 201100L
    :onBannerLoaded(nullptr)
    ,onBannerLoadFailed(nullptr)
    ,onBannerClicked(nullptr)
    ,onBannerCollapsed(nullptr)
    ,onBannerExpanded(nullptr)
    ,delegate(nullptr)
#else
    :delegate(NULL)
#endif
    {
    };
    ~BannerAds();
public:
    
    BannerAdsDelegate *delegate;
    
    static BannerAds* getInstance();
    
    void preload();
    
    void show();
    
    void setAutoShow(bool bIsAuto);
    
    bool isAutoShow();
    
    
    void setVisible(bool bVisibility);
    
    /**
     *@brief remove banner ads
     **/
    void remove();
    
    void setDebugMode(bool bIsDebug);
    
    /**
     *@brief  set the position of banner
     **/
    void setLayout(LAYOUT_TYPE kType);
    
protected:
    static BannerAds* m_pInstance;
    
public:
    
#if __cplusplus > 201100L
    /**
     *@brief bannner ads call back
     **/
    std::function<void()> onBannerLoaded;
    std::function<void(std::string)> onBannerLoadFailed;
    std::function<void()> onBannerClicked;
    std::function<void()> onBannerExpanded;
    std::function<void()> onBannerCollapsed;
#endif
};
class BannerAdsDelegate{
public:
    virtual void onBannerLoaded(void){};
    virtual void onBannerLoadFailed(std::string error){};
    virtual void onBannerClicked(void){};
    virtual void onBannerExpanded(void){};
    virtual void onBannerCollapsed(void){};
};
#endif /* defined(__Ads_Framework__BannerAds__) */