
/*
 * com_common_android_ads_jni_CrosspromoAdsJni.cpp
 *
 *  Created on: 2015-07-09
 *      Author: Jimmy.Hu
 */
#include "com_common_android_ads_jni_CrosspromoAdsJni.h"
#include "CrosspromoAdsJNI.h"


JNIEXPORT void JNICALL Java_com_common_android_ads_jni_CrosspromoAdsJni_nativeInit
  (JNIEnv *pEnv, jobject thisObj)
{
	CrosspromoAdsJNI::getInstance()->init(pEnv, thisObj);
}

JNIEXPORT void JNICALL Java_com_common_android_ads_jni_CrosspromoAdsJni_nativeOnCrosspromoLoaded
  (JNIEnv *pEnv, jobject thisObj)
{
	CrosspromoAdsJNI::getInstance()->onCrosspromoLoaded();
}

/*
 * Class:     com_common_android_ads_jni_CrosspromoAdsJni
 * Method:    nativeOnCrosspromoFailed
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_CrosspromoAdsJni_nativeOnCrosspromoFailed
  (JNIEnv *pEnv, jobject thisObj, jstring errorCode)
{
	const char *str = pEnv->GetStringUTFChars(errorCode, 0);
	CrosspromoAdsJNI::getInstance()->onCrosspromoFailed(str);
	pEnv->ReleaseStringUTFChars(errorCode, str);
}

/*
 * Class:     com_common_android_ads_jni_CrosspromoAdsJni
 * Method:    nativeOnCrosspromoClicked
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_CrosspromoAdsJni_nativeOnCrosspromoClicked
  (JNIEnv *pEnv, jobject thisObj)
{
	CrosspromoAdsJNI::getInstance()->onCrosspromoClicked();
}

/*
 * Class:     com_common_android_ads_jni_CrosspromoAdsJni
 * Method:    nativeOnCrosspromoCollapsed
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_CrosspromoAdsJni_nativeOnCrosspromoCollapsed
  (JNIEnv *, jobject)
{
	CrosspromoAdsJNI::getInstance()->onCrosspromoCollapsed();
}

/*
 * Class:     com_common_android_ads_jni_CrosspromoAdsJni
 * Method:    nativeOnCrosspromoExpanded
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_common_android_ads_jni_CrosspromoAdsJni_nativeOnCrosspromoExpanded
  (JNIEnv *, jobject)
{
	CrosspromoAdsJNI::getInstance()->onCrosspromoExpanded();
}
