

//
//  CrosspromoAds.cpp
//
//  Created by hujie2 on 15/7/9.
//
//

#include "CrosspromoAds.h"
#include "CrosspromoAdsJNI.h"

CrosspromoAds* CrosspromoAds::m_pInstance = nullptr;



CrosspromoAds::~CrosspromoAds()
{
    
}

CrosspromoAds* CrosspromoAds::getInstance()
{
    if (!m_pInstance)
    {
        m_pInstance = new CrosspromoAds();
    }
    return m_pInstance;
}

bool CrosspromoAds::show()
{
    return CrosspromoAdsJNI::getInstance()->show();
}

void CrosspromoAds::preload()
{
    CrosspromoAdsJNI::getInstance()->preload();
}

void CrosspromoAds::setAutoShow(bool bIsAuto)
{
    CrosspromoAdsJNI::getInstance()->setAutoShow(bIsAuto);
}

bool CrosspromoAds::isAutoShow()
{
    return CrosspromoAdsJNI::getInstance()->isAutoShow();
}

bool CrosspromoAds::isPreloaded()
{
    return CrosspromoAdsJNI::getInstance()->isPreloaded();
}

void CrosspromoAds::setDebugMode(bool bIsDebug)
{
	CrosspromoAdsJNI::getInstance()->setDebugMode(bIsDebug);
}
