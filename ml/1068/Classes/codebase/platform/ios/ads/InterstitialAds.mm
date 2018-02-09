//
//  InterstitialAds.cpp
//  NewAdsManager
//
//  Created by zengwenbin on 15/7/9.
//  Copyright (c) 2015年 zengwenbin. All rights reserved.
//

#include <stdio.h>
#if __has_include(<AdsSdk/InterstitialAdsManager.h>)
#include <AdsSdk/InterstitialAdsManager.h>
#else
#include "InterstitialAdsManager.h"
#endif
#include "InterstitialAds.h"
@interface InterstitialAdManagerCppAdapter:NSObject<InterstitialAdsManagerDelegate>
@property(nonatomic,assign)InterstitialAds *ad;
@end

@implementation InterstitialAdManagerCppAdapter

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.ad=nil;
        [InterstitialAdsManager getInstance].delegate=self;
    }
    return self;
}
-(void)onInterstitialLoaded{
#if __cplusplus > 201100L
    if(self.ad->onInterstitialLoaded){
        self.ad->onInterstitialLoaded();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onInterstitialLoaded();
    }
}
-(void)onInterstitialFailed:(NSError *)error{
#if __cplusplus > 201100L
    if(self.ad->onInterstitialFailed){
        self.ad->onInterstitialFailed("");
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onInterstitialFailed("");
    }
}
-(void)onInterstitialExpanded{
#if __cplusplus > 201100L
    if(self.ad->onInterstitialExpanded){
        self.ad->onInterstitialExpanded();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onInterstitialExpanded();
    }
    
}
-(void)onInterstitialCollapsed{
#if __cplusplus > 201100L
    if(self.ad->onInterstitialCollapsed){
        self.ad->onInterstitialCollapsed();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onInterstitialCollapsed();
    }
    
}
-(void)onInterstitialClicked{
#if __cplusplus > 201100L
    if(self.ad->onInterstitialClicked){
        self.ad->onInterstitialClicked();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onInterstitialClicked();
    }
}
@end
#if __cplusplus > 201100L
InterstitialAds * InterstitialAds::m_pInstance=nullptr;
#else
InterstitialAds * InterstitialAds::m_pInstance=NULL;
#endif

InterstitialAds::~InterstitialAds(){
    
}
InterstitialAds* InterstitialAds::getInstance(){
    if(!m_pInstance){
        m_pInstance=new InterstitialAds();
        InterstitialAdManagerCppAdapter *adapter=[[InterstitialAdManagerCppAdapter alloc] init];
        adapter.ad=m_pInstance;
    }
    return m_pInstance;
}
void InterstitialAds::preload(){
    [[InterstitialAdsManager getInstance] preload];
}
bool InterstitialAds::isPreloaded(){
    return [InterstitialAdsManager getInstance].isPreloaded;
}
bool InterstitialAds::show(){
    return [[InterstitialAdsManager getInstance] show];
}

void InterstitialAds::setAutoShow(bool bIsAuto){
    [InterstitialAdsManager getInstance].autoShow=bIsAuto;
}

bool InterstitialAds::isAutoShow(){
    return [InterstitialAdsManager getInstance].autoShow;
}
void InterstitialAds::setDebugMode(bool bIsDebug){
    [InterstitialAdsManager getInstance].isDebugModel=bIsDebug;
}

