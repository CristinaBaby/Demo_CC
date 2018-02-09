//
//  CBAppManager.h
//  ColorBook
//
//  Created by maxiang on 4/21/15.
//
//

#ifndef __ColorBook__CBAppManager__
#define __ColorBook__CBAppManager__

#include "cocos2d.h"
#include "CBAppGlobal.h"

#define xApp    AppManager::getInstance()

class AppManager
{
public:
    static AppManager* getInstance();
    virtual ~AppManager();
    AppManager();
    
    //first launch app
    bool isFirstLaunchApp(){return _isFirstLaunchApp;};
    void setIsFirstLaunchApp(bool isFirstLaunch){_isFirstLaunchApp = isFirstLaunch;};

    //ads
    void requestCrossAd();
    void requestCrossAd(cocos2d::Node* parent, int zorder = 0);
    void requestFullScreenAd();
    void requestFullScreenAd(cocos2d::Node* parent, int zorder);
    
    /* Rate us logic, when user save picture 3 times, show rate us */
    void rateUsLogic();
    
    CC_SYNTHESIZE(cocos2d::Vec2, _backPosition, BackPosition);
    CC_SYNTHESIZE(bool, _enableFullAds, EnableFullAds);
    
protected:
    int _saveTimes;
    bool _isFirstLaunchApp;
};


#endif /* defined(__ColorBook__CBAppManager__) */
