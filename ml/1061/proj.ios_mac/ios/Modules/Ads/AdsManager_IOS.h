//
//  AdsIOSWrapper.h
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-22.
//
//

#import <Foundation/Foundation.h>
#import "MPAdView.h"
#import "MPInterstitialAdController.h"
#import "AdsPopupManager.h"
#include "STAds.h"

@interface AdsIOSManager : NSObject<AdsPopupManagerDelegate>
{
    CFAdsDelegate* stAdsDelegate_;
    
    //广告视图
    MPAdView *adView_;
    MPInterstitialAdController *adController_;
}

+(AdsIOSManager*)sharedManager;
+(void)purgeManager;

-(void)setCFAdsDelegate:(CFAdsDelegate*)delegate;

-(void)requestAds;
-(void)removeAds;
-(void)requestInterstitialAds:(InterstitialType)type;
-(void)setAdsVisibility:(BOOL)visibility;

@end
