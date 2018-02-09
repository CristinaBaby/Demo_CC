//
//  AnalyticsAdapter.hpp


#ifndef AnalyticsAdapter_h
#define AnalyticsAdapter_h

#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

using namespace std;

class AnalyticsAdapter
{
public:
    static AnalyticsAdapter* getInstance();
    
    void startSession(int types);
    void endSession();
    void sendEvent(string name, string label, string category);
    void sendEvent(string name, map<string,string> params, string category);
    void sendEvent(string category, string action, string label, long value);
    void sendEventScreen(string screenName);
    void setDebugMode(bool isDebug);
    bool getDebugMode();

    
private:
    static AnalyticsAdapter* m_pInstance;
};

#endif /* AnalyticsAdapter_h */
