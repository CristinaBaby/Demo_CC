//
//  AppManager.h
//  ParfaitMaker
//
//  Created by maxiang on 7/30/15.
//
//

#ifndef __ParfaitMaker__AppManager__
#define __ParfaitMaker__AppManager__

#include "cocos2d.h"
#include "AppConfigs.h"

class AppManager
{
public:
    static AppManager* getInstance();
    virtual ~AppManager();
    AppManager();

    //format string
    const std::string getFormatTimeString(const int time) const;
    const std::string getFormatDateString() const;
    
    bool isDigit(const std::string& string);

    //sound
    void playEffect(const std::string& soundFile);
    void playBgm(const std::string& soundFile);
    
    void playLoopEffect(const std::string& soundFile);
    void pauseLoopEffect(const std::string& soundFile);
    void stopLoopEffect(const std::string& soundFile);

    //ads
    CC_SYNTHESIZE(bool, _hasRemoveAds, HasRemoveAds);
    
    void requestFullScreenAd();
    void requestCrossPromoAd();

    void requestBannerAd();
    void setBannerAdVisible(bool visible);
    
    CC_SYNTHESIZE(bool, _hasUnlockAll, HasUnlockAll);
    CC_SYNTHESIZE(bool, _isOpenSound, IsOpenSound);
protected:

    bool _isShowBannerAds;

    std::unordered_map<std::string, int> _loopEffectSoundMap;
};


#endif /* defined(__ParfaitMaker__AppManager__) */
