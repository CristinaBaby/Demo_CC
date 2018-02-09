//
//  BannerAds.cpp
//
//  Created by hujie2 on 15/7/9.
//
//

#include "BannerAds.h"
#include "BannerAdsJNI.h"

BannerAds* BannerAds::m_pInstance = nullptr;



BannerAds::~BannerAds()
{
    
}

BannerAds* BannerAds::getInstance()
{
    if (!m_pInstance)
    {
        m_pInstance = new BannerAds();
    }
    return m_pInstance;
}

void BannerAds::show()
{
    BannerAdsJNI::getInstance()->show();
}

void BannerAds::preload()
{
    BannerAdsJNI::getInstance()->preload();
}

void BannerAds::setAutoShow(bool bIsAuto)
{
    BannerAdsJNI::getInstance()->setAutoShow(bIsAuto);
}

bool BannerAds::isAutoShow()
{
    return BannerAdsJNI::getInstance()->isAutoShow();
}

void BannerAds::remove()
{
    BannerAdsJNI::getInstance()->remove();
}

void BannerAds::setVisible(bool isVisibility)
{
    BannerAdsJNI::getInstance()->setVisible(isVisibility);
}

void BannerAds::setDebugMode(bool bIsDebug)
{
	BannerAdsJNI::getInstance()->setDebugMode(bIsDebug);
}

void BannerAds::setLayout(LAYOUT_TYPE kType)
{
	BannerAdsJNI::getInstance()->setLayout(kType);
}
