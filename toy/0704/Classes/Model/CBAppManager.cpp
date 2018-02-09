//
//  CBAppManager.cpp
//  ColorBook
//
//  Created by maxiang on 4/21/15.
//
//

#include "CBAppManager.h"
#include "../Module/STSystemFunction.h"
#include "CBPictureScene.h"
#include "../Layer/AdsLoadingLayer.h"
#include "CBPackScene.h"
#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

USING_NS_CC;


static AppManager *s_SingletonAppManager = nullptr;

AppManager* AppManager::getInstance()
{
    if (!s_SingletonAppManager) {
        s_SingletonAppManager = new (std::nothrow)(AppManager);
    }
    
    return s_SingletonAppManager;
}

AppManager::AppManager():_saveTimes(0)
{
    bool isFirstLaunch = UserDefault::getInstance()->getBoolForKey(UserDefaultKey_FirstTimeLaunchApp, true);
    if (isFirstLaunch)
    {
        /* if is first time launch app, DO NOT request full screen ads */
        UserDefault::getInstance()->setBoolForKey(UserDefaultKey_FirstTimeLaunchApp, false);
        UserDefault::getInstance()->flush();
        
        setIsFirstLaunchApp(true);
    }
    else
    {
        setIsFirstLaunchApp(false);
    }
    
    _enableFullAds = true;
}

AppManager::~AppManager()
{
    s_SingletonAppManager = nullptr;
}

#pragma mark- Rate us logic
void AppManager::rateUsLogic()
{
    _saveTimes++;
    if (_saveTimes % 3 == 0)
    {
        STSystemFunction function = STSystemFunction();
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        function.rating(APP_ID, "If you like this app, please rate our game!");
#else
        function.rating();
#endif
    }
}

#pragma mark- Ads methods
void AppManager::requestCrossAd()
{
#if TARGET_IPHONE_SIMULATOR == 0
    AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true);
#endif
}

void AppManager::requestCrossAd(Node* parent, int zorder)
{
#if TARGET_IPHONE_SIMULATOR == 0
    AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true, parent, zorder);
#endif
}

void AppManager::requestFullScreenAd()
{
    AdLoadingLayerBase::showLoading<AdsLoadingLayer>(false, nullptr, 101);
}

void AppManager::requestFullScreenAd(Node* parent, int zorder)
{
    AdLoadingLayerBase::showLoading<AdsLoadingLayer>(false, parent, zorder);
}
