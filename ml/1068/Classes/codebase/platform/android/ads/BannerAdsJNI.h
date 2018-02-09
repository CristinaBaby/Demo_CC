//
//  BanerAdsJNI.h
//
//  Created by hujie2 on 15/7/9.
//
//

#ifndef __Ads_Framework__BanerAdsJNI__
#define __Ads_Framework__BanerAdsJNI__

#include <stdio.h>
#include <jni.h>
#include "BannerAds.h"

class BannerAdsJNI
{
protected:
    
    BannerAdsJNI();
    
    ~BannerAdsJNI();
    
public:
    
    static BannerAdsJNI* getInstance();
    
    bool init(JNIEnv * pEnv, jobject pAdJava);
    
protected:
    
    static BannerAdsJNI *m_pInstance;
    
public:
    
    void preload();
    
    void show();
    
    void setAutoShow(bool bIsAuto);
    
    bool isAutoShow();
    
    
    void setVisible(bool bVisibility);
    
    /**
     *@brief remove banner ads
     **/
    void remove();
    
    void setLayout(LAYOUT_TYPE kType);

public:
    
    void onBannerLoaded();
    
    void onBannerLoadFailed(std::string errorCode);
    
    void onBannerClicked();
    
    void onBannerExpanded();
    
    void onBannerCollapsed();
    
    void setDebugMode(bool);

private:
    
    // Cached Classes.
    jclass ClassAds;
    
    // Cached java object
    jobject _adsJava;
    
    // Cached active Methods.
    jmethodID _methodPreload;
    
    jmethodID _methodShow;
    
    jmethodID _methodSetAutoShow;
    
    jmethodID _methodIsAutoShow;
    
    jmethodID _methodSetVisible;
    
    jmethodID _methodRemove;

    jmethodID _methodSetDebugMode;

    jmethodID _methodSetLayout;
};

#endif /* defined(__Ads_Framework__BanerAdsJNI__) */
