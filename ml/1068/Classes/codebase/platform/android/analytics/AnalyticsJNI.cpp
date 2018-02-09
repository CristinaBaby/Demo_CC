//
//  AnalyticsJNI.cpp
//  AnalyticsCenter
//
//  Created by Jimmy on 15/11/2.
//
//

#include "AnalyticsJNI.h"
#include "../JNIHelper.h"

AnalyticsJNI::AnalyticsJNI()
: ClassAnalytics(0)
,_analyticsJava(0)
,_methodSendEvent(0)
,_methodSendEvents(0)
,_methodSendScreenEvent(0)
,_methodEndSession(0)
,_methodSetDebugMode(0)
,_methodGetDebugMode(0)
,_methodSendEventMore(0)
{

}

AnalyticsJNI* AnalyticsJNI::m_pInstance = NULL;

AnalyticsJNI::~AnalyticsJNI() {

}

AnalyticsJNI* AnalyticsJNI::getInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new AnalyticsJNI();
	}
	return m_pInstance;
}

bool AnalyticsJNI::init(JNIEnv *pEnv, jobject pAdJava)
{
	JNIHelper::getInstance()->init(pEnv);
	ClassAnalytics = pEnv->GetObjectClass(pAdJava);
	if (!ClassAnalytics)
	{
		LOGE("initial JNI Analytics class Failed!");
		return false;
	}
	ClassAnalytics = (jclass) JNIHelper::makeGlobalRef(pEnv, ClassAnalytics);

	_analyticsJava = JNIHelper::makeGlobalRef(pEnv, pAdJava);
	if (!_analyticsJava)
	{
		LOGE("Cache JNI Analytics jobject Java Failed!");
		return false;
	}

	_methodSendEvent =  pEnv->GetMethodID(ClassAnalytics, "sendEvent", "(Ljava/lang/String;Ljava/lang/String;)V");

	if (!_methodSendEvent)
	{
		LOGE("JNI get Java method sendEvent(String, String) Failed!");
		return false;
	}

	_methodSendEvents =  pEnv->GetMethodID(ClassAnalytics, "sendEvent", "(Ljava/lang/String;Ljava/util/HashMap;)V");
	if (!_methodSendEvents)
	{
		LOGE("JNI get Java method sendEvent(String, HashMap) Failed!");
		return false;
	}
    
    _methodSendEventMore = pEnv->GetMethodID(ClassAnalytics, "sendEvent", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;J)V");

	_methodSendScreenEvent =  pEnv->GetMethodID(ClassAnalytics, "sendScreenEvent", "(Ljava/lang/String;)V");
	if (!_methodSendScreenEvent)
	{
		LOGE("JNI get Java method sendScreenEvent Failed!");
		return false;
	}

	_methodSetDebugMode = pEnv->GetMethodID(ClassAnalytics, "setDebugMode", "(Z)V");
	if(!_methodSetDebugMode)
	{
		LOGE("JNI get Java method setDubugMode Failed!");
		return false;
	}

	_methodEndSession = pEnv->GetMethodID(ClassAnalytics, "endSession", "()V");
	if(!_methodEndSession)
	{
		LOGE("JNI get Java method endSession Failed!");
		return false;
	}

	_methodGetDebugMode = pEnv->GetMethodID(ClassAnalytics, "getDebugMode", "()Z");
	if(!_methodGetDebugMode)
	{
		LOGE("JNI get Java method endSession Failed!");
		return false;
	}

	return true;
}

