//
// Created by Jimmy on 16/1/12.
//

#ifndef INTERNAL_MICAudioManagerJNI_H
#define INTERNAL_MICAudioManagerJNI_H
#include "JNIHelper.h"
#include <stdio.h>
#include <functional>
#include <string>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class MICAudioManagerJNI {
private:
    MICAudioManagerJNI():
    MICAudioManagerJNIClass(0),
    MICAudioManagerJNIJava(0),
    MethodInit(0),
    MethodDeviceReady(0),
    MethodStartRecord(0),
    MethodStopRecord(0),
    MethodGetPower(0)
    {}
    ~MICAudioManagerJNI();
    
public:
    static MICAudioManagerJNI* getInstance();
    
    bool init(JNIEnv * pEnv, jobject pJava);
    
    bool deviceReady();
    void startRecord();
    void stopRecord();
    int getPower();
    
private:
    /*>>>>>>>> below declaration is used by jni <<<<<<<<*/
    // Cached Classes.
    jclass MICAudioManagerJNIClass;
    // Cached java object
    jobject MICAudioManagerJNIJava;
    // Cached active Methods.
    
    jmethodID MethodInit;
    
    jmethodID MethodDeviceReady;
    
    jmethodID MethodStartRecord;
    
    jmethodID MethodStopRecord;
    
    jmethodID MethodGetPower;
};

#endif //INTERNAL_MICAudioManagerJNI_H
