//
//  RewardedAdsJNI.cpp
//  Ads_Framework
//
//  Created by hujie2 on 15/7/13.
//
//

#include "RewardedAdsJNI.h"
#include "../JNIHelper.h"

RewardedAdsJNI::RewardedAdsJNI() :
		ClassAds(0), _adsJava(0), _methodPreload(0), _methodShow(0), _methodSetAutoShow(
				0), _methodIsAutoShow(0), _methodIsPreloaded(0), _methodSetDebugMode(
				0) {

}

RewardedAdsJNI::~RewardedAdsJNI() {

}

RewardedAdsJNI* RewardedAdsJNI::m_pInstance = nullptr;

RewardedAdsJNI* RewardedAdsJNI::getInstance() {
	if (!m_pInstance) {
		m_pInstance = new RewardedAdsJNI();
	}
	return m_pInstance;
}

bool RewardedAdsJNI::init(JNIEnv *pEnv, jobject pAdJava) {
	JNIHelper::getInstance()->init(pEnv);
	ClassAds = pEnv->GetObjectClass(pAdJava);
	if (!ClassAds) {
		LOGE("initial JNI RewardedAds class Failed!");
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

void RewardedAdsJNI::preload() {

	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodPreload);
}

bool RewardedAdsJNI::show() {
	if (!_adsJava) {
		return false;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jboolean ret = lEnv->CallBooleanMethod(_adsJava, _methodShow);
	return (ret == JNI_TRUE ? true : false);
}

void RewardedAdsJNI::setAutoShow(bool isAuto) {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodSetAutoShow, isAuto);
}

bool RewardedAdsJNI::isAutoShow() {
	if (!_adsJava) {
		return false;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jboolean ret = lEnv->CallBooleanMethod(_adsJava, _methodIsAutoShow);
	return (ret == JNI_TRUE ? true : false);
}

bool RewardedAdsJNI::isPreloaded() {
	if (!_adsJava) {
		return false;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jboolean ret = lEnv->CallBooleanMethod(_adsJava, _methodIsPreloaded);
	return (ret == JNI_TRUE ? true : false);
}

void RewardedAdsJNI::setDebugMode(bool bIsDebug) {
	if (!_adsJava) {
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_adsJava, _methodSetDebugMode, bIsDebug);
}

void RewardedAdsJNI::onRewardedLoaded() {
#if __cplusplus > 201100L
	if (RewardedAds::getInstance()->onRewardedLoaded) {
		RewardedAds::getInstance()->onRewardedLoaded();
	}
#endif
	if (RewardedAds::getInstance()->delegate) {
		RewardedAds::getInstance()->delegate->onRewardedLoaded();
	}

}

void RewardedAdsJNI::onRewardedFailed(std::string errorCode) {
#if __cplusplus > 201100L
	if (RewardedAds::getInstance()->onRewardedFailed) {
		RewardedAds::getInstance()->onRewardedFailed(errorCode);
	}
#endif
	if (RewardedAds::getInstance()->delegate) {
		RewardedAds::getInstance()->delegate->onRewardedFailed(errorCode);
	}

}

void RewardedAdsJNI::onRewardedClicked() {
#if __cplusplus > 201100L
	if (RewardedAds::getInstance()->onRewardedClicked) {
		RewardedAds::getInstance()->onRewardedClicked();
	}
#endif
	if (RewardedAds::getInstance()->delegate) {
		RewardedAds::getInstance()->delegate->onRewardedClicked();
	}

}

void RewardedAdsJNI::onRewardedExpanded() {
#if __cplusplus > 201100L
	if (RewardedAds::getInstance()->onRewardedExpanded) {
		RewardedAds::getInstance()->onRewardedExpanded();
	}
#endif
	if (RewardedAds::getInstance()->delegate) {
		RewardedAds::getInstance()->delegate->onRewardedExpanded();
	}

}

void RewardedAdsJNI::onRewardedCollapsed() {
#if __cplusplus > 201100L
	if (RewardedAds::getInstance()->onRewardedCollapsed) {
		RewardedAds::getInstance()->onRewardedCollapsed();
	}
#endif
	if (RewardedAds::getInstance()->delegate) {
		RewardedAds::getInstance()->delegate->onRewardedCollapsed();
	}

}

void RewardedAdsJNI::onRewarded(std::string unitId, int amount, bool bSkipped) {
#if __cplusplus > 201100L
	if (RewardedAds::getInstance()->onRewarded) {
	RewardedAds::getInstance()->onRewarded(unitId, amount,
					bSkipped);
		}
#endif
		if (RewardedAds::getInstance()->delegate) {
			RewardedAds::getInstance()->delegate->onRewarded(unitId, amount,
					bSkipped);
		}

	}
