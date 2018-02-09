

//
//  InterstitialAds.cpp
//
//  Created by hujie2 on 15/7/9.
//
//

#include "InterstitialAds.h"
#include "InterstitialAdsJNI.h"

InterstitialAds* InterstitialAds::m_pInstance = nullptr;



InterstitialAds::~InterstitialAds()
{
    
}

InterstitialAds* InterstitialAds::getInstance()
{
    if (!m_pInstance)
    {
        m_pInstance = new InterstitialAds();
    }
    return m_pInstance;
}

bool InterstitialAds::show()
{
    return InterstitialAdsJNI::getInstance()->show();
}

void InterstitialAds::preload()
{
    InterstitialAdsJNI::getInstance()->preload();
}

void InterstitialAds::setAutoShow(bool bIsAuto)
{
    InterstitialAdsJNI::getInstance()->setAutoShow(bIsAuto);
}

bool InterstitialAds::isAutoShow()
{
    return InterstitialAdsJNI::getInstance()->isAutoShow();
}

bool InterstitialAds::isPreloaded()
{
    return InterstitialAdsJNI::getInstance()->isPreloaded();
}

void InterstitialAds::setDebugMode(bool bIsDebug)
{
	InterstitialAdsJNI::getInstance()->setDebugMode(bIsDebug);
}
