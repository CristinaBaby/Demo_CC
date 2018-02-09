//
//  ST_JNI_Helper.cpp
//  MysteryGuardians
//
//  Created by Steven.Xc.Tian on 13-6-25.
//
//

#include "JNIHelper.h"

JavaVM* JNIHelper::stJavaVM = 0;

static JNIHelper *m_pInstance = 0;

void JNIHelper::init(JNIEnv * pEnv)
{
	// Caches the VM.
	if (pEnv->GetJavaVM(&stJavaVM) != JNI_OK)
	{
		LOGE("get JavaVM Failed!");
		return;
	}
}

jclass JNIHelper::getJClass(std::string classStr){
//	JNIEnv* env=getJNIEnv();
//
//	jclass cls = env->FindClass(classStr);
//	return (jclass)makeGlobalRef(cls);
	//return cls
}
 jobject JNIHelper::getJObject(jclass cls){
	 JNIEnv* env=getJNIEnv();
	 //根据类的CLASS对象获取该类的实例
	 jobject obj = env->AllocObject(cls);
	 return makeGlobalRef(obj);
}

 jobject JNIHelper::getJObject(std::string cls){
	 jclass _class=getJClass(cls);
	return getJObject(_class);
 }

JNIEnv* JNIHelper::getJNIEnv()
{
//	LOGE("======getJNIEnv 001");
	JavaVMAttachArgs lJavaVMAttachArgs;
	lJavaVMAttachArgs.version = JNI_VERSION_1_6;
	lJavaVMAttachArgs.name = "NativeThread";
	lJavaVMAttachArgs.group = 0;

	JNIEnv* lEnv;
	if (stJavaVM->AttachCurrentThread(&lEnv, &lJavaVMAttachArgs) != JNI_OK)
	{
		lEnv = 0;
	}
	return lEnv;
}

jobject JNIHelper::makeGlobalRef(jobject pRef)
{
	return makeGlobalRef(getJNIEnv(), pRef);
}

jobject JNIHelper::makeGlobalRef(JNIEnv* pEnv, jobject pRef)
{
	if (pRef)
	{
		jobject lGlobalRef = pEnv->NewGlobalRef(pRef);
		// No need for a local reference any more.
		pEnv->DeleteLocalRef(pRef);
		// Here, lGlobalRef may be null.
		return lGlobalRef;
	}
}

void JNIHelper::deleteGlobalRef(jobject pRef)
{
	deleteGlobalRef(getJNIEnv(), pRef);
}

void JNIHelper::deleteGlobalRef(JNIEnv* pEnv, jobject pRef)
{
	if (pRef)
	{
		pEnv->DeleteGlobalRef(pRef);
		pRef = 0;
	}
}

JNIHelper* JNIHelper::getInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new JNIHelper();
	}

	return m_pInstance;
}

JNIHelper::~JNIHelper()
{
    stJavaVM = 0;
}
