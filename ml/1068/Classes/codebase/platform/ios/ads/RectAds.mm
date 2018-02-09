//
//  RectAds.cpp
//  NewAdsManager
//
//  Created by zengwenbin on 15/7/9.
//  Copyright (c) 2015年 zengwenbin. All rights reserved.
//

#include <stdio.h>
#if __has_include(<AdsSdk/RectAdsManager.h>)
#include <AdsSdk/RectAdsManager.h>
#else
#include "RectAdsManager.h"
#endif
#include "RectAds.h"
@interface RectAdManagerCppAdapter:NSObject<RectAdsManagerDelegate>
@property(nonatomic,assign)RectAds *ad;
@end

@implementation RectAdManagerCppAdapter

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.ad=nil;
        [RectAdsManager getInstance].delegate=self;
    }
    return self;
}
-(void)onRectLoaded{
#if __cplusplus > 201100L
    if(self.ad->onRectLoaded){
        self.ad->onRectLoaded();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onRectLoaded();
    }
}
-(void)onRectFailed:(NSError *)error{
#if __cplusplus > 201100L
    if(self.ad->onRectLoadFailed){
        self.ad->onRectLoadFailed("");
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onRectLoadFailed("");
    }
}
-(void)onRectExpanded{
#if __cplusplus > 201100L
    if(self.ad->onRectExpanded){
        self.ad->onRectExpanded();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onRectExpanded();
    }
    
}
-(void)onRectCollapsed{
#if __cplusplus > 201100L
    if(self.ad->onRectCollapsed){
        self.ad->onRectCollapsed();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onRectCollapsed();
    }
    
}
-(void)onRectClicked{
#if __cplusplus > 201100L
    if(self.ad->onRectClicked){
        self.ad->onRectClicked();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onRectClicked();
    }
    
}
@end
#if __cplusplus > 201100L
RectAds * RectAds::m_pInstance=nullptr;
#else
RectAds * RectAds::m_pInstance=NULL;
#endif


RectAds::~RectAds(){
    
}
RectAds* RectAds::getInstance(){
    if(!m_pInstance){
        m_pInstance=new RectAds();
        RectAdManagerCppAdapter *adapter=[[RectAdManagerCppAdapter alloc] init];
        adapter.ad=m_pInstance;
    }
    return m_pInstance;
}
void RectAds::preload(){
    [[RectAdsManager getInstance] preload];
}

void RectAds::show(){
    return [[RectAdsManager getInstance] show];
}

void RectAds::setAutoShow(bool bIsAuto){
    [RectAdsManager getInstance].autoShow=bIsAuto;
}

bool RectAds::isAutoShow(){
    return [RectAdsManager getInstance].autoShow;
}

void RectAds::setVisible(bool bVisibility){
    [RectAdsManager getInstance].visible=bVisibility;
}
void RectAds::remove(){
    [[RectAdsManager getInstance] removeAd];
}

void RectAds::setDebugMode(bool bIsDebug){
    [RectAdsManager getInstance].isDebugModel=bIsDebug;
}
