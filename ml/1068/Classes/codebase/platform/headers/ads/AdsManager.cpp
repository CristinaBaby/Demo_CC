//
//  AdsManager.cpp
//
//  Created by hujie2 on 15/7/14.
//
//

#include "AdsManager.h"

#if __cplusplus > 201100L
AdsManager* AdsManager::m_pInstance = nullptr;
#else
AdsManager* AdsManager::m_pInstance = NULL;
#endif
AdsManager::AdsManager()
#if __cplusplus > 201100L
:onAdsLoaded(nullptr)
,onAdsLoadFailed(nullptr)
,onAdsClicked(nullptr)
,onAdsExpanded(nullptr)
,onAdsCollapsed(nullptr)
,onAdsRewarded(nullptr)
,delegate(nullptr)
#else
:delegate(NULL)
#endif
,m_iFullScreenAdsShowingTag(0)
,m_iPauseResumeTagBanner(0)
,m_iPauseResumeTagRect(0)
,m_bVisibleFlagBannber(false)
,m_bVisibleFlagRect(false)
{
    initBannerAdsDelegate();
    initInterstitialAdsDelegate();
    initCrosspromoAdsDelegate();
    initRectAdsDelegate();
    initRewardedAdsDelegate();
    initNativeAdsDelegate();
}

AdsManager::~AdsManager()

{
    
}

AdsManager* AdsManager::getInstance()
{
    if(!m_pInstance)
    {
        m_pInstance = new AdsManager();
    }
    
    return m_pInstance;
}

void AdsManager::preloadAllAds()
{
    BannerAds::getInstance()->preload();
    BannerAds::getInstance()->setAutoShow(false);
    RectAds::getInstance()->preload();
    RectAds::getInstance()->setAutoShow(false);
    InterstitialAds::getInstance()->preload();
    CrosspromoAds::getInstance()->preload();
    RewardedAds::getInstance()->preload();
    SSCNativeAds::getInstance()->preload();
    
}
void AdsManager::preloadAds(ADS_TYPE adType){
    switch (adType) {
        case kTypeBannerAds:
            BannerAds::getInstance()->preload();
            break;
        case kTypeRectAds:
            RectAds::getInstance()->preload();
            break;
        case kTypeInterstitialAds:
            InterstitialAds::getInstance()->preload();
            break;
        case kTypeCrosspromoAds:
            CrosspromoAds::getInstance()->preload();
            break;
        case kTypeRewardedAds:
            RewardedAds::getInstance()->preload();
            break;
        case kTypeNativeAds:
            SSCNativeAds::getInstance()->preload();
            break;
        default:
            break;
    }
}
bool AdsManager::showAds(ADS_TYPE adType){
    switch (adType) {
        case kTypeBannerAds:
            BannerAds::getInstance()->show();
            m_bVisibleFlagBannber = true;
            m_iPauseResumeTagBanner = 0;
            return true;
            break;
        case kTypeRectAds:
            RectAds::getInstance()->show();
            m_bVisibleFlagRect= true;
            m_iPauseResumeTagRect = 0;
            return true;
            break;
        case kTypeInterstitialAds:
            return InterstitialAds::getInstance()->show();
            break;
        case kTypeCrosspromoAds:
            return CrosspromoAds::getInstance()->show();
            break;
        case kTypeRewardedAds:
            return RewardedAds::getInstance()->show();
            break;
        case kTypeNativeAds:
            SSCNativeAds::getInstance()->show();
            return true;
            break;
        default:
            break;
    }
}
void AdsManager::removeAds(ADS_TYPE adType){
    switch (adType) {
        case kTypeBannerAds:
            BannerAds::getInstance()->remove();
            m_bVisibleFlagBannber = false;
            break;
        case kTypeRectAds:
            RectAds::getInstance()->remove();
            m_bVisibleFlagRect = false;
            break;
        case kTypeNativeAds:
            SSCNativeAds::getInstance()->remove();
            break;
        default:
            break;
    }
    
    
}
void AdsManager::setVisiable(ADS_TYPE adType, bool visiable){
    switch (adType) {
        case kTypeBannerAds:
            BannerAds::getInstance()->setVisible(visiable);
            
            break;
        case kTypeRectAds:
            RectAds::getInstance()->setVisible(visiable);
            
            break;
        case kTypeNativeAds:
            SSCNativeAds::getInstance()->setVisible(visiable);
            break;
        default:
            break;
    }
    
}

void AdsManager::pauseAds(ADS_TYPE adType)
{
    
    switch (adType) {
        case kTypeBannerAds:
            m_iPauseResumeTagBanner ++ ;
            BannerAds::getInstance()->setVisible(false);
            break;
        case kTypeRectAds:
            m_iPauseResumeTagRect ++ ;
            RectAds::getInstance()->setVisible(false);
            break;
        default:
            break;
    }
}

