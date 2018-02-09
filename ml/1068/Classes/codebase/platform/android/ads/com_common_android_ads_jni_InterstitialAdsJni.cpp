
/*
 * com_common_android_ads_jni_InterstitialAdsJni.cpp
 *
 *  Created on: 2015-07-09
 *      Author: Jimmy.Hu
 */
#include "com_common_android_ads_jni_InterstitialAdsJni.h"
#include "InterstitialAdsJNI.h"


JNIEXPORT void JNICALL Java_com_common_android_ads_jni_InterstitialAdsJni_nativeInit
  (JNIEnv *pEnv, jobject thisObj)
{
	InterstitialAdsJNI::getInstance()->init(pEnv, thisObj);
}

JNIEXPORT void JNICALL Java_com_common_android_ads_jni_InterstitialAdsJni_nativeOnInterstitialLoaded
  (JNIEnv *pEnv, jobject thisObj)
{
	InterstitialAdsJNI::getInstance()->onInterstitialLoaded();
}

/*
 * Class:     com_common_android_ads_jni_InterstitialAdsJni
 * Method:    nativeOnInterstitialFailed
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_InterstitialAdsJni_nativeOnInterstitialFailed
  (JNIEnv *pEnv, jobject thisObj, jstring errorCode)
{
	const char *str = pEnv->GetStringUTFChars(errorCode, 0);
	InterstitialAdsJNI::getInstance()->onInterstitialFailed(str);
	pEnv->ReleaseStringUTFChars(errorCode, str);
}

/*
 * Class:     com_common_android_ads_jni_InterstitialAdsJni
 * Method:    nativeOnInterstitialClicked
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_InterstitialAdsJni_nativeOnInterstitialClicked
  (JNIEnv *pEnv, jobject thisObj)
{
	InterstitialAdsJNI::getInstance()->onInterstitialClicked();
}

/*
 * Class:     com_common_android_ads_jni_InterstitialAdsJni
 * Method:    nativeOnInterstitialCollapsed
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_InterstitialAdsJni_nativeOnInterstitialCollapsed
  (JNIEnv *, jobject)
{
	InterstitialAdsJNI::getInstance()->onInterstitialCollapsed();
}

/*
 * Class:     com_common_android_ads_jni_InterstitialAdsJni
 * Method:    nativeOnInterstitialExpanded
 * Signature: ()V
 */

JNIEXPORT void JNICALL Java_com_common_android_ads_jni_InterstitialAdsJni_nativeOnInterstitialExpanded
  (JNIEnv *, jobject)
{
	InterstitialAdsJNI::getInstance()->onInterstitialExpanded();
}
