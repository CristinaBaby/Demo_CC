
#include "com_helper_MICAudioManager.h"

#include "MICAudioManagerJNI.h"
JNIEXPORT void JNICALL Java_com_helper_MICAudioManager_nativeInit
    (JNIEnv *pEnv, jobject obj){
        
        MICAudioManagerJNI::getInstance()->init(pEnv, obj);
    }