void AdsManager::resumeAds(ADS_TYPE adType)
{
   
    switch (adType) {
        case kTypeBannerAds:
            m_iPauseResumeTagBanner --;
            if (m_iPauseResumeTagBanner>0)
                return;
            BannerAds::getInstance()->setVisible(m_bVisibleFlagBannber);
            break;
        case kTypeRectAds:
            m_iPauseResumeTagRect --;
            if (m_iPauseResumeTagRect > 0)
                return;
            RectAds::getInstance()->setVisible(m_bVisibleFlagRect);
            break;
        default:
            break;
    }
}


void AdsManager::setDebugMode(bool bIsDebug)
{
    BannerAds::getInstance()->setDebugMode(bIsDebug);
    InterstitialAds::getInstance()->setDebugMode(bIsDebug);
    CrosspromoAds::getInstance()->setDebugMode(bIsDebug);
    RewardedAds::getInstance()->setDebugMode(bIsDebug);
    RectAds::getInstance()->setDebugMode(bIsDebug);
    SSCNativeAds::getInstance()->setDebugMode(bIsDebug);
}

void AdsManager::setLayoutBanner(LAYOUT_TYPE kType)
{
    BannerAds::getInstance()->setLayout(kType);
}

void AdsManager::initBannerAdsDelegate()
{
    
#if __cplusplus > 201100L
    BannerAds::getInstance()->onBannerLoaded = [&](){if (onAdsLoaded){onAdsLoaded(ADS_TYPE::kTypeBannerAds);}};
    BannerAds::getInstance()->onBannerLoadFailed = [&](string errorCode){ if (onAdsLoadFailed){onAdsLoadFailed(errorCode, ADS_TYPE::kTypeBannerAds);} };
    BannerAds::getInstance()->onBannerClicked= [&](){ if (onAdsClicked){onAdsClicked(ADS_TYPE::kTypeBannerAds);} };
    BannerAds::getInstance()->onBannerCollapsed= [&](){ if (onAdsCollapsed){onAdsCollapsed(ADS_TYPE::kTypeBannerAds);} };
    BannerAds::getInstance()->onBannerExpanded =  [&](){ if (onAdsExpanded){onAdsExpanded(ADS_TYPE::kTypeBannerAds);}};
#endif
    BannerAds::getInstance()->delegate=this;
}


void AdsManager::initInterstitialAdsDelegate()
{
#if __cplusplus > 201100L
    InterstitialAds::getInstance()->onInterstitialLoaded = [&](){ if (onAdsLoaded){onAdsLoaded(ADS_TYPE::kTypeInterstitialAds);} };
    InterstitialAds::getInstance()->onInterstitialFailed = [&](string errorCode){ if (onAdsLoadFailed){onAdsLoadFailed(errorCode, ADS_TYPE::kTypeInterstitialAds);} };
    InterstitialAds::getInstance()->onInterstitialClicked = [&](){ if (onAdsClicked){onAdsClicked(ADS_TYPE::kTypeInterstitialAds);} };
    InterstitialAds::getInstance()->onInterstitialExpanded = [&](){ if (onAdsExpanded){onAdsExpanded(ADS_TYPE::kTypeInterstitialAds);/*m_iFullScreenAdsShowingTag ++;*/} };
    InterstitialAds::getInstance()->onInterstitialCollapsed = [&](){ if (onAdsCollapsed){onAdsCollapsed(ADS_TYPE::kTypeInterstitialAds);/*m_iFullScreenAdsShowingTag --;*/} };
#endif
    InterstitialAds::getInstance()->delegate=this;
}

void AdsManager::initCrosspromoAdsDelegate()
{
#if __cplusplus > 201100L
    CrosspromoAds::getInstance()->onCrosspromoLoaded = [&](){ if (onAdsLoaded){onAdsLoaded(ADS_TYPE::kTypeCrosspromoAds);} };
    CrosspromoAds::getInstance()->onCrosspromoFailed = [&](string errorCode){ if (onAdsLoadFailed){onAdsLoadFailed(errorCode, ADS_TYPE::kTypeCrosspromoAds);} };
    CrosspromoAds::getInstance()->onCrosspromoClicked = [&](){ if (onAdsClicked){onAdsClicked(ADS_TYPE::kTypeCrosspromoAds);} };
    CrosspromoAds::getInstance()->onCrosspromoExpanded = [&](){ if (onAdsExpanded){onAdsExpanded(ADS_TYPE::kTypeCrosspromoAds);/*m_iFullScreenAdsShowingTag ++;*/} };
    CrosspromoAds::getInstance()->onCrosspromoCollapsed = [&](){ if (onAdsCollapsed){onAdsCollapsed(ADS_TYPE::kTypeCrosspromoAds);/*m_iFullScreenAdsShowingTag --;*/} };
#endif
    CrosspromoAds::getInstance()->delegate=this;
}

