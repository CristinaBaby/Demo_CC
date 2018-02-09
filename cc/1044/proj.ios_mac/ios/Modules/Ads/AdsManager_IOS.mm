//
//  AdsIOSWrapper.m
//  MysteryGuardians
//
//  Created by Steven.Tian on 13-7-22.
//
//

#import "AdsManager_IOS.h"
#import "AppController.h"
#import "RootViewController.h"
#import "AdsPopupManager.h"

static AdsIOSManager *s_instance = nil;

@implementation AdsIOSManager

+(AdsIOSManager*)sharedManager
{
    if(s_instance==nil)
    {
        s_instance=[[AdsIOSManager alloc] init];
        [AdsPopupManager sharedManager].delegate=s_instance;
    }
    return s_instance;
}

+(void)purgeManager
{
    if (s_instance)
    {
        [s_instance release],s_instance = nil;
    }
}

-(void)dealloc
{
    stAdsDelegate_=0;
    s_instance=0;
    [super dealloc];
}

-(void)setCFAdsDelegate:(CFAdsDelegate*)delegate
{
    stAdsDelegate_ = delegate;
}

-(void)requestAds
{
    RootViewController *vc=[AppController sharedAppController].viewController;
    [[AdsPopupManager sharedManager] setViewController:vc];
    [[AdsPopupManager sharedManager] showBannerAd];
}

-(void)removeAds
{
    [[AdsPopupManager sharedManager] hideBannerAd];
}

-(void)setAdsVisibility:(BOOL)visibility
{
    if (adView_)
    {
        visibility ?
        [adView_ setHidden:NO] : [adView_ setHidden:YES];
    }
}

//banner ad 点击处理
- (UIViewController *)viewControllerForPresentingModalView
{
    UIViewController *vc=[AppController sharedAppController].viewController;
    return vc;
}

- (void)bannerAdDidLoad{
    cocos2d::CCNotificationCenter::getInstance()->postNotification("bannerAdDidLoad");
}

- (void)interstitialDidShow{
    NSLog(@"full ads load--------------------");
    cocos2d::CCNotificationCenter::getInstance()->postNotification("interstitialDidShow");
}
- (void)willPresentModalViewForBanner{
    NSLog(@"full ads load--------------------");
    cocos2d::CCNotificationCenter::getInstance()->postNotification("interstitialDidShow");
}

- (void)interstitialDidDismiss{
    NSLog(@"full ads close--------------------");
    cocos2d::CCNotificationCenter::getInstance()->postNotification("interstitialDidmissed");
}




//全屏广告-(void)requestInterstitialAds:(InterstitialType)type
-(void)requestInterstitialAds:(InterstitialType)type
{
    if (CFAds::willShowAd)
    {
        if (type == TypeDefault)
        {
            [[AdsPopupManager sharedManager]showInterstitialForType:InterstitialAdTypeDefault];
        }else if(type == TypeCrossPromo)
        {
            [[AdsPopupManager sharedManager]showInterstitialForType:InterstitialAdTypeCrossPromo];
        }
    }
    else
    {
        if (adController_)
        {
            [adController_ release], adController_=nil;
        }
    }
}

@end
