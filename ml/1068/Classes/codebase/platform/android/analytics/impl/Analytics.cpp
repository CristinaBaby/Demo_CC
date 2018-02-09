//
//  Analytics.cpp
//  MPM4121
//
//  Created by hujie2 on 15/11/2.
//
//

#include "Analytics.h"
#include "AnalyticsJNI.h"
Analytics* Analytics::m_pInstance = NULL;

Analytics::Analytics()
{
    
}

Analytics::~Analytics()
{
    
}

Analytics* Analytics::getInstance()
{
    if (!m_pInstance) {
        m_pInstance = new Analytics();
    }
    
    return m_pInstance;
}

void Analytics::sendEvent(std::string eventName, std::string value)
{
    AnalyticsJNI::getInstance()->sendEvent(eventName, value);
}

void Analytics::sendEvent(std::string eventName, std::map<std::string, std::string> mapValue)
{
    AnalyticsJNI::getInstance()->sendEvent(eventName, mapValue);
}

void Analytics::sendScreenEvent(std::string sreenName)
{
    AnalyticsJNI::getInstance()->sendScreenEvent(sreenName);
}

void Analytics::sendEvent(std::string category, std::string action, std::string label, long value)
{
    AnalyticsJNI::getInstance()->sendEvent(category, action, label, value);
}

void Analytics::setDebugMode(bool bIsDebug)
{
    AnalyticsJNI::getInstance()->setDebugMode(bIsDebug);
}

bool Analytics::getDebugMode()
{
	return AnalyticsJNI::getInstance()->getDebugMode();
}

void Analytics::endSession()
{
    AnalyticsJNI::getInstance()->endSession();
}
