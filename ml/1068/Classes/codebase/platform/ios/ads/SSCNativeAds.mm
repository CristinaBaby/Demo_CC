//
//  SSCNativeAds.m
//  AdsDemo
//
//  Created by xuzepei on 16/1/6.
//
//

#import "SSCNativeAds.h"

#include <stdio.h>
#if __has_include(<AdsSdk/NativeAdsManager.h>)
#include <AdsSdk/NativeAdsManager.h>
#else
#include "NativeAdsManager.h"
#endif
#include "SSCNativeAds.h"


@interface NativeAdsManagerCppAdapter:NSObject<NativeAdsDelegate>
@property(nonatomic,assign)SSCNativeAds *ad;
@end

@implementation NativeAdsManagerCppAdapter

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.ad=nil;
        NativeAdsManager* manager = [NativeAdsManager getInstance];
        manager.delegate=self;
    }
    return self;
}

- (void)onLoaded
{
#if __cplusplus > 201100L
    if(self.ad->onNativeAdsLoaded){
        self.ad->onNativeAdsLoaded();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onNativeAdsLoaded();
    }
}

- (void)onFailed:(NSError *)error
{
#if __cplusplus > 201100L
    if(self.ad->onNativeAdsLoadFailed){
        self.ad->onNativeAdsLoadFailed("");
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onNativeAdsLoadFailed("");
    }
}

- (void)onClicked
{
#if __cplusplus > 201100L
    if(self.ad->onNativeAdsClicked){
        self.ad->onNativeAdsClicked();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onNativeAdsClicked();
    }
}

- (void)onExpanded
{
#if __cplusplus > 201100L
    if(self.ad->onNativeAdsExpanded){
        self.ad->onNativeAdsExpanded();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onNativeAdsExpanded();
    }
}

- (void)onCollapsed
{
#if __cplusplus > 201100L
    if(self.ad->onNativeAdsCollapsed){
        self.ad->onNativeAdsCollapsed();
    }
#endif
    if(self.ad->delegate){
        self.ad->delegate->onNativeAdsCollapsed();
    }
}

@end


#if __cplusplus > 201100L
SSCNativeAds * SSCNativeAds::m_pInstance=nullptr;
#else
SSCNativeAds * SSCNativeAds::m_pInstance=NULL;
#endif

SSCNativeAds::~SSCNativeAds(){
    
}

SSCNativeAds* SSCNativeAds::getInstance(){
    if(!m_pInstance){
        m_pInstance = new SSCNativeAds();
        NativeAdsManagerCppAdapter *adapter = [[NativeAdsManagerCppAdapter alloc] init];
        adapter.ad=m_pInstance;
    }
    return m_pInstance;
}

void SSCNativeAds::preload(){
    [[NativeAdsManager getInstance] preload];
}

void SSCNativeAds::show(){
    return [[NativeAdsManager getInstance] show];
}

void SSCNativeAds::setAutoShow(bool bIsAuto){
    //[NativeAdsManager getInstance].autoShow=bIsAuto;
    
    NSLog(@"No implement.");
}

bool SSCNativeAds::isAutoShow(){
    return NO;//[NativeAdsManager getInstance].autoShow;
}

void SSCNativeAds::setVisible(bool bVisibility){
    [[NativeAdsManager getInstance] setHidden:!bVisibility];
}

void SSCNativeAds::remove(){
    [[NativeAdsManager getInstance] remove];
}

void SSCNativeAds::setDebugMode(bool bIsDebug){
    [[NativeAdsManager getInstance] setIsDebugModel:bIsDebug];
}

void SSCNativeAds::setLayout(NativeAdsLayoutType kType){
    [[NativeAdsManager getInstance] layoutByType:kType];
}
