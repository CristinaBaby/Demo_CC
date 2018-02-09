//
// Created by Jimmy on 16/1/12.
//

#include "MICAudioManagerJNI.h"
#include "JNIHelper.h"

static MICAudioManagerJNI* pInstance = 0;

MICAudioManagerJNI::~MICAudioManagerJNI()
{
    JNIHelper::deleteGlobalRef(MICAudioManagerJNIClass);
    JNIHelper::deleteGlobalRef(MICAudioManagerJNIJava);
}

MICAudioManagerJNI* MICAudioManagerJNI::getInstance()
{
    if(!pInstance)
    {
        pInstance = new MICAudioManagerJNI();
    }
    
    return pInstance;
}


bool MICAudioManagerJNI::init(JNIEnv *pEnv, jobject pJava)
{
    MICAudioManagerJNIClass = pEnv->GetObjectClass(pJava);
    if (!MICAudioManagerJNIClass)
    {
        LOGE("get MicAudio class Failed!");
        return false;
    }
    
    MICAudioManagerJNIClass = (jclass) JNIHelper::makeGlobalRef(pEnv, MICAudioManagerJNIClass);
    
    MICAudioManagerJNIJava = JNIHelper::makeGlobalRef(pEnv, pJava);
    if (!MICAudioManagerJNIJava)
    {
        LOGE("Cache Java Obj Failed!");
        return false;
    }
    
    MethodInit = pEnv->GetMethodID(MICAudioManagerJNIClass, "init", "()V");
    if(!MethodInit)
    {
        LOGE("get Method init Failed!");
        return false;
    }
    MethodStartRecord = pEnv->GetMethodID(MICAudioManagerJNIClass, "startRecord", "()V");
    if(!MethodStartRecord)
    {
        LOGE("get Method startRecord Failed!");
        return false;
    }
    
    MethodDeviceReady = pEnv->GetMethodID(MICAudioManagerJNIClass, "deviceReady", "()Z");
    if(!MethodDeviceReady)
    {
        LOGE("get Method deviceReady Failed!");
        return false;
    }
    
    MethodStopRecord = pEnv->GetMethodID(MICAudioManagerJNIClass, "stopRecord", "()V");
    if(!MethodStopRecord)
    {
        LOGE("get Method stopRecord Failed!");
        return false;
    }
    
    MethodGetPower = pEnv->GetMethodID(MICAudioManagerJNIClass, "getPower", "()I");
    if (!MethodGetPower) {
        LOGE("get Method getPower Failed!");
        return false;
    }
    return true;
}

bool MICAudioManagerJNI::deviceReady()
{
    if(!MethodDeviceReady)
    {
        LOGE("MICAudioManagerJNI::deviceReady() failed!");
        return false;
    }
    JNIEnv* lEnv = JNIHelper::getJNIEnv();
    jboolean ret = lEnv->CallBooleanMethod(MICAudioManagerJNIJava, MethodDeviceReady);
    return (ret == JNI_TRUE ? true : false);
}
void MICAudioManagerJNI::startRecord()
{
    if(!MethodStartRecord)
    {
        LOGE("MICAudioManagerJNI::startRecord() failed!");
        return;
    }
    
    JNIEnv* lEnv = JNIHelper::getJNIEnv();
    lEnv->CallVoidMethod(MICAudioManagerJNIJava, MethodStartRecord);
    
}
void MICAudioManagerJNI::stopRecord()
{
    if(!MethodStopRecord)
    {
        LOGE("MICAudioManagerJNI::stopRecord() failed!");
        return;
    }
    
    JNIEnv* lEnv = JNIHelper::getJNIEnv();
    lEnv->CallVoidMethod(MICAudioManagerJNIJava, MethodStopRecord);
    
}
int MICAudioManagerJNI::getPower()
{
    if(!MethodGetPower)
    {
        LOGE("MICAudioManagerJNI::getPower() failed!");
        return -120;
    }
    
    JNIEnv* lEnv = JNIHelper::getJNIEnv();
    int power = lEnv->CallIntMethod(MICAudioManagerJNIJava, MethodGetPower);
    return power;
}


