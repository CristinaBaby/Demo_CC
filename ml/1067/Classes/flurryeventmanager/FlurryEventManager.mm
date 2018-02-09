//
//  FlurryEventManager.m
//
//
//

#include "FlurryEventManager.h"
#include <AnalyticsSDK/Flurry.h>
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

void FlurryEventManager::logCurrentModuleEnterEvent(string moduleName)
{
    if(Analytics::getInstance()->getDebugMode()) {
        string message = "sendEvent(enter event:"+moduleName+")";
        CCLOG(message.c_str());
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
    vModulesNamePath.push_back(moduleName);
}

void FlurryEventManager::logCurrentModuleExitEvent(string moduleName)
{
    if(Analytics::getInstance()->getDebugMode()) {
        string message = "sendEvent(exit event:"+moduleName+")";
        CCLOG(message.c_str());
    }
    logEventTimedEnd(moduleName);
}

void FlurryEventManager::logCurrentModuleExitEvent(string moduleName, map<string, string> mapParam)
{
    logEventTimedEndWithParam(moduleName, mapParam);
}

int FlurryEventManager::logEventTimedWithParam(const string eventName, const string key, const string value)
{
    if(key.length() > 0 && value.length() > 0)
    {
        return [Flurry logEvent:[NSString stringWithCString:eventName.c_str() encoding:NSUTF8StringEncoding]
                 withParameters:[NSDictionary dictionaryWithObject:[NSString stringWithCString:value.c_str() encoding:NSUTF8StringEncoding] forKey:[NSString stringWithCString:key.c_str() encoding:NSUTF8StringEncoding]]
                          timed:YES];
    }
    return [Flurry logEvent:[NSString stringWithCString:eventName.c_str() encoding:NSUTF8StringEncoding] timed:YES];
}

int FlurryEventManager::logEventTimedWithParam(const string eventName, map<string, string> mapParam)
{
    NSMutableDictionary* dic = [NSMutableDictionary dictionary];
    for(auto it = mapParam.begin(); it != mapParam.end(); ++it)
    {
        [dic setObject:[NSString stringWithCString:it->second.c_str() encoding:NSUTF8StringEncoding] forKey:[NSString stringWithCString:it->first.c_str() encoding:NSUTF8StringEncoding]];
    }
    return [Flurry logEvent:[NSString stringWithCString:eventName.c_str() encoding:NSUTF8StringEncoding] withParameters:dic timed:YES];
}

void FlurryEventManager::logEventTimedEndWithParam(const string eventName, const string key, const string value)
{
    if(key.length() > 0 && value.length() > 0)
    {
        [Flurry endTimedEvent:[NSString stringWithCString:eventName.c_str() encoding:NSUTF8StringEncoding]
               withParameters:[NSDictionary dictionaryWithObject:[NSString stringWithCString:value.c_str() encoding:NSUTF8StringEncoding] forKey:[NSString stringWithCString:key.c_str() encoding:NSUTF8StringEncoding]]];
    }
    [Flurry endTimedEvent:[NSString stringWithCString:eventName.c_str() encoding:NSUTF8StringEncoding] withParameters:nil];
}

void FlurryEventManager::logEventTimedEndWithParam(const string eventName, map<string, string> mapParam)
{
    NSMutableDictionary* dic = [NSMutableDictionary dictionary];
    for(auto it = mapParam.begin(); it != mapParam.end(); ++it)
    {
        [dic setObject:[NSString stringWithCString:it->second.c_str() encoding:NSUTF8StringEncoding] forKey:[NSString stringWithCString:it->first.c_str() encoding:NSUTF8StringEncoding]];
    }
    [Flurry endTimedEvent:[NSString stringWithCString:eventName.c_str() encoding:NSUTF8StringEncoding] withParameters:dic];
}

int FlurryEventManager::logEventWithParam(const string eventName, const string key, const string value)
{
    if(key.length() > 0 && value.length() > 0)
    {
        return [Flurry logEvent:[NSString stringWithCString:eventName.c_str() encoding:NSUTF8StringEncoding]
               withParameters:[NSDictionary dictionaryWithObject:[NSString stringWithCString:value.c_str() encoding:NSUTF8StringEncoding] forKey:[NSString stringWithCString:key.c_str() encoding:NSUTF8StringEncoding]]];
    }
    return [Flurry logEvent:[NSString stringWithCString:eventName.c_str() encoding:NSUTF8StringEncoding] withParameters:nil];
}

int FlurryEventManager::logEventWithParam(const string eventName, map<string, string> mapParam)
{
    NSMutableDictionary* dic = [NSMutableDictionary dictionary];
    for(auto it = mapParam.begin(); it != mapParam.end(); ++it)
    {
        [dic setObject:[NSString stringWithCString:it->second.c_str() encoding:NSUTF8StringEncoding] forKey:[NSString stringWithCString:it->first.c_str() encoding:NSUTF8StringEncoding]];
    }
    return [Flurry logEvent:[NSString stringWithCString:eventName.c_str() encoding:NSUTF8StringEncoding] withParameters:dic];
}


void FlurryEventManager::pushModule(string name)
{
    vModulesNamePath.push_back(name);
}

void FlurryEventManager::clearModules()
{
    vModulesNamePath.clear();
}