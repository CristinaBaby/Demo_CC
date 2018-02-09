//
//  Analytics.h
//  MPM4121
//
//  Created by hujie2 on 15/11/2.
//
//

#ifndef __MPM4121__Analytics__
#define __MPM4121__Analytics__

#include <stdio.h>
#include <string>
#include <map>
class Analytics
{
protected:
    Analytics();
    ~Analytics();
    
protected:
    
    static Analytics*  m_pInstance;
public:
    static Analytics* getInstance();
    
public:
    void sendEvent(std::string eventName, std::string value);
    
    void sendEvent(std::string eventName, std::map<std::string , std::string> mapValue);
    
    void sendEvent(std::string category, std::string action, std::string label, long value);
    
    void sendScreenEvent(std::string sreenName);
    
    void setDebugMode(bool bIsDebug);
    
    bool getDebugMode();
    
    void endSession();
};
#endif /* defined(__MPM4121__Analytics__) */
