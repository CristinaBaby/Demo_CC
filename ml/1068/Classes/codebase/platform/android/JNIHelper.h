//
//  JNIHelper.h
//
//
//
//
//

#ifndef __MysteryGuardians__ST_JNI_Helper__
#define __MysteryGuardians__ST_JNI_Helper__

#include <jni.h>
#include <android/log.h>
#include <iostream>
#define LOG_TAG "from jni"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

class JNIHelper {
public:
	JNIHelper(){}
    virtual ~JNIHelper();
    static JNIHelper* getInstance();
public:
    // Cached JNI references.
	static JavaVM* stJavaVM;
public:
	static jclass getJClass(std::string);
	static jobject getJObject(jclass);
	static jobject getJObject(std::string );
	static JNIEnv* getJNIEnv();
	static jobject makeGlobalRef(JNIEnv* pEnv, jobject pRef);
	static jobject makeGlobalRef(jobject pRef);
	static void deleteGlobalRef(JNIEnv* pEnv, jobject pRef);
	static void deleteGlobalRef(jobject pRef);
public:
	virtual void init(JNIEnv* pEnv);
};

#endif
