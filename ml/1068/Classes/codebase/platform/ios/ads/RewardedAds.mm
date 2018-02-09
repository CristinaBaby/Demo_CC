//
//  RewardedAds.cpp
//  NewAdsManager
//
//  Created by zengwenbin on 15/7/9.
//  Copyright (c) 2015年 zengwenbin. All rights reserved.
//

#include <stdio.h>

#if __has_include(<AdsSdk/RewardedAdsManager.h>)
#include <AdsSdk/RewardedAdsManager.h>
#else
#include "RewardedAdsManager.h"
#endif
#include "RewardedAds.h"
@interface RewardedAdManagerCppAdapter:NSObject<RewardedAdsManagerDelegate>
@property(nonatomic,assign)RewardedAds *ad;
@end

@implementation RewardedAdManagerCppAdapter

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.ad=nil;
        [RewardedAdsManager getInstance].delegate=self;
    }
    return self;
}
-(void)onRewardedLoaded{
#if __cplusplus > 201100L
    if(self.ad->onRewardedLoaded){
        self.ad->onRewardedLoaded();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onRewardedLoaded();
    }
}
-(void)onRewardedFailed:(NSError *)error{
#if __cplusplus > 201100L
    if(self.ad->onRewardedFailed){
        self.ad->onRewardedFailed("");
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onRewardedFailed("");
    }
}
-(void)onRewardedExpanded{
#if __cplusplus > 201100L
    if(self.ad->onRewardedExpanded){
        self.ad->onRewardedExpanded();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onRewardedExpanded();
    }
}
-(void)onRewardedCollapsed{
#if __cplusplus > 201100L
    if(self.ad->onRewardedCollapsed){
        self.ad->onRewardedCollapsed();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onRewardedCollapsed();
    }
}
-(void)onRewarded:(NSString *)rewardedItem rewardedNum:(NSInteger)rewardedNum isSkipped:(BOOL)isSkipped{
    if(!rewardedItem) rewardedItem=@"";
    std::string s_rewardedItem=[rewardedItem cStringUsingEncoding:NSUTF8StringEncoding];
#if __cplusplus > 201100L
    if(self.ad->onRewarded){
        self.ad->onRewarded(s_rewardedItem,(int)rewardedNum,isSkipped);
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onRewarded(s_rewardedItem,(int)rewardedNum,isSkipped);
    }
    
}
-(void)onRewardedClicked{
#if __cplusplus > 201100L
    if(self.ad->onRewardedClicked){
        self.ad->onRewardedClicked();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onRewardedClicked();
    }
}
@end

#if __cplusplus > 201100L
RewardedAds * RewardedAds::m_pInstance=nullptr;
#else
RewardedAds * RewardedAds::m_pInstance=NULL;
#endif

RewardedAds::~RewardedAds(){
    
}
RewardedAds* RewardedAds::getInstance(){
    if(!m_pInstance){
        m_pInstance=new RewardedAds();
        RewardedAdManagerCppAdapter *adapter=[[RewardedAdManagerCppAdapter alloc] init];
        adapter.ad=m_pInstance;
    }
    return m_pInstance;
}
void RewardedAds::preload(){
    [[RewardedAdsManager getInstance] preload];
}
bool RewardedAds::isPreloaded(){
    return [RewardedAdsManager getInstance].isPreloaded;
}
bool RewardedAds::show(){
    return [[RewardedAdsManager getInstance] show];
}

void RewardedAds::setAutoShow(bool autoShow){
    [RewardedAdsManager getInstance].autoShow=autoShow;
}

bool RewardedAds::isAutoShow(){
    return [RewardedAdsManager getInstance].autoShow;
}
void RewardedAds::setDebugMode(bool bIsDebug){
    [RewardedAdsManager getInstance].isDebugModel=bIsDebug;
}

