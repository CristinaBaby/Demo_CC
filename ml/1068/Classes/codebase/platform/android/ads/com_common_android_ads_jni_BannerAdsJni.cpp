
/*
 * com_common_android_ads_jni_BannerAdsJni.cpp
 *
 *  Created on: 2015-07-09
 *      Author: Jimmy.Hu
 */
#include "com_common_android_ads_jni_BannerAdsJni.h"
#include "BannerAdsJNI.h"


JNIEXPORT void JNICALL Java_com_common_android_ads_jni_BannerAdsJni_nativeInit
  (JNIEnv *pEnv, jobject thisObj)
{
	BannerAdsJNI::getInstance()->init(pEnv, thisObj);
}

JNIEXPORT void JNICALL Java_com_common_android_ads_jni_BannerAdsJni_nativeOnBannerLoaded
  (JNIEnv *pEnv, jobject thisObj)
{
	BannerAdsJNI::getInstance()->onBannerLoaded();
}

/*
 * Class:     com_common_android_ads_jni_BannerAdsJni
 * Method:    nativeOnBannerFailed
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_BannerAdsJni_nativeOnBannerFailed
  (JNIEnv *pEnv, jobject thisObj, jstring errorCode)
{
//	LOGE("============onBannerFailed=============");
	const char *str = pEnv->GetStringUTFChars(errorCode, 0);
	BannerAdsJNI::getInstance()->onBannerLoadFailed(str);
	pEnv->ReleaseStringUTFChars(errorCode, str);
}

/*
 * Class:     com_common_android_ads_jni_BannerAdsJni
 * Method:    nativeOnBannerClicked
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_BannerAdsJni_nativeOnBannerClicked
  (JNIEnv *pEnv, jobject thisObj)
{
	BannerAdsJNI::getInstance()->onBannerClicked();
}

/*
 * Class:     com_common_android_ads_jni_BannerAdsJni
 * Method:    nativeOnBannerCollapsed
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_BannerAdsJni_nativeOnBannerCollapsed
  (JNIEnv *, jobject)
{
	BannerAdsJNI::getInstance()->onBannerCollapsed();
}

/*
 * Class:     com_common_android_ads_jni_BannerAdsJni
 * Method:    nativeOnBannerExpanded
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_BannerAdsJni_nativeOnBannerExpanded
  (JNIEnv *, jobject)
{
	BannerAdsJNI::getInstance()->onBannerExpanded();
}
