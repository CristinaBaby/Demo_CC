/*
 * com_common_android_permission_PermissionJNI.cpp
 *
 *  Created on: 2013-9-2
 *      Author: Steven.Xc.Tian
 */
#include "com_common_android_permission_PermissionJNI.h"
#include "RuntimePermissionJNI.h"
#include "RuntimePermissionManager.h"

JNIEXPORT void JNICALL Java_com_common_android_permission_PermissionJNI_nativeInit
(JNIEnv * pEnv, jobject thisObj)
{
    LOGE("%s", "Java_com_common_android_permission_PermissionJNI_nativeInit");
	RuntimePermissionJNI::getInstance()->init(pEnv, thisObj);
}

//add by tangjl
JNIEXPORT void JNICALL Java_com_common_android_permission_PermissionJNI_nativeNotifyGranted
(JNIEnv * pEnv, jobject jobj, jint requestcode, jboolean bgranted){
     LOGE("%s", "----->Java_com_common_android_permission_PermissionJNI_nativeNotifyGranted");
    int req = (int)requestcode;
    bool granted = bgranted ==JNI_FALSE? false : true;
#if __cplusplus > 201100L
     LOGE("%s", "----->Java_com_common_android_permission_PermissionJNI_nativeNotifyGranted,C++11");
    if (RuntimePermissionManager::getInstance()->onPermissionGrantedResult) {
        LOGE("%s", "----->Java_com_common_android_permission_PermissionJNI_nativeNotifyGranted,notify to callback");
        RuntimePermissionManager::getInstance()->onPermissionGrantedResult(req,granted);
    }
#else
    if (RuntimePermissionManager::getInstance()->mRuntimePermissionDelegate) {
           LOGE("%s", "----->Java_com_common_android_permission_PermissionJNI_nativeNotifyGranted,(--delegate--)notify to callback");
        RuntimePermissionManager::getInstance()->mRuntimePermissionDelegate->onPermissionGrantedResult(req,granted);
    }
#endif
}

