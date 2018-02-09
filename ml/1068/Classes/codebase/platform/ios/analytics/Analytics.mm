//
//  Analytics.cpp


#include "Analytics.h"
#include "AnalyticsAdapter.h"

Analytics* Analytics::m_pInstance = NULL;

Analytics::Analytics()
{
    m_pInstance = NULL;
}

Analytics* Analytics::getInstance()
{
    if(!m_pInstance)
    {
        m_pInstance = new Analytics();
    }
    
    return m_pInstance;
}

void Analytics::sendEvent(std::string eventName, std::string value)
{
    AnalyticsAdapter::getInstance()->sendEvent(eventName, value, "UserEvent");
}


void Analytics::sendEvent(std::string eventName, std::map<std::string , std::string> mapValue)
{
    AnalyticsAdapter::getInstance()->sendEvent(eventName, mapValue, "UserEvent");
}

void Analytics::sendEvent(std::string category, std::string action, std::string label, long value)
{
    AnalyticsAdapter::getInstance()->sendEvent(category,action,label,value);
}

void Analytics::sendScreenEvent(std::string screenName)
{
    AnalyticsAdapter::getInstance()->sendEventScreen(screenName);
}

void Analytics::setDebugMode(bool isDebug)
{
    AnalyticsAdapter::getInstance()->setDebugMode(isDebug);
}

bool Analytics::getDebugMode()
{
    return AnalyticsAdapter::getInstance()->getDebugMode();
}

void Analytics::endSession()
{
    return AnalyticsAdapter::getInstance()->endSession();
}