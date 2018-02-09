//
//  BanerAdsJNI.cpp
//
//  Created by Jimmy.Hu on 15/7/9.
//
//

#include "BannerAdsJNI.h"
#include "../JNIHelper.h"

BannerAdsJNI::BannerAdsJNI() :
		ClassAds(0), _adsJava(0), _methodPreload(0), _methodShow(0), _methodSetAutoShow(
				0), _methodIsAutoShow(0), _methodSetVisible(0), _methodRemove(
				0), _methodSetDebugMode(0), _methodSetLayout(0) {

}

BannerAdsJNI* BannerAdsJNI::m_pInstance = NULL;

BannerAdsJNI::~BannerAdsJNI() {

}

BannerAdsJNI* BannerAdsJNI::getInstance() {
	if (!m_pInstance) {
		m_pInstance = new BannerAdsJNI();
	}
	return m_pInstance;
}

bool BannerAdsJNI::init(JNIEnv *pEnv, jobject pAdJava) {
	JNIHelper::getInstance()->init(pEnv);
	ClassAds = pEnv->GetObjectClass(pAdJava);
	if (!ClassAds) {
		LOGE("initial JNI BannerAds class Failed!");
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

	_methodSetLayout = pEnv->GetMethodID(ClassAds, "setLayout", "(I)V");
	if (!_methodSetLayout) {
		LOGE("JNI get Java method setLayout Failed!");
		return false;
	}
	return true;
}

void BannerAdsJNI::preload() {

	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodPreload);
}

void BannerAdsJNI::show() {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodShow);
}

void BannerAdsJNI::setAutoShow(bool isAuto) {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodSetAutoShow, isAuto);
}

bool BannerAdsJNI::isAutoShow() {
	if (!_adsJava) {
		return false;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jboolean ret = lEnv->CallBooleanMethod(_adsJava, _methodIsAutoShow);
	return (ret == JNI_TRUE ? true : false);
}

void BannerAdsJNI::setVisible(bool visibility) {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodSetVisible, visibility);
}

void BannerAdsJNI::remove() {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodRemove);
}

void BannerAdsJNI::setDebugMode(bool bIsDebug) {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodSetDebugMode, bIsDebug);
}

void BannerAdsJNI::setLayout(LAYOUT_TYPE kType) {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	int iType = (int)kType;
	lEnv->CallVoidMethod(_adsJava, _methodSetLayout, iType);
}

void BannerAdsJNI::onBannerLoaded() {
#if __cplusplus > 201100L
	if (BannerAds::getInstance()->onBannerLoaded) {
		BannerAds::getInstance()->onBannerLoaded();
	}
#endif
	if (BannerAds::getInstance()->delegate) {
		BannerAds::getInstance()->delegate->onBannerLoaded();
	}

}

void BannerAdsJNI::onBannerLoadFailed(std::string errorCode) {
#if __cplusplus > 201100L
	if (BannerAds::getInstance()->onBannerLoadFailed) {
		BannerAds::getInstance()->onBannerLoadFailed(errorCode);
	}
#endif
	if (BannerAds::getInstance()->delegate) {
		BannerAds::getInstance()->delegate->onBannerLoadFailed(errorCode);
	}

}

void BannerAdsJNI::onBannerClicked() {
#if __cplusplus > 201100L
	if (BannerAds::getInstance()->onBannerClicked) {
		BannerAds::getInstance()->onBannerClicked();
	}
#endif
	if (BannerAds::getInstance()->delegate) {
		BannerAds::getInstance()->delegate->onBannerClicked();
	}

}

void BannerAdsJNI::onBannerExpanded() {
#if __cplusplus > 201100L
	if (BannerAds::getInstance()->onBannerExpanded) {
		BannerAds::getInstance()->onBannerExpanded();
	}
#endif
	if (BannerAds::getInstance()->delegate) {
		BannerAds::getInstance()->delegate->onBannerExpanded();
	}

}

void BannerAdsJNI::onBannerCollapsed() {
#if __cplusplus > 201100L
	if (BannerAds::getInstance()->onBannerCollapsed) {
		BannerAds::getInstance()->onBannerCollapsed();
	}
#endif
	if (BannerAds::getInstance()->delegate) {
		BannerAds::getInstance()->delegate->onBannerCollapsed();
	}

}
