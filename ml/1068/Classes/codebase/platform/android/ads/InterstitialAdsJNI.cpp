//
//  InterstitialAdsJNI.cpp
//  Ads_Framework
//
//  Created by hujie2 on 15/7/13.
//
//

#include "InterstitialAdsJNI.h"
#include "../JNIHelper.h"

InterstitialAdsJNI::InterstitialAdsJNI() :
		ClassAds(0), _adsJava(0), _methodPreload(0), _methodShow(0), _methodSetAutoShow(
				0), _methodIsAutoShow(0), _methodIsPreloaded(0),_methodSetDebugMode(0) {

}

InterstitialAdsJNI::~InterstitialAdsJNI() {

}

InterstitialAdsJNI* InterstitialAdsJNI::m_pInstance = nullptr;

InterstitialAdsJNI* InterstitialAdsJNI::getInstance() {
	if (!m_pInstance) {
		m_pInstance = new InterstitialAdsJNI();
	}
	return m_pInstance;
}

bool InterstitialAdsJNI::init(JNIEnv *pEnv, jobject pAdJava) {
	JNIHelper::getInstance()->init(pEnv);
	ClassAds = pEnv->GetObjectClass(pAdJava);
	if (!ClassAds) {
		LOGE("initial JNI InterstitialAds class Failed!");
		return false;
	}
	ClassAds = (jclass) JNIHelper::makeGlobalRef(pEnv, ClassAds);

	_adsJava = JNIHelper::makeGlobalRef(pEnv, pAdJava);
	if (!_adsJava) {
		LOGE("Cache JNI jobject Java Failed!");
		return false;
	}

	_methodPreload = pEnv->GetMethodID(ClassAds, "preload", "()V");

	if (!_methodPreload) {
		LOGE("JNI get Java method preload Failed!");
		return false;
	}

	_methodSetAutoShow = pEnv->GetMethodID(ClassAds, "setAutoShow", "(Z)V");
	if (!_methodPreload) {
		LOGE("JNI get Java method setAutoShow Failed!");
		return false;
	}

	_methodShow = pEnv->GetMethodID(ClassAds, "show", "()Z");
	if (!_methodPreload) {
		LOGE("JNI get Java method show Failed!");
		return false;
	}

	_methodIsAutoShow = pEnv->GetMethodID(ClassAds, "isAutoShow", "()Z");
	if (!_methodPreload) {
		LOGE("JNI get Java method isAutoShow Failed!");
		return false;
	}

	_methodIsPreloaded = pEnv->GetMethodID(ClassAds, "isPreloaded", "()Z");
	if (!_methodPreload) {
		LOGE("JNI get Java method isAutoShow Failed!");
		return false;
	}

	_methodSetDebugMode = pEnv->GetMethodID(ClassAds, "setDebugMode", "(Z)V");
	if (!_methodSetDebugMode) {
		LOGE("JNI get Java method setDebugMode Failed!");
		return false;
	}

	return true;
}

void InterstitialAdsJNI::preload() {
//	LOGE("----------InterstitialAdsJNI preload--------");
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodPreload);
}

bool InterstitialAdsJNI::show() {
	if (!_adsJava) {
		return false;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jboolean ret = lEnv->CallBooleanMethod(_adsJava, _methodShow);
	return (ret == JNI_TRUE ? true : false);
}

void InterstitialAdsJNI::setAutoShow(bool isAuto) {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodSetAutoShow, isAuto);
}

bool InterstitialAdsJNI::isAutoShow() {
	if (!_adsJava) {
		return false;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jboolean ret = lEnv->CallBooleanMethod(_adsJava, _methodIsAutoShow);
	return (ret == JNI_TRUE ? true : false);
}

bool InterstitialAdsJNI::isPreloaded() {
	if (!_adsJava) {
		return false;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jboolean ret = lEnv->CallBooleanMethod(_adsJava, _methodIsPreloaded);
	return (ret == JNI_TRUE ? true : false);
}

void InterstitialAdsJNI::setDebugMode(bool bIsDebug) {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodSetDebugMode, bIsDebug);
}

void InterstitialAdsJNI::onInterstitialLoaded() {
#if __cplusplus > 201100L
	if (InterstitialAds::getInstance()->onInterstitialLoaded) {
		InterstitialAds::getInstance()->onInterstitialLoaded();
	}
#endif
	if (InterstitialAds::getInstance()->delegate) {
		InterstitialAds::getInstance()->delegate->onInterstitialLoaded();
	}

}

void InterstitialAdsJNI::onInterstitialFailed(std::string errorCode) {
#if __cplusplus > 201100L
	if (InterstitialAds::getInstance()->onInterstitialFailed) {
		InterstitialAds::getInstance()->onInterstitialFailed(errorCode);
	}
#endif
	if (InterstitialAds::getInstance()->delegate) {
		InterstitialAds::getInstance()->delegate->onInterstitialFailed(errorCode);
	}

}

void InterstitialAdsJNI::onInterstitialClicked() {
#if __cplusplus > 201100L
	if (InterstitialAds::getInstance()->onInterstitialClicked) {
		InterstitialAds::getInstance()->onInterstitialClicked();
	}
#endif
	if (InterstitialAds::getInstance()->delegate) {
		InterstitialAds::getInstance()->delegate->onInterstitialClicked();
	}

}

void InterstitialAdsJNI::onInterstitialExpanded() {
#if __cplusplus > 201100L
	if (InterstitialAds::getInstance()->onInterstitialExpanded) {
		InterstitialAds::getInstance()->onInterstitialExpanded();
	}
#endif
	if (InterstitialAds::getInstance()->delegate) {
		InterstitialAds::getInstance()->delegate->onInterstitialExpanded();
	}

}

void InterstitialAdsJNI::onInterstitialCollapsed() {
#if __cplusplus > 201100L
	if (InterstitialAds::getInstance()->onInterstitialCollapsed) {
		InterstitialAds::getInstance()->onInterstitialCollapsed();
	}
#endif
	if (InterstitialAds::getInstance()->delegate) {
		InterstitialAds::getInstance()->delegate->onInterstitialCollapsed();
	}

}