void AdsManager::initRectAdsDelegate()
{
#if __cplusplus > 201100L
    RectAds::getInstance()->onRectLoaded = [&](){ if (onAdsLoaded){onAdsLoaded(ADS_TYPE::kTypeRectAds);} };
    RectAds::getInstance()->onRectLoadFailed = [&](string errorCode){ if (onAdsLoadFailed){onAdsLoadFailed(errorCode, ADS_TYPE::kTypeRectAds);} };
    RectAds::getInstance()->onRectClicked = [&](){ if (onAdsClicked){onAdsClicked(ADS_TYPE::kTypeRectAds);} };
    RectAds::getInstance()->onRectExpanded = [&](){ if (onAdsExpanded){onAdsExpanded(ADS_TYPE::kTypeRectAds);} };
    RectAds::getInstance()->onRectCollapsed = [&](){ if (onAdsCollapsed){onAdsCollapsed(ADS_TYPE::kTypeRectAds);} };
#endif
    RectAds::getInstance()->delegate=this;
}

void AdsManager::initRewardedAdsDelegate()
{
#if __cplusplus > 201100L
    RewardedAds::getInstance()->onRewardedLoaded =[&](){ if (onAdsLoaded){onAdsLoaded(ADS_TYPE::kTypeRewardedAds);} };
    RewardedAds::getInstance()->onRewardedFailed = [&](string errorCode){ if (onAdsLoadFailed){onAdsLoadFailed(errorCode, ADS_TYPE::kTypeRewardedAds);} };
    RewardedAds::getInstance()->onRewardedClicked = [&](){ if (onAdsClicked){onAdsClicked(ADS_TYPE::kTypeRewardedAds);} };
    RewardedAds::getInstance()->onRewardedExpanded = [&](){ if (onAdsExpanded){onAdsExpanded(ADS_TYPE::kTypeRewardedAds);} };
    RewardedAds::getInstance()->onRewardedCollapsed=[&](){ if (onAdsCollapsed){onAdsCollapsed(ADS_TYPE::kTypeRewardedAds);} };
    RewardedAds::getInstance()->onRewarded=[&](std::string item,int count,bool isSkipped){ if (onAdsRewarded){onAdsRewarded(item,count,isSkipped);} };
#endif
    RewardedAds::getInstance()->delegate=this;
}

void AdsManager::initNativeAdsDelegate()
{
    
#if __cplusplus > 201100L
    SSCNativeAds::getInstance()->onNativeAdsLoaded = [&](){if (onAdsLoaded){onAdsLoaded(ADS_TYPE::kTypeNativeAds);}};
    SSCNativeAds::getInstance()->onNativeAdsLoadFailed = [&](string errorCode){ if (onAdsLoadFailed){onAdsLoadFailed(errorCode, ADS_TYPE::kTypeNativeAds);} };
    SSCNativeAds::getInstance()->onNativeAdsClicked= [&](){ if (onAdsClicked){onAdsClicked(ADS_TYPE::kTypeNativeAds);} };
    SSCNativeAds::getInstance()->onNativeAdsCollapsed= [&](){ if (onAdsCollapsed){onAdsCollapsed(ADS_TYPE::kTypeNativeAds);} };
    SSCNativeAds::getInstance()->onNativeAdsExpanded =  [&](){ if (onAdsExpanded){onAdsExpanded(ADS_TYPE::kTypeNativeAds);}};
#endif
    SSCNativeAds::getInstance()->delegate=this;
}


