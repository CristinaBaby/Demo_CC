//
//  SpotDifferenceScene.h
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#ifndef __BedtimeStory__SpotDifferenceScene__
#define __BedtimeStory__SpotDifferenceScene__

#include "BaseScene.h"
#include "STInAppBilling.h"

class SpotDifferenceScene : public BaseScene, public STIABDelegate
{
public:
    SpotDifferenceScene();
    virtual ~SpotDifferenceScene();
    
    virtual bool init() override;

    void startGameWithGameLevel(const int level);

protected:

    /* register touch event */
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
    
    //STIABDelegate
    virtual void purchaseSuccessful(const char* pid) override;
    virtual void purchaseFailed(const char *pid, int errorCode) override;
    virtual void restoreSuccessful(const char* pid) override;
    virtual void restoreFailed(const char* pid, int errorCode) override;
    
    void showPurchaseDialog();

    //tmx map
    void setupTMXMap(const int level);
    
    //picture
    void setupPicture(const int level);
    
    //icons
    void setupIcons(const int level);
    
    //setup finish game layer
    void setupFinshGameLayer();
    
    //show quit layer
    void showQuitGameLayer();
    
    float getIconPositionX(const int iconIndex);
    float getIconPositionY(const int iconIndex);

    void touchInPicture(cocos2d::Sprite *picture, const cocos2d::Point& position);
    void correctTouch(cocos2d::Sprite *icon);
    
    void timerUpdate(float time);

    void finishGame();
private:
    cocos2d::Sprite     *_rightMarkSprite1;
    cocos2d::ui::Button *_hintButton;
    
    cocos2d::Sprite             *_leftPicture;
    cocos2d::Sprite             *_rightPicture;
    cocos2d::ui::Text           *_timeLabel;
    cocos2d::ui::LoadingBar     *_progressBar;
    cocos2d::ui::ImageView      *_progressImage1;
    cocos2d::ui::ImageView      *_progressImage2;
    cocos2d::ui::ImageView      *_progressImage3;
    cocos2d::ui::ImageView      *_progressImage4;

    cocos2d::Layer      *_gameContentLayer;
    cocos2d::Layer      *_finishGameLayer;

    cocos2d::experimental::TMXTiledMap *_tmxMap;

    std::vector<cocos2d::Sprite *> _iconsVector;
    std::vector<cocos2d::Sprite *> _hooksVector;

    int     _gameLevel;
    int     _usedTime;
    int     _hintCooldownTime;
    STInAppBilling _iap;

    bool    _pause;
};

#endif /* defined(__BedtimeStory__SpotDifferenceScene__) */
