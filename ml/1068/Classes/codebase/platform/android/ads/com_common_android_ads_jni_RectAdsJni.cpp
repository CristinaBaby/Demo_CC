
/*
 * com_common_android_ads_jni_RectAdsJni.cpp
 *
 *  Created on: 2015-07-09
 *      Author: Jimmy.Hu
 */
#include "com_common_android_ads_jni_RectAdsJni.h"
#include "RectAdsJNI.h"


JNIEXPORT void JNICALL Java_com_common_android_ads_jni_RectAdsJni_nativeInit
  (JNIEnv *pEnv, jobject thisObj)
{
	RectAdsJNI::getInstance()->init(pEnv, thisObj);
}

JNIEXPORT void JNICALL Java_com_common_android_ads_jni_RectAdsJni_nativeOnRectLoaded
  (JNIEnv *pEnv, jobject thisObj)
{
	RectAdsJNI::getInstance()->onRectLoaded();
}

/*
 * Class:     com_common_android_ads_jni_RectAdsJni
 * Method:    nativeOnRectFailed
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_RectAdsJni_nativeOnRectFailed
  (JNIEnv *pEnv, jobject thisObj, jstring errorCode)
{
	const char *str = pEnv->GetStringUTFChars(errorCode, 0);
	RectAdsJNI::getInstance()->onRectLoadFailed(str);
	pEnv->ReleaseStringUTFChars(errorCode, str);
}

/*
 * Class:     com_common_android_ads_jni_RectAdsJni
 * Method:    nativeOnRectClicked
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_RectAdsJni_nativeOnRectClicked
  (JNIEnv *pEnv, jobject thisObj)
{
	RectAdsJNI::getInstance()->onRectClicked();
}

/*
 * Class:     com_common_android_ads_jni_RectAdsJni
 * Method:    nativeOnRectCollapsed
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_RectAdsJni_nativeOnRectCollapsed
  (JNIEnv *, jobject)
{
	RectAdsJNI::getInstance()->onRectCollapsed();
}

/*
 * Class:     com_common_android_ads_jni_RectAdsJni
 * Method:    nativeOnRectExpanded
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_RectAdsJni_nativeOnRectExpanded
  (JNIEnv *, jobject)
{
	RectAdsJNI::getInstance()->onRectExpanded();
}
