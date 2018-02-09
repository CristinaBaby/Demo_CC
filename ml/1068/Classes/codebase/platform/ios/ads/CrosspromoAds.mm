//
//  CrosspromoAds.cpp
//  NewAdsManager
//
//  Created by zengwenbin on 15/7/9.
//  Copyright (c) 2015年 zengwenbin. All rights reserved.
//

#include <stdio.h>
#if __has_include(<AdsSdk/CrosspromoAdsManager.h>)
#include <AdsSdk/CrosspromoAdsManager.h>
#else
#include "CrosspromoAdsManager.h"
#endif
#include "CrosspromoAds.h"
@interface CrosspromoAdManagerCppAdapter:NSObject<CrosspromoAdsManagerDelegate>
@property(nonatomic,assign)CrosspromoAds *ad;
@end

@implementation CrosspromoAdManagerCppAdapter

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.ad=nil;
        [CrosspromoAdsManager getInstance].delegate=self;
    }
    return self;
}
-(void)onCrosspromoLoaded{
#if __cplusplus > 201100L
    if(self.ad->onCrosspromoLoaded){
        self.ad->onCrosspromoLoaded();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onCrosspromoLoaded();
    }
}
-(void)onCrosspromoFailed:(NSError *)error{
#if __cplusplus > 201100L
    if(self.ad->onCrosspromoFailed){
        self.ad->onCrosspromoFailed("");
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onCrosspromoFailed("");
    }
}
-(void)onCrosspromoExpanded{
#if __cplusplus > 201100L
    if(self.ad->onCrosspromoExpanded){
        self.ad->onCrosspromoExpanded();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onCrosspromoExpanded();
    }
}
-(void)onCrosspromoCollapsed{
#if __cplusplus > 201100L
    if(self.ad->onCrosspromoCollapsed){
        self.ad->onCrosspromoCollapsed();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onCrosspromoCollapsed();
    }
    
}
-(void)onCrosspromoClicked{
#if __cplusplus > 201100L
    if(self.ad->onCrosspromoClicked){
        self.ad->onCrosspromoClicked();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onCrosspromoClicked();
    }
}
@end

#if __cplusplus > 201100L
CrosspromoAds * CrosspromoAds::m_pInstance=nullptr;
#else
CrosspromoAds * CrosspromoAds::m_pInstance=NULL;
#endif

CrosspromoAds::~CrosspromoAds(){
    
}
CrosspromoAds* CrosspromoAds::getInstance(){
    if(!m_pInstance){
        m_pInstance=new CrosspromoAds();
        CrosspromoAdManagerCppAdapter *adapter=[[CrosspromoAdManagerCppAdapter alloc] init];
        adapter.ad=m_pInstance;
    }
    return m_pInstance;
}
void CrosspromoAds::preload(){
    [[CrosspromoAdsManager getInstance] preload];
}
bool CrosspromoAds::isPreloaded(){
    return [CrosspromoAdsManager getInstance].isPreloaded;
}
bool CrosspromoAds::show(){
    return [[CrosspromoAdsManager getInstance] show];
}

void CrosspromoAds::setAutoShow(bool autoShow){
    [CrosspromoAdsManager getInstance].autoShow=autoShow;
}

bool CrosspromoAds::isAutoShow(){
    return [CrosspromoAdsManager getInstance].autoShow;
}
void CrosspromoAds::setDebugMode(bool bIsDebug){
    [CrosspromoAdsManager getInstance].isDebugModel=bIsDebug;
}

