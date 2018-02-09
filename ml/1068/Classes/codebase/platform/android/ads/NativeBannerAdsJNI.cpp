//
//  BanerAdsJNI.cpp
//
//  Created by Jimmy.Hu on 15/7/9.
//
//

#include "NativeBannerAdsJNI.h"
#include "../JNIHelper.h"

NativeBannerAdsJNI::NativeBannerAdsJNI() :
		ClassAds(0), _adsJava(0), _methodPreload(0), _methodShow(0), _methodSetAutoShow(
				0), _methodIsAutoShow(0), _methodSetVisible(0), _methodRemove(
				0), _methodSetDebugMode(0), _methodSetLayout(0) {

}

NativeBannerAdsJNI* NativeBannerAdsJNI::m_pInstance = NULL;

NativeBannerAdsJNI::~NativeBannerAdsJNI() {

}

NativeBannerAdsJNI* NativeBannerAdsJNI::getInstance() {
	if (!m_pInstance) {
		m_pInstance = new NativeBannerAdsJNI();
	}
	return m_pInstance;
}

bool NativeBannerAdsJNI::init(JNIEnv *pEnv, jobject pAdJava) {
	JNIHelper::getInstance()->init(pEnv);
    LOGE("initial JNI NativeBannerAds init init !111111111111");
	ClassAds = pEnv->GetObjectClass(pAdJava);
	if (!ClassAds) {
		LOGE("initial JNI NativeBannerAds class Failed!");
		return false;
	}
	ClassAds = (jclass) JNIHelper::makeGlobalRef(pEnv, ClassAds);
LOGE("initial JNI NativeBannerAds init init !22222");
	_adsJava = JNIHelper::makeGlobalRef(pEnv, pAdJava);
	if (!_adsJava) {
		LOGE("Cache JNI jobject Java Failed!");
		return false;
	}
LOGE("initial JNI NativeBannerAds init init !4444");
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

	_methodShow = pEnv->GetMethodID(ClassAds, "show", "()V");
	if (!_methodPreload) {
		LOGE("JNI get Java method show Failed!");
		return false;
	}

	_methodIsAutoShow = pEnv->GetMethodID(ClassAds, "isAutoShow", "()Z");
	if (!_methodPreload) {
		LOGE("JNI get Java method isAutoShow Failed!");
		return false;
	}

	_methodSetVisible = pEnv->GetMethodID(ClassAds, "setVisible", "(Z)V");
	if (!_methodPreload) {
		LOGE("JNI get Java method setVisible Failed!");
		return false;
	}

	_methodRemove = pEnv->GetMethodID(ClassAds, "remove", "()V");
	if (!_methodPreload) {
		LOGE("JNI get Java method remove Failed!");
		return false;
	}

	_methodSetDebugMode = pEnv->GetMethodID(ClassAds, "setDebugMode", "(Z)V");
	if (!_methodSetDebugMode) {
		LOGE("JNI get Java method setDebugMode Failed!");
		return false;
	}

	_methodSetLayout = pEnv->GetMethodID(ClassAds, "setLayout", "(I)V");
	if (!_methodSetLayout) {
		LOGE("JNI get Java method setLayout Failed!");
		return false;
	}
	return true;
}

void NativeBannerAdsJNI::preload() {
    LOGE("JNI get Java NativeBannerAdsJNI method preload 11!");
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodPreload);
    LOGE("JNI get Java  NativeBannerAdsJNI method preload 22!");
}

void NativeBannerAdsJNI::show() {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodShow);
}

void NativeBannerAdsJNI::setAutoShow(bool isAuto) {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodSetAutoShow, isAuto);
}

bool NativeBannerAdsJNI::isAutoShow() {
	if (!_adsJava) {
		return false;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jboolean ret = lEnv->CallBooleanMethod(_adsJava, _methodIsAutoShow);
	return (ret == JNI_TRUE ? true : false);
}

void NativeBannerAdsJNI::setVisible(bool visibility) {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodSetVisible, visibility);
}

void NativeBannerAdsJNI::remove() {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodRemove);
}

void NativeBannerAdsJNI::setDebugMode(bool bIsDebug) {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodSetDebugMode, bIsDebug);
}

void NativeBannerAdsJNI::setLayout(NativeAdsLayoutType kType) {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	int iType = (int)kType;
	lEnv->CallVoidMethod(_adsJava, _methodSetLayout, iType);
}

void NativeBannerAdsJNI::onNativeLoaded() {
#if __cplusplus > 201100L
	if (SSCNativeAds::getInstance()->onNativeAdsLoaded) {
		SSCNativeAds::getInstance()->onNativeAdsLoaded();
	}
#endif
	if (SSCNativeAds::getInstance()->delegate) {
		SSCNativeAds::getInstance()->delegate->onNativeAdsLoaded();
	}
}

void NativeBannerAdsJNI::onNativeLoadFailed(std::string errorCode) {
#if __cplusplus > 201100L
	if (SSCNativeAds::getInstance()->onNativeAdsLoadFailed) {
		SSCNativeAds::getInstance()->onNativeAdsLoadFailed(errorCode);
	}
#endif
	if (SSCNativeAds::getInstance()->delegate) {
		SSCNativeAds::getInstance()->delegate->onNativeAdsLoadFailed(errorCode);
	}
}

void NativeBannerAdsJNI::onNativeClicked() {
#if __cplusplus > 201100L
	if (SSCNativeAds::getInstance()->onNativeAdsClicked) {
		SSCNativeAds::getInstance()->onNativeAdsClicked();
	}
#endif
	if (SSCNativeAds::getInstance()->delegate) {
		SSCNativeAds::getInstance()->delegate->onNativeAdsClicked();
	}
}

void NativeBannerAdsJNI::onNativeExpanded() {
#if __cplusplus > 201100L
	if (SSCNativeAds::getInstance()->onNativeAdsExpanded) {
		SSCNativeAds::getInstance()->onNativeAdsExpanded();
	}
#endif
	if (SSCNativeAds::getInstance()->delegate) {
		SSCNativeAds::getInstance()->delegate->onNativeAdsExpanded();
	}
}

void NativeBannerAdsJNI::onNativeCollapsed() {
#if __cplusplus > 201100L
	if (SSCNativeAds::getInstance()->onNativeAdsCollapsed) {
		SSCNativeAds::getInstance()->onNativeAdsCollapsed();
	}
#endif
	if (SSCNativeAds::getInstance()->delegate) {
		SSCNativeAds::getInstance()->delegate->onNativeAdsCollapsed();
	}

}
