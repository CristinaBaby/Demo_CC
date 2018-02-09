/*
 * ST_SystemFunction_Android.cpp
 *
 *  Created on: 2013-9-2
 *      Author: Steven.Xc.Tian
 */
#include "RuntimePermissionJNI.h"

static RuntimePermissionJNI* sp_RuntimePermissionJNI = 0;

RuntimePermissionJNI* RuntimePermissionJNI::getInstance()
{
	if (!sp_RuntimePermissionJNI)
	{
		sp_RuntimePermissionJNI = new RuntimePermissionJNI();
	}

	return sp_RuntimePermissionJNI;
}

bool RuntimePermissionJNI::init(JNIEnv * pEnv, jobject pJava)
{
    LOGE("%s", __func__);
	// get class
	ClassSF = pEnv->GetObjectClass(pJava);
	if (!ClassSF)
	{
		LOGE("get STSystemFunction class Failed!");
		return false;
	}
	ClassSF = (jclass) JNIHelper::makeGlobalRef(pEnv, ClassSF);

//    MethodRequestPermissons = pEnv->GetMethodID(ClassSF, "requestRuntimePermissions","(I[Ljava/lang/String;)V");
      MethodRequestPermissons = pEnv->GetMethodID(ClassSF, "requestRuntimePermissions","(IJ)V");
    if (!MethodRequestPermissons) {
        LOGE("get MethodRequestPermissons id Failed!");
        return false;
    }
    
	// Caches objects.

	// Caches objects.
	stSFJava = JNIHelper::makeGlobalRef(pEnv, pJava);
	if (!stSFJava)
	{
		LOGE("Cache stSFJava Failed!");
		return false;
	}

	return true;
}

void RuntimePermissionJNI::requestRuntimePermissions(int requestCode,long permissions){
    if (!stSFJava)
    {
        LOGE("RuntimePermissionJNI::%s() failed!", __FUNCTION__);
        return ;
    }
//    LOGE("%s %s", __FILE__, __FUNCTION__);
//    JNIEnv* lEnv = JNIHelper::getJNIEnv();
//    jstring   str;
//    jobjectArray args = 0;
//    jsize  len = permissionCount;
//    int  i=0;
//    args = (lEnv)->NewObjectArray(len,(lEnv)->FindClass("java/lang/String"),0);
//    for( i=0; i < len; i++ )
//    {
//        str = (lEnv)->NewStringUTF(permissions[i].c_str());
//        (lEnv)->SetObjectArrayElement(args, i, str);
//    }
//    lEnv->CallVoidMethod(stSFJava,MethodRequestPermissons,requestCode,args);
    LOGE("%s %s", __FILE__, __FUNCTION__);
    JNIEnv* lEnv = JNIHelper::getJNIEnv();
    lEnv->CallVoidMethod(stSFJava,MethodRequestPermissons,requestCode,permissions);
}

RuntimePermissionJNI::~RuntimePermissionJNI()
{
	JNIHelper::deleteGlobalRef(ClassSF);
	JNIHelper::deleteGlobalRef(stSFJava);
}
