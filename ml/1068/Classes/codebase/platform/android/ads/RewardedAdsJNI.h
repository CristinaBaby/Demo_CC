//
//  RewardedAdsJNI.h
//  Ads_Framework
//
//  Created by hujie2 on 15/7/13.
//
//

#ifndef __Ads_Framework__RewardedAdsJNI__
#define __Ads_Framework__RewardedAdsJNI__

#include <stdio.h>
#include <jni.h>
#include "RewardedAds.h"

class RewardedAdsJNI {
protected:
	RewardedAdsJNI();
	~RewardedAdsJNI();
protected:

	static RewardedAdsJNI* m_pInstance;

public:
	static RewardedAdsJNI* getInstance();

	 bool init(JNIEnv * pEnv, jobject pAdJava);
public:

	void preload();

	bool isPreloaded();

	bool show();

	void setAutoShow(bool);

	bool isAutoShow();

	void setDebugMode(bool);
public:
	void onRewardedLoaded();

	void onRewardedFailed(std::string errorCode);

	void onRewardedClicked();

	void onRewardedExpanded();

	void onRewarded(std::string, int, bool);

	void onRewardedCollapsed();

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

#endif /* defined(__Ads_Framework__RewardedAdsJNI__) */
