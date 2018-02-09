

#include "RewardAdLoadingLayer.h"
#include "UICpp.h"
#include "CBPackScene.h"
#include "GameScene.h"
#include "CBAppManager.h"
#include "TryLaterLayer.h"
#include "AdsLoadingLayer.h"
#include "STSystemFunction.h"

RewardAdLoadingLayer::~RewardAdLoadingLayer()
{
    unschedule(schedule_selector(RewardAdLoadingLayer::closeReward));
    
    AdsManager::getInstance()->delegate = nullptr;
    
    xApp->setEnableFullAds(true);
}

bool RewardAdLoadingLayer::init()
{
    if (Layer::init())
    {
        return true;
    }
    return false;
}

void RewardAdLoadingLayer::onEnter()
{
    Layer::onEnter();
    auto listener1 = EventListenerTouchOneByOne::create();
    listener1->onTouchBegan = [](Touch* touch, Event* event){
        return true;
    };
    listener1->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
}

void RewardAdLoadingLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    AdsManager::getInstance()->delegate = this;
    bool rtn = AdsManager::getInstance()->showAds(kTypeRewardedAds);
    log("reward showAds %d", rtn);
    
    if (!rtn) {
        auto try_later = TryLaterLayer::create();
        auto scene = Director::getInstance()->getRunningScene();
        scene->addChild(try_later, 100);
        
        log("reload fail!");
        AdsManager::getInstance()->delegate = nullptr;
        this->removeFromParent();
        //弹出失败对话框
    }else{
        
    }
    
    xApp->setEnableFullAds(false);
}


void RewardAdLoadingLayer::onAdsLoaded(ADS_TYPE adType)
{
    if(adType == ADS_TYPE::kTypeRewardedAds)
    {
        log("reward loaded.");
    }
    
};
void RewardAdLoadingLayer::onAdsLoadFailed(std::string error, ADS_TYPE adType)
{
    if(adType == ADS_TYPE::kTypeRewardedAds)
    {
        log("reward load failed : %s", error.c_str());
    }
};


#pragma mark - Ad Delegate
void RewardAdLoadingLayer::onAdsCollapsed(ADS_TYPE adType)
{
}

void RewardAdLoadingLayer::onAdsExpanded(ADS_TYPE adType) {
    if(adType == ADS_TYPE::kTypeRewardedAds)
    {
        xAds->pauseAds(kTypeBannerAds);
        _loading = AdsLoadingLayerEx::create();
        Director::getInstance()->getRunningScene()->addChild(_loading, 100);
        scheduleOnce(schedule_selector(RewardAdLoadingLayer::closeReward), 45.f);
    }
}

void RewardAdLoadingLayer::onAdsRewarded(std::string str, int num, bool isOk)
{
    if (isOk) {
        STSystemFunction sf;
        if (sf.checkNetworkAvailable()) {
            this->removeFromParent();
        }
        else
        {
            closeReward(0);
        }
    }else{
        _eventDispatcher->dispatchCustomEvent(EVENT_REWARD);
        
        AdsManager::getInstance()->delegate = nullptr;
        scheduleOnce(schedule_selector(RewardAdLoadingLayer::closeReward), 3);
    }
}

void RewardAdLoadingLayer::closeReward(float dt)
{
    Director::getInstance()->getRunningScene()->removeChild(_loading);
    xAds->resumeAds(kTypeBannerAds);
    removeFromParent();
}


