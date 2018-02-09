#include "com_common_android_analyticscenter_AnalyticsCenterJni.h"
#include "AnalyticsJNI.h"


JNIEXPORT void JNICALL Java_com_common_android_analyticscenter_AnalyticsCenterJni_nativeInit
  (JNIEnv *pEnv, jobject obj)
{
	AnalyticsJNI::getInstance()->init(pEnv, obj);
}
