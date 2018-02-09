
/*
 * com_common_android_ads_jni_NativeBannerAdsJNI.cpp
 *
 *  Created on: 2015-07-09
 *      Author: Jimmy.Hu
 */
#include "com_common_android_ads_jni_NativeBannerAdsJNI.h"
#include "NativeBannerAdsJNI.h"


JNIEXPORT void JNICALL Java_com_common_android_ads_jni_NativeBannerAdsJni_nativeInit
  (JNIEnv *pEnv, jobject thisObj)
{
	NativeBannerAdsJNI::getInstance()->init(pEnv, thisObj);
}

JNIEXPORT void JNICALL Java_com_common_android_ads_jni_NativeBannerAdsJni_nativeOnNativeAdsLoaded
  (JNIEnv *pEnv, jobject thisObj)
{
	NativeBannerAdsJNI::getInstance()->onNativeLoaded();
}

/*
 * Class:     com_common_android_ads_jni_NativeBannerAdsJNI
 * Method:    nativeOnNativeAdsFailed
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_NativeBannerAdsJni_nativeOnNativeAdsFailed
  (JNIEnv *pEnv, jobject thisObj, jstring errorCode)
{
//	LOGE("============OnNativeAdsFailed=============");
	const char *str = pEnv->GetStringUTFChars(errorCode, 0);
	NativeBannerAdsJNI::getInstance()->onNativeLoadFailed(str);
	pEnv->ReleaseStringUTFChars(errorCode, str);
}

/*
 * Class:     com_common_android_ads_jni_NativeBannerAdsJNI
 * Method:    nativeOnNativeAdsClicked
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_NativeBannerAdsJni_nativeOnNativeAdsClicked
  (JNIEnv *pEnv, jobject thisObj)
{
	NativeBannerAdsJNI::getInstance()->onNativeClicked();
}

/*
 * Class:     com_common_android_ads_jni_NativeBannerAdsJNI
 * Method:    nativeOnNativeAdsCollapsed
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_NativeBannerAdsJni_nativeOnNativeAdsCollapsed
  (JNIEnv *, jobject)
{
	NativeBannerAdsJNI::getInstance()->onNativeCollapsed();
}

/*
 * Class:     com_common_android_ads_jni_NativeBannerAdsJNI
 * Method:    nativeOnNativeAdsExpanded
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_NativeBannerAdsJni_nativeOnNativeAdsExpanded
  (JNIEnv *, jobject)
{
	NativeBannerAdsJNI::getInstance()->onNativeExpanded();
}
