//
//  SSCNativeAds.cpp
//
//  Created by hujie2 on 15/7/9.
//
//

#include "SSCNativeAds.h"
#include "NativeBannerAdsJNI.h"
#include <iostream>

SSCNativeAds* SSCNativeAds::m_pInstance = nullptr;



SSCNativeAds::~SSCNativeAds()
{

}

SSCNativeAds* SSCNativeAds::getInstance()
{
    if (!m_pInstance)
    {
        m_pInstance = new SSCNativeAds();
    }
    return m_pInstance;
}

void SSCNativeAds::show()
{
	NativeBannerAdsJNI::getInstance()->show();
}

void SSCNativeAds::preload()
{
    
//    cout<<"ssssssssssssssssss";
    std::cout<<"SSCNativeAds"<<std::endl;
	NativeBannerAdsJNI::getInstance()->preload();
}

void SSCNativeAds::setAutoShow(bool bIsAuto)
{
	NativeBannerAdsJNI::getInstance()->setAutoShow(bIsAuto);
}

bool SSCNativeAds::isAutoShow()
{
    return NativeBannerAdsJNI::getInstance()->isAutoShow();
}

void SSCNativeAds::remove()
{
	NativeBannerAdsJNI::getInstance()->remove();
}

void SSCNativeAds::setVisible(bool isVisibility)
{
	NativeBannerAdsJNI::getInstance()->setVisible(isVisibility);
}

void SSCNativeAds::setDebugMode(bool bIsDebug)
{
	NativeBannerAdsJNI::getInstance()->setDebugMode(bIsDebug);
}

void SSCNativeAds::setLayout(NativeAdsLayoutType kType)
{
	NativeBannerAdsJNI::getInstance()->setLayout(kType);
}
