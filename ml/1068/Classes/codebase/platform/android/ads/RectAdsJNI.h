//
//  RectAdsJNI.h
//
//  Created by hujie2 on 15/7/9.
//
//

#ifndef __Ads_Framework__BanerAdsJNI__
#define __Ads_Framework__BanerAdsJNI__

#include <stdio.h>
#include <jni.h>
#include "RectAds.h"

class RectAdsJNI
{
protected:

    RectAdsJNI();

    ~RectAdsJNI();

public:

    static RectAdsJNI* getInstance();

    bool init(JNIEnv * pEnv, jobject pAdJava);

protected:

    static RectAdsJNI *m_pInstance;

public:

    void preload();

    void show();

    void setAutoShow(bool bIsAuto);

    bool isAutoShow();


    void setVisible(bool bVisibility);

    /**
     *@brief remove Rect ads
     **/
    void remove();

public:

    void onRectLoaded();

    void onRectLoadFailed(std::string errorCode);

    void onRectClicked();

    void onRectExpanded();

    void onRectCollapsed();

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
};

#endif /* defined(__Ads_Framework__RectAdsJNI__) */
