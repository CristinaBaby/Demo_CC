//
//  PlayAudioScene.h
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#ifndef __BedtimeStory__PlayAudioScene__
#define __BedtimeStory__PlayAudioScene__

#include "BaseScene.h"
#include "RecordItem.hpp"
#include "PlayRecordItemBar.hpp"
#include "AppManager.h"
#include "STInAppBilling.h"

class PlayAudioScene : public BaseScene, public STIABDelegate
{
public:
    PlayAudioScene();
    virtual ~PlayAudioScene();
    virtual bool init() override;
    
    void startPlayWithStoryIndex(const int index);
    
protected:
    void backAction();
    void leftAction();
    void rightAction();
    void playAction();
    
    void showParentGate();
    
    void showPurchaseDialog();

    //STIABDelegate
    virtual void purchaseSuccessful(const char* pid) override;
    virtual void purchaseFailed(const char *pid, int errorCode) override;
    virtual void restoreSuccessful(const char* pid) override;
    virtual void restoreFailed(const char* pid, int errorCode) override;
    
protected:
    cocos2d::Sprite     *_background;

    cocos2d::Sprite     *_leftStoryImage;
    cocos2d::Sprite     *_rightStoryImage;
    
    STInAppBilling _iap;

    bool _isAnimation;
    int _index;
    int _readedStoryCount;
};

#endif /* defined(__BedtimeStory__PlayAudioScene__) */
