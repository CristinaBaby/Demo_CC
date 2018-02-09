//
//  InterstitialAdsJNI.h
//  Ads_Framework
//
//  Created by hujie2 on 15/7/13.
//
//

#ifndef __Ads_Framework__InterstitialAdsJNI__
#define __Ads_Framework__InterstitialAdsJNI__

#include <stdio.h>
#include <jni.h>
#include "InterstitialAds.h"

class InterstitialAdsJNI {
protected:
	InterstitialAdsJNI();
	~InterstitialAdsJNI();
protected:

	static InterstitialAdsJNI* m_pInstance;

public:
	static InterstitialAdsJNI* getInstance();

	bool init(JNIEnv * pEnv, jobject pAdJava);

public:

	void preload();

	bool isPreloaded();

	bool show();

	void setAutoShow(bool);

	bool isAutoShow();

	void setDebugMode(bool);

public:
	void onInterstitialLoaded();

	void onInterstitialFailed(std::string errorCode);

	void onInterstitialClicked();

	void onInterstitialExpanded();

	void onInterstitialCollapsed();

private:

	// Cached Classes.
	jclass ClassAds;

	// Cached java object
	jobject _adsJava;

	// Cached active Methods.
	jmethodID _methodPreload;

	jmethodID _methodIsPreloaded;

	jmethodID _methodShow;

	jmethodID _methodSetAutoShow;

	jmethodID _methodIsAutoShow;

	jmethodID _methodSetDebugMode;

};

#endif /* defined(__Ads_Framework__InterstitialAdsJNI__) */