void AnalyticsJNI::sendEvent(std::string eventName, std::string value)
{
	if (!_analyticsJava)
	{
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jstring name = lEnv->NewStringUTF(eventName.c_str());
	jstring values = lEnv->NewStringUTF(value.c_str());
	lEnv->CallVoidMethod(_analyticsJava, _methodSendEvent, name, values);
}

void AnalyticsJNI::sendEvent(std::string eventName, std::map<std::string , std::string> mapValue)
{
	if (!_analyticsJava)
	{
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jstring name = lEnv->NewStringUTF(eventName.c_str());

	/**
	 *c++ map 转化为 java HashMap
	 **/
	jclass map_cls = lEnv->FindClass("java/util/HashMap");// 获取java HashMap类
	jmethodID map_costruct = lEnv->GetMethodID(map_cls , "<init>","()V");// 获取java HashMap的构造函数
	jobject map_obj = lEnv->NewObject(map_cls , map_costruct);// 通过构造方法初始化一个java HashMap
	jmethodID map_put  = lEnv->GetMethodID(map_cls,"put","(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");// 获取java HashMap的put方法

	jclass string_cls = lEnv->FindClass("java/lang/String");// 获取java String类，在c++ std::string 转化到Java String用到
	jmethodID string_costruct = lEnv->GetMethodID(string_cls, "<init>", "([B)V");// 获取java String的构造方法

	//循环取出c++map中的类容放进Java HashMap中
	for(auto itr = mapValue.begin(); itr != mapValue.end(); itr++)
	{
		std::string ckey =  itr->first;
		std::string cvalue =  itr->second;

		//string obj转化
		const char *keychar = ckey.c_str();
		jbyteArray keybytes = lEnv->NewByteArray(strlen(keychar));
		lEnv->SetByteArrayRegion(keybytes, 0, strlen(keychar), (jbyte*)keychar);
		jobject KeyElement = lEnv->NewObject( string_cls, string_costruct, keybytes);


		const char *valuechar = cvalue.c_str();
		jbyteArray valuebytes = lEnv->NewByteArray(strlen(valuechar));
		lEnv->SetByteArrayRegion(valuebytes, 0, strlen(valuechar), (jbyte*)valuechar);
		jobject ValueElement = lEnv->NewObject( string_cls, string_costruct, valuebytes);

		lEnv->CallObjectMethod(map_obj,map_put, KeyElement, ValueElement);

	}

	lEnv->CallVoidMethod(_analyticsJava, _methodSendEvents, name, map_obj);

}

void AnalyticsJNI::sendEvent(std::string category, std::string action, std::string label, long value)
{
    if (!_analyticsJava)
    {
        return;
    }
    JNIEnv* lEnv = JNIHelper::getJNIEnv();
    jstring ca = lEnv->NewStringUTF(category.c_str());
    jstring ac = lEnv->NewStringUTF(action.c_str());
    jstring la = lEnv->NewStringUTF(label.c_str());
    long long values = (long long)value;
    lEnv->CallVoidMethod(_analyticsJava, _methodSendEventMore, ca, ac, la, values);
}

void AnalyticsJNI::sendScreenEvent(std::string screenName)
{
	if (!_analyticsJava)
	{
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jstring name = lEnv->NewStringUTF(screenName.c_str());
	lEnv->CallVoidMethod(_analyticsJava, _methodSendScreenEvent, name);
//    std::string value = "UserEvent";
//    jstring values = lEnv->NewStringUTF(value.c_str());
//    lEnv->CallVoidMethod(_analyticsJava, _methodSendEvent, name, values);
}

void AnalyticsJNI::setDebugMode(bool bIsDebug)
{
	if (!_analyticsJava)
	{
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_analyticsJava, _methodSetDebugMode, bIsDebug);
}

bool AnalyticsJNI::getDebugMode()
{
	if (!_analyticsJava)
	{
		return false;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	jboolean ret = lEnv->CallBooleanMethod(_analyticsJava, _methodGetDebugMode);
	return (ret == JNI_TRUE ? true : false);
}

void AnalyticsJNI::endSession()
{
	if (!_analyticsJava)
	{
		return;
	}
	JNIEnv* lEnv = JNIHelper::getJNIEnv();
	lEnv->CallVoidMethod(_analyticsJava, _methodEndSession);
}
