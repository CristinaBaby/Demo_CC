
/*
 * com_common_android_ads_jni_RewardedAdsJni.cpp
 *
 *  Created on: 2015-07-09
 *      Author: Jimmy.Hu
 */
#include "com_common_android_ads_jni_RewardedAdsJni.h"
#include "RewardedAdsJNI.h"


JNIEXPORT void JNICALL Java_com_common_android_ads_jni_RewardedAdsJni_nativeInit
  (JNIEnv *pEnv, jobject thisObj)
{
	RewardedAdsJNI::getInstance()->init(pEnv, thisObj);
}

JNIEXPORT void JNICALL Java_com_common_android_ads_jni_RewardedAdsJni_nativeOnRewardedLoaded
  (JNIEnv *pEnv, jobject thisObj)
{
	RewardedAdsJNI::getInstance()->onRewardedLoaded();
}

/*
 * Class:     com_common_android_ads_jni_RewardedAdsJni
 * Method:    nativeOnRewardedFailed
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_RewardedAdsJni_nativeOnRewardedFailed
  (JNIEnv *pEnv, jobject thisObj, jstring errorCode)
{
	const char *str = pEnv->GetStringUTFChars(errorCode, 0);
	RewardedAdsJNI::getInstance()->onRewardedFailed(str);
	pEnv->ReleaseStringUTFChars(errorCode, str);
}

/*
 * Class:     com_common_android_ads_jni_RewardedAdsJni
 * Method:    nativeOnRewardedClicked
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_RewardedAdsJni_nativeOnRewardedClicked
  (JNIEnv *pEnv, jobject thisObj)
{
	RewardedAdsJNI::getInstance()->onRewardedClicked();
}

/*
 * Class:     com_common_android_ads_jni_RewardedAdsJni
 * Method:    nativeOnRewardedCollapsed
 * Signature: (Ljava/lang/String;IZ)V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_RewardedAdsJni_nativeOnRewardedCollapsed
  (JNIEnv *pEnv, jobject thisObj)
{
	RewardedAdsJNI::getInstance()->onRewardedCollapsed();
}

JNIEXPORT void JNICALL Java_com_common_android_ads_jni_RewardedAdsJni_nativeOnRewarded
  (JNIEnv *pEnv, jobject thisObj, jstring unitId, jint amount, jboolean bSkipped)
{

	const char *str = pEnv->GetStringUTFChars(unitId, 0);
	RewardedAdsJNI::getInstance()->onRewarded(str, amount, bSkipped);
	pEnv->ReleaseStringUTFChars(unitId, str);

}

/*
 * Class:     com_common_android_ads_jni_RewardedAdsJni
 * Method:    nativeOnRewardedExpanded
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_RewardedAdsJni_nativeOnRewardedExpanded
  (JNIEnv *, jobject)
{
	RewardedAdsJNI::getInstance()->onRewardedExpanded();
}
