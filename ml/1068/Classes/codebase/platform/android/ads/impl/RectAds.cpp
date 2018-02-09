//
//  RectAds.cpp
//  Ads_Framework
//
//  Created by hujie2 on 15/7/10.
//
//

#include "RectAds.h"

//
//  RectAds.cpp
//
//  Created by hujie2 on 15/7/9.
//
//

#include "RectAds.h"
#include "RectAdsJNI.h"

RectAds* RectAds::m_pInstance = nullptr;



RectAds::~RectAds()
{
    
}

RectAds* RectAds::getInstance()
{
    if (!m_pInstance)
    {
        m_pInstance = new RectAds();
    }
    return m_pInstance;
}

void RectAds::show()
{
    RectAdsJNI::getInstance()->show();
}

void RectAds::preload()
{
    RectAdsJNI::getInstance()->preload();
}

void RectAds::setAutoShow(bool bIsAuto)
{
    RectAdsJNI::getInstance()->setAutoShow(bIsAuto);
}

bool RectAds::isAutoShow()
{
    return RectAdsJNI::getInstance()->isAutoShow();
}

void RectAds::remove()
{
    RectAdsJNI::getInstance()->remove();
}

void RectAds::setVisible(bool isVisibility)
{
    RectAdsJNI::getInstance()->setVisible(isVisibility);
}

void RectAds::setDebugMode(bool bIsDebug)
{
	RectAdsJNI::getInstance()->setDebugMode(bIsDebug);
}
