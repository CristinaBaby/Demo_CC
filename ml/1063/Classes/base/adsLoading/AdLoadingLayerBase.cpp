//
//  AdLoadingLayerBase.cpp
//
//  Created by tanshoumei on 15/6/10.
//
//

#include "AdLoadingLayerBase.h"
#include "SimpleAudioEngine.h"
#include "KeyBackEvent.h"
using namespace CocosDenshion;
using namespace cocos2d::ui;

AdLoadingLayerBase* AdLoadingLayerBase::s_currentInstance = nullptr;

Action* AdLoadingLayerBase::s_adShowAction = nullptr;

 const string AdLoadingLayerBase::ADS_SHOW = "ADS_SHOW";
 const string AdLoadingLayerBase::ADS_CLOSE = "ADS_CLOSE";

std::function<void()> AdLoadingLayerBase::loadingDoneCallback = nullptr;

AdLoadingLayerBase::~AdLoadingLayerBase()
{
    AdsManager::getInstance()->delegate=nullptr;
    
    loadingDoneCallback = nullptr;
    
    s_currentInstance = nullptr;
}

bool AdLoadingLayerBase::init()
{
    Layer::init();
    
    _loadingMinTime = 0;
    _loadingMaxTime = 5;
    
    s_currentInstance = this;
    
    _eventListenerTouch = EventListenerTouchOneByOne::create();
    _eventListenerTouch->setSwallowTouches(true);
    _eventListenerTouch->onTouchBegan = [](Touch* touch, Event* event)
    {
        return true;
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_eventListenerTouch, this);
    return true;
}

void AdLoadingLayerBase::onExit()
{
    Layer::onExit();
    
    _scheduler->unschedule(schedule_selector(AdLoadingLayerBase::_timeCheckSchedule), this);
    loadingDoneCallback = nullptr;
    if(s_adShowAction){
        Director::getInstance()->getRunningScene()->stopAction(s_adShowAction);
    }
    s_adShowAction = nullptr;
     _eventDispatcher->dispatchCustomEvent(ADS_CLOSE);
    AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds, true);
    
}

void AdLoadingLayerBase::onEnter()
{
     AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds, false);
    Layer::onEnter();
    _eventDispatcher->dispatchCustomEvent(ADS_SHOW);
    KeyBackEvent::getInstance()->addBackEvent([](){return true;}, this);
    _scheduler->schedule(schedule_selector(AdLoadingLayerBase::_timeCheckSchedule), this, 0, false);
    
    s_currentInstance = this;
}

void AdLoadingLayerBase::setSwallowTouch(bool value)
{
    _eventListenerTouch->setSwallowTouches(value);
}

void AdLoadingLayerBase::_timeCheckSchedule(float dt)
{
    struct timeval now;
    gettimeofday(&now, nullptr);
    float lTimePasses = now.tv_sec - _timeEnter.tv_sec;
    if ( ((lTimePasses >= _loadingMinTime) && _adLoadDone))
    {
        _adLoadInTime();
    }
    else if (lTimePasses >=_loadingMaxTime)
    {
        _adLoadTimeOut();
    }
    
}

void AdLoadingLayerBase::loadAd(bool abIscross)
{
    //    if(abIscross){
    //        return;
    //    }
    
    _isRequestingLoadAd = false;
    _adShowing = false;
    _bLoadingCanRemove = false;
    _adLoadDone = false;
    gettimeofday(&_timeEnter, nullptr);
    AdsManager::getInstance()->delegate=this;
    if(!_running)
        return;
    if(abIscross)
        _adLoadType = kTypeCrosspromoAds;
    else
        _adLoadType = kTypeInterstitialAds;
    this->runAction(Sequence::create(DelayTime::create(0.1f),CallFunc::create([this](){
        
        _scheduler->schedule(schedule_selector(AdLoadingLayerBase::_timeCheckSchedule), this, 0, false);
        
        bool ret = AdsManager::getInstance()->showAds(_adLoadType);
        if(!ret)
            _isRequestingLoadAd = true;
    }), nullptr));
 
    
    
}

void AdLoadingLayerBase::onAdsLoadFailed(std::string error, ADS_TYPE adType)
{
    if(_adLoadType == adType)
        _adLoadDone = true;
}

void AdLoadingLayerBase::onAdsExpanded(ADS_TYPE adType)
{
    if(_adLoadType == adType)
    {
        _adLoadDone = true;
        _adShowing  = true;
    }
}

void AdLoadingLayerBase::onAdsLoaded(ADS_TYPE adType)
{
    if (!_isRequestingLoadAd)
        return;
    if(_adLoadType == adType)
    {
        AdsManager::getInstance()->showAds(_adLoadType);
    }
}

void AdLoadingLayerBase::onAdsCollapsed(ADS_TYPE adType)
{
    if(_adLoadType == adType)
    {
        if(_bLoadingCanRemove)
        {
            _taskDone();
            return;
        }
        _adShowing = false;
    }
    _timeCheckSchedule(0);
}

void AdLoadingLayerBase::_adLoadInTime()
{
    //广告已关闭或加载失败
    if(!_adShowing)
    {
        _taskDone();
    }
    //广告正在显示
    else
    {
        _bLoadingCanRemove = true;
    }
}

void AdLoadingLayerBase::_adLoadTimeOut()
{
    //广告已关闭或加载失败
    if(!_adShowing)
    {
        _taskDone();
    }
    //广告正在显示
    else
    {
        _bLoadingCanRemove = true;
    }
}

void AdLoadingLayerBase::_taskDone()
{
    _scheduler->unschedule(schedule_selector(AdLoadingLayerBase::_timeCheckSchedule), this);
    
    auto call = [this]()
    {
        if(loadingDoneCallback)
        {
            loadingDoneCallback();
        };
        removeFromParent();
    };
    runAction(CallFunc::create(call));
}