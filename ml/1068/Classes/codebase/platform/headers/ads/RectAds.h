//
//  RectAds.h
//  Ads_Framework
//
//  Created by hujie2 on 15/7/10.
//
//

#ifndef __Ads_Framework__RectAds__
#define __Ads_Framework__RectAds__

#include <stdio.h>
#include <functional>
#include <string>

class RectAdsDelegate;

class RectAds
{
protected:
    /**
     *@brief constructor
     **/
    RectAds()
#if __cplusplus > 201100L
    :onRectLoaded(nullptr)
    ,onRectLoadFailed(nullptr)
    ,onRectClicked(nullptr)
    ,onRectCollapsed(nullptr)
    ,onRectExpanded(nullptr)
    ,delegate(nullptr)
#else
    :delegate(NULL)
#endif
    {
    };
    ~RectAds();
public:
    
    static RectAds* getInstance();
    
    void preload();
    
    void show();
    
    void setAutoShow(bool bIsAuto);
    
    bool isAutoShow();
    
    
    void setVisible(bool bVisibility);
    
    /**
     *@brief remove Rect ads
     **/
    void remove();

    void setDebugMode(bool bIsDebug);
protected:
    static RectAds* m_pInstance;
    
public:
    /**
     *@brief bannner ads call back
    **/
    
    RectAdsDelegate *delegate;
#if __cplusplus > 201100L
    std::function<void()> onRectLoaded;
    std::function<void(std::string)> onRectLoadFailed;
    std::function<void()> onRectClicked;
    std::function<void()> onRectExpanded;
    std::function<void()> onRectCollapsed;
#endif
};

class RectAdsDelegate{
public:
    virtual void onRectLoaded(void){};
    virtual void onRectLoadFailed(std::string error){};
    virtual void onRectClicked(void){};
    virtual void onRectExpanded(void){};
    virtual void onRectCollapsed(void){};
};

#endif /* defined(__Ads_Framework__RectAds__) */
