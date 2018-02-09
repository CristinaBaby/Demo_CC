//
//  AnalyticsJNI.h
//  AnalyticsCenter
//
//  Created by Jimmy on 15/11/2.
//
//

#ifndef __AnalyticsCenter__AnalyticsJNI__
#define __AnalyticsCenter__AnalyticsJNI__

#include <stdio.h>
#include <jni.h>
#include <string>
#include <map>
class AnalyticsJNI
{
protected:

    AnalyticsJNI();

    ~AnalyticsJNI();

public:

    static AnalyticsJNI* getInstance();

    bool init(JNIEnv * pEnv, jobject pAdJava);

protected:

    static AnalyticsJNI *m_pInstance;

public:

    void sendEvent(std::string eventName, std::string value);

    void sendEvent(std::string eventName, std::map<std::string , std::string> mapValue);
    
    void sendEvent(std::string category, std::string action, std::string label, long value);

    void sendScreenEvent(std::string sreenName);

    void setDebugMode(bool);

    bool getDebugMode();

    void endSession();
protected:
    // Cached Classes.
    jclass ClassAnalytics;

    // Cached java object
    jobject _analyticsJava;

    jmethodID _methodSendEvent;

    jmethodID _methodSendEvents;
    
    jmethodID _methodSendEventMore;

    jmethodID _methodSendScreenEvent;

    jmethodID _methodEndSession;

    jmethodID _methodSetDebugMode;

    jmethodID _methodGetDebugMode;
};

#endif /* defined(__AnalyticsCenter__AnalyticsJNI__) */
