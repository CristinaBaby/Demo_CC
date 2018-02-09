//
//  AdHelper.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-25.
//
//

#include "AdHelper.h"
#include "PurchaseHelper.h"
#include "../Spot/SpotScene.h"
#include "../layer/AdsLoadingLayer.h"

USING_NS_CC;

static AdHelper* sp_AdHelper = 0;

AdHelper* AdHelper::getInstance()
{
	if (!sp_AdHelper)
	{
		sp_AdHelper = new AdHelper();
	}
    
	return sp_AdHelper;
}

void AdHelper::destroy()
{
	if (sp_AdHelper)
	{
		delete sp_AdHelper;
        
		sp_AdHelper = 0;
	}
}

void AdHelper::onBannerLoadSuccessfully()
{
}

void AdHelper::onBannerLoadFailed(int errorCode)
{
}

void AdHelper::onBannerClicked()
{
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    auto scene = Director::getInstance()->getRunningScene();
    auto layer = dynamic_cast<SpotScene*>(scene->getChildByName("SpotScene"));
    
    if (nullptr != layer)
    {
        layer->onPauseCallback(nullptr);
    }
    
#endif
    
}

void AdHelper::onBannerDismissed()
{
}

void AdHelper::onInterstitialAdLoaded()
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    auto scene = Director::getInstance()->getRunningScene();
    auto layer = dynamic_cast<SpotScene*>(scene->getChildByName("SpotScene"));
    
    if (nullptr != layer)
    {
        layer->onPauseCallback(nullptr);
    }
    
#endif
    
}

void AdHelper::onInterstitialAdFailed(int errorCode)
{
}

void AdHelper::onInterstitialShown()
{
}

void AdHelper::onInterstitialDismissed()
{
}


// --------------------------------------------

void AdHelper::requestInterstitialAds()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (PurchaseHelper::isAdPurchased) return;
    
    if (AdsLoadingLayer::s_currentInstance == nullptr)
    {
        auto adsLoadingLayer = AdsLoadingLayer::create();
        Director::getInstance()->getRunningScene()->addChild(adsLoadingLayer, 100);
    }
    else
    {
        AdsLoadingLayer::s_currentInstance->loadAd();
    }
#endif
}

void AdHelper::requestAds()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (PurchaseHelper::isAdPurchased) return;
    
    STAds* ad = new STAds();
    ad->requestAds();
    ad->setAdsDelegate(AdHelper::getInstance());
    CC_SAFE_DELETE(ad);
#endif
}

void AdHelper::removeAds()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    STAds* ad = new STAds();
    ad->removeAds();
    CC_SAFE_DELETE(ad);
#endif
}


