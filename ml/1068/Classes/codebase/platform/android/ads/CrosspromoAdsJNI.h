//
//  CrosspromoAdsJNI.h
//  Ads_Framework
//
//  Created by hujie2 on 15/7/13.
//
//

#ifndef __Ads_Framework__CrosspromoAdsJNI__
#define __Ads_Framework__CrosspromoAdsJNI__

#include <stdio.h>
#include <jni.h>
#include "CrosspromoAds.h"

class CrosspromoAdsJNI {
protected:
	CrosspromoAdsJNI();
	~CrosspromoAdsJNI();
protected:

	static CrosspromoAdsJNI* m_pInstance;

public:
	static CrosspromoAdsJNI* getInstance();

	bool init(JNIEnv * pEnv, jobject pAdJava);

public:

	void preload();

	bool isPreloaded();

	bool show();

	void setAutoShow(bool);

	bool isAutoShow();

	void setDebugMode(bool);

public:
	void onCrosspromoLoaded();

	void onCrosspromoFailed(std::string errorCode);

	void onCrosspromoClicked();

	void onCrosspromoExpanded();

	void onCrosspromoCollapsed();

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

#endif /* defined(__Ads_Framework__CrosspromoAdsJNI__) */
