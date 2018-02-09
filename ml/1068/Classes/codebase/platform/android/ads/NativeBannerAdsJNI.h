//
//  NativeBannerAdsJNI.h
//
//  Created by Jimmy on 15/7/9.
//
//

#ifndef __Ads_Framework__NativeBannerAdsJNI__
#define __Ads_Framework__NativeBannerAdsJNI__

#include <stdio.h>
#include <jni.h>
#include "SSCNativeAds.h"

class NativeBannerAdsJNI
{
protected:
    
    NativeBannerAdsJNI();
    
    ~NativeBannerAdsJNI();
    
public:
    
    static NativeBannerAdsJNI* getInstance();
    
    bool init(JNIEnv * pEnv, jobject pAdJava);
    
protected:
    
    static NativeBannerAdsJNI *m_pInstance;
    
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
    
    void setLayout(NativeAdsLayoutType kType);

public:
    
    void onNativeLoaded();
    
    void onNativeLoadFailed(std::string errorCode);
    
    void onNativeClicked();
    
    void onNativeExpanded();
    
    void onNativeCollapsed();
    
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

#endif /* defined(__Ads_Framework__NativeBannerAdsJNI__) */
