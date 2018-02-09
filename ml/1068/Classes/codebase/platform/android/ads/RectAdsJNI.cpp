//
//  RectAdsJNI.cpp
//
//  Created by Jimmy.Hu on 15/7/9.
//
//

#include "RectAdsJNI.h"
#include "../JNIHelper.h"

RectAdsJNI::RectAdsJNI() :
		ClassAds(0), _adsJava(0), _methodPreload(0), _methodShow(0), _methodSetAutoShow(
				0), _methodIsAutoShow(0), _methodSetVisible(0), _methodRemove(0),_methodSetDebugMode(0) {

}

RectAdsJNI* RectAdsJNI::m_pInstance = nullptr;

RectAdsJNI::~RectAdsJNI() {

}

RectAdsJNI* RectAdsJNI::getInstance() {
	if (!m_pInstance) {
		m_pInstance = new RectAdsJNI();
	}
	return m_pInstance;
}

bool RectAdsJNI::init(JNIEnv *pEnv, jobject pAdJava) {
	JNIHelper::getInstance()->init(pEnv);
	ClassAds = pEnv->GetObjectClass(pAdJava);
	if (!ClassAds) {
		LOGE("initial JNI RectAds class Failed!");
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

	return true;
}

void RectAdsJNI::preload() {

	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodPreload);
}

void RectAdsJNI::show() {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodShow);
}

void RectAdsJNI::setAutoShow(bool isAuto) {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodSetAutoShow, isAuto);
}

bool RectAdsJNI::isAutoShow() {
	if (!_adsJava) {
		return false;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jboolean ret = lEnv->CallBooleanMethod(_adsJava, _methodIsAutoShow);
	return (ret == JNI_TRUE ? true : false);
}

void RectAdsJNI::setVisible(bool visibility) {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodSetVisible, visibility);
}

void RectAdsJNI::setDebugMode(bool bIsDebug) {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodSetDebugMode, bIsDebug);
}

void RectAdsJNI::remove() {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodRemove);
}

void RectAdsJNI::onRectLoaded() {
#if __cplusplus > 201100L
	if (RectAds::getInstance()->onRectLoaded) {
		RectAds::getInstance()->onRectLoaded();
	}
#endif
	if (RectAds::getInstance()->delegate) {
		RectAds::getInstance()->delegate->onRectLoaded();
	}

}

void RectAdsJNI::onRectLoadFailed(std::string errorCode) {
#if __cplusplus > 201100L
	if (RectAds::getInstance()->onRectLoadFailed) {
		RectAds::getInstance()->onRectLoadFailed(errorCode);
	}
#endif
	if (RectAds::getInstance()->delegate) {
		RectAds::getInstance()->delegate->onRectLoadFailed(errorCode);
	}

}

void RectAdsJNI::onRectClicked() {
#if __cplusplus > 201100L
	if (RectAds::getInstance()->onRectClicked) {
		RectAds::getInstance()->onRectClicked();
	}
#endif
	if (RectAds::getInstance()->delegate) {
		RectAds::getInstance()->delegate->onRectClicked();
	}

}

void RectAdsJNI::onRectExpanded() {
#if __cplusplus > 201100L
	if (RectAds::getInstance()->onRectExpanded) {
		RectAds::getInstance()->onRectExpanded();
	}
#endif
	if (RectAds::getInstance()->delegate) {
		RectAds::getInstance()->delegate->onRectExpanded();
	}

}

void RectAdsJNI::onRectCollapsed() {
#if __cplusplus > 201100L
	if (RectAds::getInstance()->onRectCollapsed) {
		RectAds::getInstance()->onRectCollapsed();
	}
#endif
	if (RectAds::getInstance()->delegate) {
		RectAds::getInstance()->delegate->onRectCollapsed();
	}

}
