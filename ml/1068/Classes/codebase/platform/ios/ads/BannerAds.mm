//
//  BannerAds.cpp
//  NewAdsManager
//
//  Created by zengwenbin on 15/7/9.
//  Copyright (c) 2015年 zengwenbin. All rights reserved.
//

#include <stdio.h>
#if __has_include(<AdsSdk/BannerAdsManager.h>)
#include <AdsSdk/BannerAdsManager.h>
#else
#include "BannerAdsManager.h"
#endif
#include "BannerAds.h"


@interface BannerAdManagerCppAdapter:NSObject<BannerAdsManagerDelegate>
@property(nonatomic,assign)BannerAds *ad;
@end

@implementation BannerAdManagerCppAdapter

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.ad=nil;
        [BannerAdsManager getInstance].delegate=self;
    }
    return self;
}
-(void)onBannerLoaded{
#if __cplusplus > 201100L
    if(self.ad->onBannerLoaded){
        self.ad->onBannerLoaded();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onBannerLoaded();
    }
}
-(void)onBannerFailed:(NSError *)error{
#if __cplusplus > 201100L
    if(self.ad->onBannerLoadFailed){
        self.ad->onBannerLoadFailed("");
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onBannerLoadFailed("");
    }
}
-(void)onBannerExpanded{
#if __cplusplus > 201100L
    if(self.ad->onBannerExpanded){
        self.ad->onBannerExpanded();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onBannerExpanded();
    }
    
}
-(void)onBannerCollapsed{
#if __cplusplus > 201100L
    if(self.ad->onBannerCollapsed){
        self.ad->onBannerCollapsed();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onBannerCollapsed();
    }
    
}
-(void)onBannerClicked{
#if __cplusplus > 201100L
    if(self.ad->onBannerClicked){
        self.ad->onBannerClicked();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onBannerClicked();
    }
}
@end
#if __cplusplus > 201100L
BannerAds * BannerAds::m_pInstance=nullptr;
#else
BannerAds * BannerAds::m_pInstance=NULL;
#endif

BannerAds::~BannerAds(){
    
}
BannerAds* BannerAds::getInstance(){
    if(!m_pInstance){
        m_pInstance=new BannerAds();
        BannerAdManagerCppAdapter *adapter=[[BannerAdManagerCppAdapter alloc] init];
        adapter.ad=m_pInstance;
    }
    return m_pInstance;
}
void BannerAds::preload(){
    [[BannerAdsManager getInstance] preload];
}

void BannerAds::show(){
    return [[BannerAdsManager getInstance] show];
}

void BannerAds::setAutoShow(bool bIsAuto){
    [BannerAdsManager getInstance].autoShow=bIsAuto;
}

bool BannerAds::isAutoShow(){
    return [BannerAdsManager getInstance].autoShow;
}

void BannerAds::setVisible(bool bVisibility){
    [BannerAdsManager getInstance].visible=bVisibility;
}
void BannerAds::remove(){
    [[BannerAdsManager getInstance] removeAd];
}
void BannerAds::setDebugMode(bool bIsDebug){
    [BannerAdsManager getInstance].isDebugModel=bIsDebug;
}
void BannerAds::setLayout(LAYOUT_TYPE kType){
    BannerAdsPostion pos=BannerAdsPostionCustom;
    switch (kType) {
        case kLayoutTopLeft:
            pos=BannerAdsPostionTopLeft;
            break;
        case kLayoutCenterLeft:
            pos=BannerAdsPostionCenterLeft;
            break;
        case kLayoutBottomLeft:
            pos=BannerAdsPostionBottomLeft;
            break;
            
        case kLayoutTopCenter:
            pos=BannerAdsPostionTopCenter;
            break;
        case kLayoutCenter:
            pos=BannerAdsPostionCenterCenter;
            break;
        case kLayoutBottomCenter:
            pos=BannerAdsPostionBottomCenter;
            break;
            
        case kLayoutTopRight:
            pos=BannerAdsPostionTopRight;
            break;
        case kLayoutCenterRight:
            pos=BannerAdsPostionCenterRight;
            break;
        case kLayoutBottomRight:
            pos=BannerAdsPostionBottomRight;
            break;
        default:
            break;
    }
    [[BannerAdsManager getInstance] setPositionQuick:pos];
}