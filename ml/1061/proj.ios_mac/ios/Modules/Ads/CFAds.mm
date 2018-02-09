//
//  CFAds.cpp
//  MysteryGuardians
//
//  Created by Steven.Xc.Tian on 13-7-3.
//
//

#include "cocos2d.h"
#include "CFAds.h"
#include "AdsManager_IOS.h"
#include "AdsPopupManager.h"

bool CFAds::willShowAd = true;
bool CFAds::interstitialShown = false;

void CFAds::requestAds()
{
    if(IAPManager::getInstance()->isShowAds())
    {
        [[AdsIOSManager sharedManager] requestAds];
    }
}

void CFAds::removeAds()
{
    [[AdsIOSManager sharedManager] removeAds];
}

void CFAds::requestInterstitialAds(InterstitialType type)
{
    if(IAPManager::getInstance()->isShowAds())
    {
        [[AdsIOSManager sharedManager] requestInterstitialAds:type];
    }
}

void CFAds::setAdsDelegate(CFAdsDelegate* delegate)
{
    [[AdsIOSManager sharedManager] setCFAdsDelegate:delegate];
}

void CFAds::setAdsVisibility(bool visibility)
{
    visibility ?
    [[AdsIOSManager sharedManager] setAdsVisibility:YES] :
    [[AdsIOSManager sharedManager] setAdsVisibility:NO];
}

void CFAds::destroy()
{
    [AdsIOSManager purgeManager];
}

void CFAds::getBannerSize(float  size[])
{
    size[0]=  [[AdsPopupManager sharedManager] bannerHeight];
    NSLog(@"ios--------->%f",[[AdsPopupManager sharedManager] bannerHeight]);
}

CFAds::CFAds()
{
}

CFAds::~CFAds()
{
}
