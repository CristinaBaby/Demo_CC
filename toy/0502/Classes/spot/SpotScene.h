//
//  SpotScene.h
//  SpotDifference
//
//  Created by Steven.Tian on 14-10-27.
//
//

#ifndef __SpotDifference__GameScene__
#define __SpotDifference__GameScene__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "../AppGlobal.h"
#include "../modules/STInAppBilling.h"
#include "../utilities/UITools.h"

class LevelInfo;

class GameScore;

class SpotScene : public cocos2d::Layer, public STIABDelegate
{
public:
    static cocos2d::Scene* createScene(LevelInfo* level);
    
    static SpotScene * create(LevelInfo* level);
    
    
protected:
    bool initData(LevelInfo* level);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
    SpotScene() :
    _mIsGamePause(false),
    mMapDistance(0),
    _mRemainingTime(0), _mGameMaxTime(0), _mSpentTime(1),
    _mFreezeTime(5),
    _mIsGameTimeFroze(false),
    _mHintNumber(0), _mFreezeNumber(0), _mAddTimeNumber(0),
    _mIsGameFailed(false),
    _mGameScore(0),
    _mIsComboStatus(false), _mComboNum(0),
    sound_game_time_cd_id(0), willRefreshRectAd(false),
    _mBlock(nullptr),
    m_PlayedLevels(0)
    {
    }
    
    ~SpotScene();
    
    void onButton(Ref* sender, Widget::TouchEventType type);
    void onDialog(const string& name);
    
public: // from STIABDelegate
    void purchaseSuccessful(const char* pid);
    void purchaseFailed(const char* pid, int errorCode);
    void restoreSuccessful(const char* pid);
    void restoreFailed(const char* pid, int errorCode);
    
private:
    bool buildMap();
    bool buildUI();
    
    bool pauseGame();
    bool resumeGame();
    
    void _pauseGameTime();
    void _resumeGameTime();
    
    void finishLevel(float dt);
    void resetLevel();
    
    
    cocos2d::Point getMirrorSymmetryPoint(const cocos2d::Point&);
    
    void spotDifference(cocos2d::Sprite*, const cocos2d::Point&);
    
    void touchWrong(const cocos2d::Point&);
    
    void updatePropsNum(const std::string &sku);
    void goToShop(cocos2d::Ref*);
    
    //触摸框相关
    Widget * _left;
    Widget * _right;
    
    
    //时间相关
    void updateRecord(float dt);
    int  _times = 0;
    
public:
    void onPauseCallback(cocos2d::Ref* pSender);
    void onHintCallback(cocos2d::Ref* pSender);
    void onFreezeCallback(cocos2d::Ref*);
    void onAddTimeCallback(cocos2d::Ref*);
    // pause layer
    void onResumeCallback(cocos2d::Ref*);
    void onRefreshCallback(cocos2d::Ref*);
    void onLevelsCallback(cocos2d::Ref*);
    void onSettingCallback(cocos2d::Ref*);
    void onNextCallback(cocos2d::Ref*);
    void onModeCallback(cocos2d::Ref*);
    
    void scheduleCallback(float);
    void onComboCallback(float);
    
    void onShopClosedCallback();
    void onRefreshCloseCallback(Node*);
    
private: // game score
    void initGameStatus();
    
    void settleCombo();
    void settleTime();

private:
    cocos2d::extension::Scale9Sprite *_mBlock;
    cocos2d::Point mLeftBlockCenter, mRightBlockCenter;
    cocos2d::Vector<cocos2d::Sprite*> mTargetObjects;
    cocos2d::SpriteBatchNode *mpIndicatorLayer = nullptr;
    cocos2d::ProgressTimer *mpProgressTimer = nullptr;
    
    LevelInfo *_mLevelInfo;
    
    vector<CheckBox*> _vStars;
    
private:
    Layout * _root;
    //
    float mMapDistance; // the distance between left map to the right.
    int _mGameScore;
    int _mFreezeTime;
    int _mComboNum;
    int _mGameMaxTime;
    int _mSpentTime;
    
    bool _mIsGamePause;
    bool _mIsGameTimeFroze;
    bool _mIsGameFailed;
    bool _mIsComboStatus;
    
    cocos2d::Value _mRemainingTime;
    cocos2d::Value _mHintNumber, _mFreezeNumber, _mAddTimeNumber;

    unsigned sound_game_time_cd_id;
    bool willRefreshRectAd;
    
    unsigned m_PlayedLevels;
};



class LevelInfo : public cocos2d::Ref
{
public:
    CREATE_FUNC(LevelInfo);
    
protected:
    bool init()
    { return true; }
    
public:
    int m_Index = 0;
    AppGlobal::GameMode m_GameMode;
    cocos2d::ValueMap m_Info;
    
};



#endif /* defined(__SpotDifference__GameScene__) */
