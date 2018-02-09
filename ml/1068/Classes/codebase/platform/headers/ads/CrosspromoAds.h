//
//  CrosspromoAds.h
//  Ads_Framework
//
//  Created by hujie2 on 15/7/9.
//
//

#ifndef __Ads_Framework__CrosspromoAds__
#define __Ads_Framework__CrosspromoAds__

#include <stdio.h>
#include <functional>
#include <string>

class CrosspromoAdsDelegate;

class CrosspromoAds
{
protected:
    CrosspromoAds()
#if __cplusplus > 201100L
    :onCrosspromoLoaded(nullptr)
    ,onCrosspromoFailed(nullptr)
    ,onCrosspromoClicked(nullptr)
    ,onCrosspromoExpanded(nullptr)
    ,onCrosspromoCollapsed(nullptr)
    ,delegate(nullptr)
#else
    :delegate(NULL)
#endif
    {};
    ~CrosspromoAds();
    
public:
    static CrosspromoAds* getInstance();
    
protected:
    static CrosspromoAds* m_pInstance;
    
public:
    
    void preload();
    
    bool isPreloaded();
    
    bool show();
    
    void setAutoShow(bool);
    
    bool isAutoShow();

    void setDebugMode(bool bIsDebug);
    
    
public:
    
    CrosspromoAdsDelegate* delegate;
    
#if __cplusplus > 201100L
    std::function<void()> onCrosspromoLoaded;
    std::function<void(std::string)> onCrosspromoFailed;
    std::function<void()> onCrosspromoExpanded;
    std::function<void()> onCrosspromoClicked;
    std::function<void()> onCrosspromoCollapsed;
#endif
};

class CrosspromoAdsDelegate{
public:
    virtual void onCrosspromoLoaded(void){};
    virtual void onCrosspromoFailed(std::string error){};
    virtual void onCrosspromoClicked(void){};
    virtual void onCrosspromoExpanded(void){};
    virtual void onCrosspromoCollapsed(void){};
};

#endif /* defined(__Ads_Framework__CrosspromoAds__) */
