//
//  FlurryEventManager.cpp
//
//
//

#include "FlurryEventManager.h"
#include "AnalyticX.h"
#include "Analytics.h"
#include "cocos2d.h"
USING_NS_CC;

FlurryEventManager* FlurryEventManager::getInstance()
{
    static FlurryEventManager fm;
    return &fm;
}

int FlurryEventManager::logEvent(const string name)
{
    return logEventWithParam(name, "", "");
}

int FlurryEventManager::logEventTimed(const string name)
{
    return logEventTimedWithParam(name, "", "");
}

void FlurryEventManager::logEventTimedEnd(const string name)
{
    logEventTimedEndWithParam(name, "", "");
}

int FlurryEventManager::logEventWithParam(const string eventName, const string key, const string value)
{
    
    __Dictionary* dic = __Dictionary::create();
    if(key.length() > 0 && value.length() > 0)
        dic->setObject(__String::create(value), key);
    AnalyticX::flurryLogEventWithParameters(eventName.c_str(), dic);
    return 0;
}

int FlurryEventManager::logEventWithParam(const string eventName, map<string, string> mapParam)
{
    __Dictionary* dic = __Dictionary::create();
    for(auto it = mapParam.begin(); it != mapParam.end(); ++it)
    {
        dic->setObject(__String::create(it->second), it->first);
    }
    AnalyticX::flurryLogEventWithParameters(eventName.c_str(), dic);
    return 0;
}

int FlurryEventManager::logEventTimedWithParam(const string eventName, const string key, const string value)
{
    __Dictionary* dic = __Dictionary::create();
    dic->setObject(__String::create(value), key);
    AnalyticX::flurryLogEventWithParametersTimed(eventName.c_str(), dic, true);
    return 0;
}

int FlurryEventManager::logEventTimedWithParam(const string eventName, map<string, string> mapParam)
{
    __Dictionary* dic = __Dictionary::create();
    for(auto it = mapParam.begin(); it != mapParam.end(); ++it)
    {
        dic->setObject(__String::create(it->second), it->first);
    }
    AnalyticX::flurryLogEventWithParametersTimed(eventName.c_str(), dic, true);
    return 0;
}

void FlurryEventManager::logEventTimedEndWithParam(const string eventName, const string key, const string value)
{
    __Dictionary* dic = __Dictionary::create();
    if(key.length() > 0 && value.length() > 0)
        dic->setObject(__String::create(value), key);
    AnalyticX::flurryEndTimedEventWithParameters(eventName.c_str(), dic);
}

void FlurryEventManager::logEventTimedEndWithParam(const string eventName, map<string, string> mapParam)
{
    __Dictionary* dic = __Dictionary::create();
    for(auto it = mapParam.begin(); it != mapParam.end(); ++it)
    {
        dic->setObject(__String::create(it->second), it->first);
    }
    
    AnalyticX::flurryEndTimedEventWithParameters(eventName.c_str(), dic);
}

void FlurryEventManager::logCurrentModuleEnterEvent(string moduleName)
{
    if(Analytics::getInstance()->getDebugMode()) {
        string message = "sendEvent(enter event:"+moduleName+")";
//        CCLOG(message.c_str());
    }
    string eventName = moduleName;
    if(vModulesNamePath.size() > 0)
    {
        logEventTimedWithParam(eventName, "PreviousModule", vModulesNamePath.back());
    }
    else
    {
        logEventTimed(eventName);
    }
    vModulesNamePath.push_back(moduleName);
}

void FlurryEventManager::logCurrentModuleEnterEvent(string moduleName, map<string, string> mapParam)
{
    mapParam.insert(make_pair("PreviousModule", vModulesNamePath.back()));
    logEventTimedWithParam(moduleName, mapParam);
}

void FlurryEventManager::logCurrentModuleExitEvent(string moduleName)
{
    if(Analytics::getInstance()->getDebugMode()) {
        string message = "sendEvent(exit event:"+moduleName+")";
//        CCLOG(message.c_str());
    }
    logEventTimedEnd(moduleName);
}

void FlurryEventManager::logCurrentModuleExitEvent(string moduleName, map<string, string> mapParam)
{
    logEventTimedEndWithParam(moduleName, mapParam);
}

void FlurryEventManager::pushModule(string name)
{
    vModulesNamePath.push_back(name);
}

void FlurryEventManager::clearModules()
{
    vModulesNamePath.clear();
}