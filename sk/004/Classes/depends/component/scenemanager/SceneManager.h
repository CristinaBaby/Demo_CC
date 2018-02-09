//
//  SceneManager.h
//
//  Created by zhangguangzong1 on 15/7/3.
//
//

#ifndef CODEBASE_COMPONENT_SCENEMANAGER_SCENEMANAGER_H_
#define CODEBASE_COMPONENT_SCENEMANAGER_SCENEMANAGER_H_
#include <iostream>
#include <vector>
#include "cocos2d.h"
#include "3rdparty_depends/crossplatform/headers/ads/AdsManager.h"

using namespace std;
USING_NS_CC;

/* 场景控制器
 * 主要负责场景切换中,过度场景的控制
 */
class SceneManager : public Ref{
    
public:
    enum class RewardCode
    {
        kRewardOk       =       1,
        kNoCache        =       2,
        kNetworkError   =       3,
        kLoadFailed     =       4,
        kRewardTimeOut  =       5,
    };
    
    static SceneManager* getInstance();
    
    /*==========封装常用的场景切换函数==========*/
    void replaceWithDefaultTransition(Scene* pScene);
    
    void replaceWithTransition(TransitionScene* pTranstionScene);
    
    void pushWithDefaultTransition(Scene* pScene);
    
    void pushWithTransition(TransitionScene* pTranstionScene);
    
    void popScene();
    /*======================================*/
    
    /*简化调用的接口,直接传loadingLayer对象*/
    virtual void pushLoaing(Layer* loadingLayer);
    virtual void replaceLoaing(Layer* loadingLayer);
    
    virtual Scene* getLoadingScene(Layer* loadingLayer);
    
    /*添加过度场景*/
    virtual void pushLoaing(Scene* scene);
    virtual void replaceLoaing(Scene* scene);
    
    virtual Scene* getNextScene();
    
    virtual void setNextScene(std::function<Scene*()> fn) final;
    
    virtual void setLoadingCloseCall(std::function<void()> fn) final;
    
    virtual void timeCheckSchedule(float dt);
    
    //Loading界面最多停留的时间(秒)
    CC_SYNTHESIZE(float, _loadingMaxTime, LoadingMaxTime);
    
    CC_SYNTHESIZE(float, _loadingMinTime, LoadingMinTime);
    
    void setShowRewardingAds(bool isShow)
    {
        _showRewardingAds = isShow;
    }
    
    bool isShowRewardingAds()
    {
        return _showRewardingAds;
    }
    
    void enterLoading()
    {
        _inLoading = true;
    }
    
    void exitLoading()
    {
        _inLoading = false;
    }
    
    bool isInloading()
    {
        return _inLoading;
    }
    
    bool isEnterBackground()
    {
        return _pause;
    }
    
    void enterBackground()
    {
        _pause = true;
    }
    
    void enterForeground()
    {
        _pause = false;
    }
    
    void resLoading()
    {
        _loadOk = false;
    }
    
    void loadingComplete()
    {
        _loadOk = true;
    }
    
    void setAdClose(bool val)
    {
        _adClose = val;
    }
    
    void setAdsShow()
    {
        _showAds = true;
    }
    
protected:
    SceneManager();
    ~SceneManager();
    
    /*过渡场景之后进入下一个场景*/
    virtual bool gotoNextScene();
    
    /*检查是否需要可以进入下一个场景*/
    virtual void checkGotoNext();
    
    std::function<Scene*()>     _fnGetNextScene = nullptr;
    std::function<void()>       _fnLoadingClose = nullptr;
    
    bool _loadOk = false;
    bool _timeOk = false;
    /*当广告加载成功时，过渡场景不会自己关闭，在用户点击关闭广告之后才会关闭*/
    bool _adClose = false;
    bool _showAds = false;
    
    bool _pause = false;
    
    
    bool _inLoading = false;
    
    bool _showRewardingAds = false;
    
    timeval _timeEnter;
};

struct RewardAdsInfo
{
    string productId;
    
    bool   isSkip;
    
    SceneManager::RewardCode resultCode;
};


/*过渡场景*/
class LoadingScene : public Scene{
    
public:
    bool init();
    
    CREATE_FUNC(LoadingScene);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void addLoadingLayer(Node* loading) final;
    /*加载图片资源*/
    virtual void loadRes(const string& path) final;
    /*加载的回调*/
    virtual void loadCallback(Texture2D* tex) final;
protected:
    LoadingScene(){}
    ~LoadingScene(){}
    /*加载图片计数*/
    int     m_iLoadingCount = 0;
    /*加载完成计数*/
    int     m_iCompleteCount = 0;
    
    bool    m_bCancelComplete = false;
};

class LoadingLayerBase: public Layer
{
public:
    static LoadingLayerBase* create();
    
    virtual bool init() override;
    
    virtual void onExit() override;
    
    virtual void onEnter() override;
    
    virtual void onEnterTransitionDidFinish() override;
    
    void showAd(ADS_TYPE type,bool isReume = false);
    
    void rewardCallbackCheck(float dt);
    
    void closeRewardAds(const string& productid,int count,bool isSkip,
                        SceneManager::RewardCode code);
#pragma mark --ad delegate--
    void onAdsLoaded(ADS_TYPE);
    void onAdsLoadFailed(std::string, ADS_TYPE);
    void onAdsClicked(ADS_TYPE);
    void onAdsExpanded(ADS_TYPE);
    void onAdsCollapsed(ADS_TYPE);
    void onAdsRewarded(std::string, int, bool);
#pragma mark --ad delegate end--
    
    static LoadingLayerBase* _adInstance;
    
protected:
    LoadingLayerBase(){}
    virtual ~LoadingLayerBase();
    
    bool showRewardAds();
    
    void registerAdsCallback();
    void unRegisterAdsCallback();

    ADS_TYPE   m_eType;
    
    bool _willShowAds = true;
    
    bool _needShowAds = false;
};

#define REWARD_CALLBACK     "reward_callback"
#define kSceneManager       SceneManager::getInstance()
#define kAdsManager         AdsManager::getInstance()
#define kAdInstance         LoadingLayerBase::_adInstance

#endif /* defined(CODEBASE_COMPONENT_SCENEMANAGER_SCENEMANAGER_H_) */
