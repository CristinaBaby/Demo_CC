//
//  CrosspromoAdsJNI.cpp
//  Ads_Framework
//
//  Created by hujie2 on 15/7/13.
//
//

#include "CrosspromoAdsJNI.h"
#include "../JNIHelper.h"

CrosspromoAdsJNI::CrosspromoAdsJNI() :
		ClassAds(0), _adsJava(0), _methodPreload(0), _methodShow(0), _methodSetAutoShow(
				0), _methodIsAutoShow(0), _methodIsPreloaded(0), _methodSetDebugMode(
				0) {

}

CrosspromoAdsJNI::~CrosspromoAdsJNI() {

}

CrosspromoAdsJNI* CrosspromoAdsJNI::m_pInstance = nullptr;

CrosspromoAdsJNI* CrosspromoAdsJNI::getInstance() {
	if (!m_pInstance) {
		m_pInstance = new CrosspromoAdsJNI();
	}
	return m_pInstance;
}

bool CrosspromoAdsJNI::init(JNIEnv *pEnv, jobject pAdJava) {
	JNIHelper::getInstance()->init(pEnv);
	ClassAds = pEnv->GetObjectClass(pAdJava);
	if (!ClassAds) {
		LOGE("initial JNI CrosspromoAds class Failed!");
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

void CrosspromoAdsJNI::preload() {

	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodPreload);
}

bool CrosspromoAdsJNI::show() {
	if (!_adsJava) {
		return false;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jboolean ret = lEnv->CallBooleanMethod(_adsJava, _methodShow);
	return (ret == JNI_TRUE ? true : false);
}

void CrosspromoAdsJNI::setAutoShow(bool isAuto) {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodSetAutoShow, isAuto);
}

bool CrosspromoAdsJNI::isAutoShow() {
	if (!_adsJava) {
		return false;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jboolean ret = lEnv->CallBooleanMethod(_adsJava, _methodIsAutoShow);
	return (ret == JNI_TRUE ? true : false);
}

void CrosspromoAdsJNI::setDebugMode(bool bIsDebug) {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodSetDebugMode, bIsDebug);
}

bool CrosspromoAdsJNI::isPreloaded() {
	if (!_adsJava) {
		return false;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jboolean ret = lEnv->CallBooleanMethod(_adsJava, _methodIsPreloaded);
	return (ret == JNI_TRUE ? true : false);
}

void CrosspromoAdsJNI::onCrosspromoLoaded() {
#if __cplusplus > 201100L
	if (CrosspromoAds::getInstance()->onCrosspromoLoaded) {
		CrosspromoAds::getInstance()->onCrosspromoLoaded();
	}
#endif
	if (CrosspromoAds::getInstance()->delegate) {
		CrosspromoAds::getInstance()->delegate->onCrosspromoLoaded();
	}

}

void CrosspromoAdsJNI::onCrosspromoFailed(std::string errorCode) {
#if __cplusplus > 201100L
	if (CrosspromoAds::getInstance()->onCrosspromoFailed) {
		CrosspromoAds::getInstance()->onCrosspromoFailed(errorCode);
	}
#endif
	if (CrosspromoAds::getInstance()->delegate) {
		CrosspromoAds::getInstance()->delegate->onCrosspromoFailed(errorCode);
	}

}

void CrosspromoAdsJNI::onCrosspromoClicked() {
#if __cplusplus > 201100L
	if (CrosspromoAds::getInstance()->onCrosspromoClicked) {
		CrosspromoAds::getInstance()->onCrosspromoClicked();
	}
#endif
	if (CrosspromoAds::getInstance()->delegate) {
		CrosspromoAds::getInstance()->delegate->onCrosspromoClicked();
	}

}

void CrosspromoAdsJNI::onCrosspromoExpanded() {
#if __cplusplus > 201100L
	if (CrosspromoAds::getInstance()->onCrosspromoExpanded) {
		CrosspromoAds::getInstance()->onCrosspromoExpanded();
	}
#endif
	if (CrosspromoAds::getInstance()->delegate) {
		CrosspromoAds::getInstance()->delegate->onCrosspromoExpanded();
	}

}

void CrosspromoAdsJNI::onCrosspromoCollapsed() {
#if __cplusplus > 201100L
	if (CrosspromoAds::getInstance()->onCrosspromoCollapsed) {
		CrosspromoAds::getInstance()->onCrosspromoCollapsed();
	}
#endif
	if (CrosspromoAds::getInstance()->delegate) {
		CrosspromoAds::getInstance()->delegate->onCrosspromoCollapsed();
	}

}
