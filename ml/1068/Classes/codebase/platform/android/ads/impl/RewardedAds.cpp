

//
//  RewardedAds.cpp
//
//  Created by hujie2 on 15/7/9.
//
//

#include "RewardedAds.h"
#include "RewardedAdsJNI.h"

RewardedAds* RewardedAds::m_pInstance = nullptr;



RewardedAds::~RewardedAds()
{
    
}

RewardedAds* RewardedAds::getInstance()
{
    if (!m_pInstance)
    {
        m_pInstance = new RewardedAds();
    }
    return m_pInstance;
}

bool RewardedAds::show()
{
    return RewardedAdsJNI::getInstance()->show();
}

void RewardedAds::preload()
{
    RewardedAdsJNI::getInstance()->preload();
}

void RewardedAds::setAutoShow(bool bIsAuto)
{
    RewardedAdsJNI::getInstance()->setAutoShow(bIsAuto);
}

bool RewardedAds::isAutoShow()
{
    return RewardedAdsJNI::getInstance()->isAutoShow();
}

bool RewardedAds::isPreloaded()
{
    return RewardedAdsJNI::getInstance()->isPreloaded();
}

void RewardedAds::setDebugMode(bool bIsDebug)
{
	RewardedAdsJNI::getInstance()->setDebugMode(bIsDebug);
}