bool AdsManager::isFullScreenShowing()
{
    return m_iFullScreenAdsShowingTag>0?true:false;
}
#pragma mark - delegete
void AdsManager::onBannerLoaded(void){
    if(this->delegate){
        this->delegate->onAdsLoaded(kTypeBannerAds);
    }
}
void AdsManager::onBannerLoadFailed(std::string error){
    if(this->delegate){
        this->delegate->onAdsLoadFailed(error,kTypeBannerAds);
    }
}
void AdsManager::onBannerClicked(void){
    if(this->delegate){
        this->delegate->onAdsClicked(kTypeBannerAds);
    }
}
void AdsManager::onBannerExpanded(void){
    if(this->delegate){
        this->delegate->onAdsExpanded(kTypeBannerAds);
    }
}
void AdsManager::onBannerCollapsed(void){
    if(this->delegate){
        this->delegate->onAdsCollapsed(kTypeBannerAds);
    }
}
void AdsManager::onRectLoaded(void){
    if(this->delegate){
        this->delegate->onAdsLoaded(kTypeRectAds);
    }
    
}
void AdsManager::onRectLoadFailed(std::string error){
    if(this->delegate){
        this->delegate->onAdsLoadFailed(error,kTypeRectAds);
    }
}
void AdsManager::onRectClicked(void){
    if(this->delegate){
        this->delegate->onAdsClicked(kTypeRectAds);
    }
}
void AdsManager::onRectExpanded(void){
    if(this->delegate){
        this->delegate->onAdsExpanded(kTypeRectAds);
    }
}
void AdsManager::onRectCollapsed(void){
    if(this->delegate){
        this->delegate->onAdsCollapsed(kTypeRectAds);
    }
}
void AdsManager::onInterstitialLoaded(void){
    if(this->delegate){
        this->delegate->onAdsLoaded(kTypeInterstitialAds);
    }
}
void AdsManager::onInterstitialFailed(std::string error){
    if(this->delegate){
        this->delegate->onAdsLoadFailed(error,kTypeInterstitialAds);
    }
}
void AdsManager::onInterstitialClicked(void){
    if(this->delegate){
        this->delegate->onAdsClicked(kTypeInterstitialAds);
    }
}
void AdsManager::onInterstitialExpanded(void){
    m_iFullScreenAdsShowingTag ++;
    if(this->delegate){
        this->delegate->onAdsExpanded(kTypeInterstitialAds);
    }
}
void AdsManager::onInterstitialCollapsed(void){
    m_iFullScreenAdsShowingTag --;
    if(this->delegate){
        this->delegate->onAdsCollapsed(kTypeInterstitialAds);
    }
}
void AdsManager::onCrosspromoLoaded(void){
    if(this->delegate){
        this->delegate->onAdsLoaded(kTypeCrosspromoAds);
    }
}
void AdsManager::onCrosspromoFailed(std::string error){
    if(this->delegate){
        this->delegate->onAdsLoadFailed(error,kTypeCrosspromoAds);
    }
}
void AdsManager::onCrosspromoClicked(void){
    if(this->delegate){
        this->delegate->onAdsClicked(kTypeCrosspromoAds);
    }
}
void AdsManager::onCrosspromoExpanded(void){
    m_iFullScreenAdsShowingTag ++;
    if(this->delegate){
        this->delegate->onAdsExpanded(kTypeCrosspromoAds);
    }
}
void AdsManager::onCrosspromoCollapsed(void){
    m_iFullScreenAdsShowingTag --;
    if(this->delegate){
        this->delegate->onAdsCollapsed(kTypeCrosspromoAds);
    }
}
void AdsManager::onRewardedLoaded(void){
    if(this->delegate){
        this->delegate->onAdsLoaded(kTypeRewardedAds);
    }
}
void AdsManager::onRewardedFailed(std::string error){
    if(this->delegate){
        this->delegate->onAdsLoadFailed(error,kTypeRewardedAds);
    }
}
void AdsManager::onRewardedClicked(void){
    if(this->delegate){
        this->delegate->onAdsClicked(kTypeRewardedAds);
    }
}
void AdsManager::onRewardedExpanded(void){
    m_iFullScreenAdsShowingTag ++;
    if(this->delegate){
        this->delegate->onAdsExpanded(kTypeRewardedAds);
    }
}
void AdsManager::onRewardedCollapsed()
{
    m_iFullScreenAdsShowingTag --;
    if(this->delegate){
        this->delegate->onAdsCollapsed(kTypeRewardedAds);
    }
}
void AdsManager::onRewarded(std::string item, int count, bool isSkipped){
    if(this->delegate){
        this->delegate->onAdsRewarded(item, count, isSkipped);
    }
}

void AdsManager::onNativeAdsLoaded(void){
    if(this->delegate){
        this->delegate->onAdsLoaded(kTypeNativeAds);
    }
}
void AdsManager::onNativeAdsLoadFailed(std::string error){
    if(this->delegate){
        this->delegate->onAdsLoadFailed(error,kTypeNativeAds);
    }
}
void AdsManager::onNativeAdsClicked(void){
    if(this->delegate){
        this->delegate->onAdsClicked(kTypeNativeAds);
    }
}
void AdsManager::onNativeAdsExpanded(void){
    if(this->delegate){
        this->delegate->onAdsExpanded(kTypeNativeAds);
    }
}
void AdsManager::onNativeAdsCollapsed(void){
    if(this->delegate){
        this->delegate->onAdsCollapsed(kTypeNativeAds);
    }
}