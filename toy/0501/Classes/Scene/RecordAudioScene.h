//
//  RecordAudioScene.h
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#ifndef __BedtimeStory__RecordAudioScene__
#define __BedtimeStory__RecordAudioScene__

#include "BaseScene.h"
#include "RecordItem.hpp"
#include "PlayRecordItemBar.hpp"
#include "AppManager.h"
#include "STInAppBilling.h"

class RecordAudioScene : public BaseScene, public STIABDelegate
{
public:
    RecordAudioScene();
    virtual ~RecordAudioScene();
    virtual bool init() override;
    
    void setBackCallback(const std::function<void()>& callback);

    void startWithStoryIndex(const int index);
    
protected:
    void refreshRecords();
    
    void backAction();

    void beginRecordAction();
    void stopRecordAction();
    
    void touchedRecordItem(RecordItem *item);
    void playRecordItemAction(RecordItem *item);
    void pauseRecordItemAction(RecordItem *item);
    void deleteRecordItemAction();
    void refreshRecordItemAction();
    
    void selectRecordItem(RecordItem *item);
    void unSelectRecordItem(RecordItem *item);

    void touchedRecordItemBar();
    void showPlayRecordBar(const cocos2d::Vec2& fromePosition);
    void hidePlayRecordBar(const cocos2d::Vec2& fromePosition);
    
    void nextStoryAction();
    void previousStoryAction();
    
    void showAlert();
    
    void showPurchaseDialog();
    
    void timer(float time);
    
    //STIABDelegate
    virtual void purchaseSuccessful(const char* pid) override;
    virtual void purchaseFailed(const char *pid, int errorCode) override;
    virtual void restoreSuccessful(const char* pid) override;
    virtual void restoreFailed(const char* pid, int errorCode) override;
    
protected:
    std::string getCurrentFormatTime(const std::string& seperator);

    cocos2d::ui::Button *_recordButton;
    cocos2d::ui::Text   *_beginTimeLabel;
    cocos2d::ui::Text   *_endTimeLabel;
    cocos2d::ui::Slider *_timeSilder;
    
    LayerColor          *_rightLayer;
    
    cocos2d::Sprite     *_soundLevelIndicator1;
    cocos2d::Sprite     *_soundLevelIndicator2;
    cocos2d::Sprite     *_soundLevelIndicator3;
    cocos2d::Sprite     *_soundLevelIndicator4;
    PlayRecordItemBar   *_playRecordItemBar;
    
    cocos2d::Sprite     *_bookSprite;
    cocos2d::Sprite     *_leftStoryImage;
    cocos2d::Sprite     *_rightStoryImage;
    
    std::vector<RecordItem*> _recordItemVector;
    
    STInAppBilling _iap;

    int _storyIndex;
    
    float _recordTime;
    float _playTime;
    
    bool _isShowPlayRecordBar;
//    bool _isPlaying;
//    bool _isPausing;

    bool _isAnimation;
    bool _isRecording;
    std::string _currentRecordFile;
    
    std::function<void()> _backCallback;
};

#endif /* defined(__BedtimeStory__RecordAudioScene__) */
