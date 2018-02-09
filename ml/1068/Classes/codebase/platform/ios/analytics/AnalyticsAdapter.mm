//
//  AnalyticsAdapter.cpp


#include "AnalyticsAdapter.h"
#import <AnalyticsSDK/AnalyticsSDK.h>

AnalyticsAdapter* AnalyticsAdapter::m_pInstance = NULL;

AnalyticsAdapter* AnalyticsAdapter::getInstance()
{
    if(!m_pInstance)
    {
        m_pInstance = new AnalyticsAdapter;
    }
    
    return m_pInstance;
}

void AnalyticsAdapter::startSession(int types)
{
    if(types <= 0)
        return;
    
    [[SSCAnalytics getInstance] startSession:types];
}

void AnalyticsAdapter::endSession()
{
    [[SSCAnalytics getInstance] endSession];
}

void AnalyticsAdapter::sendEvent(string name, string label, string category)
{
    if(name.empty() || category.empty())
        return;
    
    char* pName = (char*)name.data();
    if(pName && strlen(pName))
    {
        NSString* nameStr = [NSString stringWithFormat:@"%s",pName];
        NSString* labelStr = nil;
        NSString* categoryStr = nil;
        
        char* pLabel = (char*)label.data();
        if(pLabel && strlen(pLabel))
            labelStr = [NSString stringWithFormat:@"%s",pLabel];
        
        char* pCategory = (char*)category.data();
        if(pCategory && strlen(pCategory))
            categoryStr = [NSString stringWithFormat:@"%s",pCategory];
        
        [[SSCAnalytics getInstance] sendEvent:categoryStr action:nameStr label:labelStr];
    }
}

void AnalyticsAdapter::sendEvent(string name, map<string,string> params, string category)
{
    if(name.empty() || category.empty())
        return;

    NSString* nameStr = nil;
    char* pName = (char*)name.data();
    if(pName && strlen(pName))
        nameStr = [NSString stringWithFormat:@"%s",pName];
    
    if(0 == [nameStr length])
        return;
        
    NSMutableDictionary* dict = [[NSMutableDictionary alloc] init];
    
    map<string,string>::iterator it;
    for(it = params.begin(); it != params.end(); ++it)
    {
        //cout << it->first<<"|"<<it->second<<endl;
        
        string key = it->first;
        string value = it->second;
        
        char* pKey = (char*)key.data();
        if(pKey && strlen(pKey))
        {
            BOOL isValue = NO;
            NSString* keyStr = [NSString stringWithFormat:@"%s",pKey];
            if([keyStr isEqualToString:@"value"])
            {
                isValue = YES;
            }
            
            char* pValue = (char*)value.data();
            if(pValue && strlen(pValue))
            {
                NSString* valueStr = [NSString stringWithFormat:@"%s",pValue];
                
                if(isValue)
                {
                    NSNumber* valueNum = [NSNumber numberWithInt:[valueStr intValue]];
                    [dict setObject:valueNum forKey:keyStr];
                }
                else
                    [dict setObject:valueStr forKey:keyStr];
                
            }
        }
        
        //NSLog(@"event-dict:%@",dict);
    }
    
    NSString* categoryStr = nil;
    char* pCategory = (char*)category.data();
    if(pCategory && strlen(pCategory))
        categoryStr = [NSString stringWithFormat:@"%s",pCategory];
    
    [[SSCAnalytics getInstance] sendEvent:categoryStr action:nameStr params:dict];
}

void AnalyticsAdapter::sendEvent(string category, string action, string label, long value)
{
    if(category.empty() || action.empty())
        return;
    
    NSString* categoryStr = nil;
    char* pCategory = (char*)category.data();
    if(pCategory && strlen(pCategory))
        categoryStr = [NSString stringWithFormat:@"%s",pCategory];
    
    if(0 == [categoryStr length])
        return;
    

    NSString* actionStr = nil;
    char* pAction = (char*)action.data();
    if(pAction && strlen(pAction))
        actionStr = [NSString stringWithFormat:@"%s",pAction];
    
    if(0 == [actionStr length])
        return;
    
    NSString* labelStr = @"";
    char* pLabel = (char*)label.data();
    if(pLabel && strlen(pLabel))
        labelStr = [NSString stringWithFormat:@"%s",pLabel];
    
    NSNumber* number = [NSNumber numberWithLong:value];
    
    [[SSCAnalytics getInstance] sendEvent:categoryStr action:actionStr label:labelStr value:number params:nil];
}

/*
void AnalyticsAdapter::sendEvent(map<string,string> params)
{
    NSMutableDictionary* dict = [[NSMutableDictionary alloc] init];
    
    map<string,string>::iterator it;
    for(it = params.begin(); it != params.end(); ++it)
    {
        cout << it->first<<"|"<<it->second<<endl;
        
        string key = it->first;
        string value = it->second;
        
        char* pKey = (char*)key.data();
        if(pKey && strlen(pKey))
        {
            BOOL isValue = NO;
            NSString* keyStr = [NSString stringWithFormat:@"%s",pKey];
            if([keyStr isEqualToString:@"value"])
            {
                isValue = YES;
            }
           
            char* pValue = (char*)value.data();
            if(pValue && strlen(pValue))
            {
                NSString* valueStr = [NSString stringWithFormat:@"%s",pValue];
                
                if(isValue)
                {
                    NSNumber* valueNum = [NSNumber numberWithInt:[valueStr intValue]];
                    [dict setObject:valueNum forKey:keyStr];
                }
                else
                    [dict setObject:valueStr forKey:keyStr];
                
            }
        }
        
        NSLog(@"event-dict:%@",dict);
        [[Analytics getInstance] sendEvent:dict];
    }
}
 */

void AnalyticsAdapter::sendEventScreen(string screenName)
{
    if(screenName.empty())
        return;
    
    char* pScreenName = (char*)screenName.data();
    if(pScreenName && strlen(pScreenName))
    {
        NSString* temp = [NSString stringWithFormat:@"%s",pScreenName];
        [[SSCAnalytics getInstance] sendEventScreen:temp];
    }
    
}

void AnalyticsAdapter::setDebugMode(bool isDebug)
{
    [[SSCAnalytics getInstance] setDebugMode:isDebug];
}

bool AnalyticsAdapter::getDebugMode()
{
    return [[SSCAnalytics getInstance] getDebugMode];
}