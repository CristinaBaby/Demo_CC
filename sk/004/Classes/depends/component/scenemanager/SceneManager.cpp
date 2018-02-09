//
//  SceneManager.cpp
//
//  Created by zhangguangzong1 on 15/7/3.
//
//

#include "depends/component/scenemanager/SceneManager.h"

static SceneManager* instance = nullptr;

static void replaceScene(Scene* apScene)
{
    if(apScene == nullptr)
        return;
    if( Director::getInstance()->getRunningScene() ){
        Director::getInstance()->replaceScene(apScene);
    }
    else{
        Director::getInstance()->runWithScene(apScene);
    }
}

static void pushScene(Scene* apScene)
{
    if(apScene == nullptr)
        return;
    if( Director::getInstance()->getRunningScene() ){
        Director::getInstance()->pushScene(apScene);
    }
    else{
        Director::getInstance()->runWithScene(apScene);
    }
}

void SceneManager::replaceWithDefaultTransition(Scene* pScene)
{
    TransitionFade* fade = TransitionFade::create(1.0, pScene);
    replaceScene(fade);
}

void SceneManager::replaceWithTransition(cocos2d::TransitionScene *pTranstionScene)
{
    replaceScene(pTranstionScene);
}

void SceneManager::pushWithDefaultTransition(Scene* pScene)
{
    TransitionFade* fade = TransitionFade::create(1.0, pScene);
    pushScene(fade);
}

void SceneManager::pushWithTransition(cocos2d::TransitionScene *pTranstionScene)
{
    pushScene(pTranstionScene);
}

void SceneManager::popScene()
{
    Director::getInstance()->popScene();
}

SceneManager* SceneManager::getInstance()
{
    if (!instance)
    {
        instance = new SceneManager();
    }
    return instance;
}

SceneManager::SceneManager()
{
    _loadingMinTime = 2;
    _loadingMaxTime = 5;
}

SceneManager::~SceneManager(){}

void SceneManager::pushLoaing(Layer* loadingLayer)
{
    LoadingScene* loading = LoadingScene::create();
    loading->addLoadingLayer(loadingLayer);
    kSceneManager->pushLoaing(TransitionFade::create(0.5f, loading, Color3B::WHITE));
}

void SceneManager::replaceLoaing(Layer* loadingLayer)
{
    LoadingScene* loading = LoadingScene::create();
    loading->addLoadingLayer(loadingLayer);
    kSceneManager->replaceLoaing(TransitionFade::create(0.5f, loading, Color3B::WHITE));
}

Scene* SceneManager::getLoadingScene(Layer* loadingLayer)
{
    LoadingScene* loading = LoadingScene::create();
    loading->addLoadingLayer(loadingLayer);
    return loading;
}

void SceneManager::pushLoaing(Scene* scene)
{
    Scene* curScene = Director::getInstance()->getRunningScene();
    /*判断：当前场景还在播放过渡场景动画时，push失败。*/
    bool runningIsTransition = dynamic_cast<TransitionScene*>(curScene) != nullptr;
    if (runningIsTransition)
        return;
    /*判断：当前场景是LoadingScene的情况下，就不重复push了*/
    LoadingScene* transition = dynamic_cast<LoadingScene*>(curScene);
    if (transition)
    {
        return;
    }
    
    _timeOk = false;
    _showAds = false;
    kSceneManager->setAdClose(false);
    
    Director::getInstance()->pushScene(scene);
    
    gettimeofday(&_timeEnter, nullptr);
    
    Director* director = Director::getInstance();
    director->getScheduler()->schedule(schedule_selector(SceneManager::timeCheckSchedule),
                                       this,
                                       0.25f,    //每0.25s检测一次是否可以进入下一场景
                                       false);
}

void SceneManager::replaceLoaing(Scene* scene)
{
    _timeOk = false;
    _showAds = false;
    kSceneManager->setAdClose(false);
    Director::getInstance()->replaceScene(scene);
    
    gettimeofday(&_timeEnter, nullptr);
    
    Director* director = Director::getInstance();
    director->getScheduler()->schedule(schedule_selector(SceneManager::timeCheckSchedule),
                                       this,
                                       0.25f,    //每0.25s检测一次是否可以进入下一场景
                                       false);
}

bool SceneManager::gotoNextScene()
{
    Scene* scene = Director::getInstance()->getRunningScene();
    /*看下当前的场景是否是过渡场景，如果是的话，表示过渡动画还未播放完成,不进入下一个场景*/
    bool runningIsTransition = dynamic_cast<TransitionScene*>(scene) != nullptr;
    if (runningIsTransition)
    {
        return false;   //当前场景还是过渡场景，还没播放完过渡动画
    }
    
    kSceneManager->exitLoading();
    
    LoadingScene* transition = dynamic_cast<LoadingScene*>(scene);
    if (transition)
    {
        Scene* nextScene = this->getNextScene();
        if (nextScene)
        {
            /*过渡场景肯定是不保留的，直接replace*/
            Director::getInstance()->replaceScene(nextScene);
        }else
        {
            /*过渡场景没有设置下一个场景直接pop即可*/
            Director::getInstance()->popScene();
        }
        this->setNextScene(nullptr);
    }else
    {
        log("当前场景不是LoadingScene类,直接调用popScene弹出");
        Director::getInstance()->popScene();
    }
    return true;
}

void SceneManager::timeCheckSchedule(float dt)
{
    struct timeval now;
    gettimeofday(&now, nullptr);
    float lTimePasses = now.tv_sec - _timeEnter.tv_sec;
    if ( lTimePasses >= _loadingMinTime)
    {
        _timeOk = true;
    }
    if (lTimePasses >=_loadingMaxTime)
    {
        /*如果超时了，强制标识为加载操作成功进入下一个界面
         *加载操作完成包括资源加载完成,广告显示完成(包括成功或失败)
         */
        if (!_showAds)
        {
            _loadOk = true;
            _adClose = true;  //超时后没显示广告,直接强制关闭广告
        }
    }
    
    this->checkGotoNext();
}

void SceneManager::checkGotoNext()
{
//    log("_timeOk = %d,_loadOk = %d,_adClose = %d",_timeOk,_loadOk,_adClose);
    if (_timeOk && _loadOk && _adClose)
    {
        if (_fnLoadingClose)
        {
            kSceneManager->exitLoading();
            Scheduler* scheduler = Director::getInstance()->getScheduler();
            scheduler->unschedule(schedule_selector(SceneManager::timeCheckSchedule), this);
            
            _fnLoadingClose();
            
            this->setLoadingCloseCall(nullptr);
            this->setNextScene(nullptr);
            return;//设置了loading关闭回调，优先执行回调，不自动跳入下个场景
        }
        bool gotoNext = this->gotoNextScene();
        if (gotoNext)
        {
            Scheduler* scheduler = Director::getInstance()->getScheduler();
            scheduler->unschedule(schedule_selector(SceneManager::timeCheckSchedule), this);
        }
    }
}

Scene* SceneManager::getNextScene()
{
    if (_fnGetNextScene)
    {
        return _fnGetNextScene();
    }
    return nullptr;
}

void SceneManager::setNextScene(std::function<Scene*()> fn)
{
    _fnGetNextScene = fn;
}

void SceneManager::setLoadingCloseCall(std::function<void()> fn)
{
    _fnLoadingClose = fn;
}

/*
 *@imp LoadingScene
 */
bool LoadingScene::init()
{
    if (!Scene::init())
    {
        return false;
    }
    
    kSceneManager->loadingComplete();
    
    return true;
}

void LoadingScene::onEnterTransitionDidFinish()
{
    Scene::onEnterTransitionDidFinish();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void LoadingScene::addLoadingLayer(Node* loading)
{
    if(!loading->getParent())
        this->addChild(loading);
}

void LoadingScene::loadRes(const string& path)
{
    if (!m_bCancelComplete)
    {
        m_bCancelComplete = true;
        kSceneManager->resLoading();
    }
    m_iLoadingCount++;
    Director::getInstance()->getTextureCache()->addImageAsync(path,CC_CALLBACK_1(LoadingScene::loadCallback, this));
}

void LoadingScene::loadCallback(Texture2D* tex)
{
    m_iCompleteCount++;
    if(m_iCompleteCount >= m_iLoadingCount)
    {
        kSceneManager->loadingComplete();    //加载结束
    }
}

/*
 *@imp LoadingLayerBase
 */

LoadingLayerBase* LoadingLayerBase::_adInstance = nullptr;

LoadingLayerBase* LoadingLayerBase::create()
{
    LoadingLayerBase* layer = new LoadingLayerBase();
    if (layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    layer = nullptr;
    return nullptr;
}

LoadingLayerBase::~LoadingLayerBase()
{
}

bool LoadingLayerBase::init()
{
    Layer::init();
    

    CC_SAFE_RELEASE(_adInstance);
    _adInstance = this;
    CC_SAFE_RETAIN(_adInstance);
    
    return true;
}

void LoadingLayerBase::onExit()
{
    Layer::onExit();
    this->unRegisterAdsCallback();
    kSceneManager->exitLoading();
    _needShowAds = false;
}

void LoadingLayerBase::onEnter()
{
    Layer::onEnter();
}

void LoadingLayerBase::onEnterTransitionDidFinish()
{
    kSceneManager->enterLoading();
    if (!_needShowAds)
    {
        kSceneManager->setAdClose(true);
        return;
    }
    if (m_eType == ADS_TYPE::kTypeRewardedAds)
    {
        return;
    }
    auto showCall = [this]()
    {
        kAdsManager->showAds(m_eType);
        switch (m_eType)
        {
            case ADS_TYPE::kTypeInterstitialAds :
                kSceneManager->setAdClose(false);
                break;
//            case ADS_TYPE::kTypeInterstitialAds:
//                kSceneManager->setAdClose(false);
//                break;
            case ADS_TYPE::kTypeBannerAds :
//            case ADS_TYPE::kTypeRectAds :
            case ADS_TYPE::kTypeRewardedAds :
                break;
            default:
                break;
        }
    };
    this->runAction(Sequence::create(CallFunc::create(showCall), nullptr));
}

bool LoadingLayerBase::showRewardAds()
{
    bool isCache = kAdsManager->showAds(kTypeRewardedAds);
    if (isCache)
    {
//        kSceneManager->setShowRewardingAds(true);
//        kSceneManager->setAdsShow();
//        kSceneManager->setAdClose(false);
    }
    else
    {
        this->closeRewardAds("\0",      //空字符串，没有ID,
                             0,         //无意义，前两个参数目前都没在项目中用到
                             true,      //异常情况退出，跳过广告标识为true;
                             SceneManager::RewardCode::kNoCache);//没有缓存广告的情况
        
        kSceneManager->setNextScene(nullptr);
    }
    
    return isCache;
}

void LoadingLayerBase::showAd(ADS_TYPE type,bool isReume)
{
    if(!_willShowAds)
    {
        _willShowAds = true;
        kSceneManager->enterForeground();
        return;
    }
    
    if (kSceneManager->isShowRewardingAds())
    {
        return;
    }
    
    this->registerAdsCallback();
    if (type == ADS_TYPE::kTypeRewardedAds)
    {
        if (!showRewardAds())
        {
            return;
        }
    }
    /*根据当前的父节点和当前的scene来判断来进行判断*/
    Node* parent = this->getParent();
    if (!parent)
    {
        //父节点为空的时候，需要添加一个loadingScene
        LoadingScene* loading = LoadingScene::create();
        loading->addLoadingLayer(this);
        kSceneManager->pushLoaing(loading);
    }
    
    m_eType = type;
    _needShowAds = true;
}

void LoadingLayerBase::registerAdsCallback()
{
    kAdsManager->onAdsClicked = CC_CALLBACK_1(LoadingLayerBase::onAdsClicked, this);
    kAdsManager->onAdsCollapsed = CC_CALLBACK_1(LoadingLayerBase::onAdsCollapsed, this);
    kAdsManager->onAdsExpanded = CC_CALLBACK_1(LoadingLayerBase::onAdsExpanded, this);
    kAdsManager->onAdsLoaded = CC_CALLBACK_1(LoadingLayerBase::onAdsLoaded, this);
    kAdsManager->onAdsLoadFailed = CC_CALLBACK_2(LoadingLayerBase::onAdsLoadFailed, this);
    kAdsManager->onAdsRewarded = CC_CALLBACK_3(LoadingLayerBase::onAdsRewarded, this);
}

void LoadingLayerBase::unRegisterAdsCallback()
{
    kAdsManager->onAdsClicked = nullptr;
    kAdsManager->onAdsCollapsed = nullptr;
    kAdsManager->onAdsExpanded = nullptr;
    kAdsManager->onAdsLoaded = nullptr;
    kAdsManager->onAdsLoadFailed = nullptr;
    kAdsManager->onAdsRewarded = nullptr;
}

void LoadingLayerBase::onAdsLoaded(ADS_TYPE type)
{
    if (kSceneManager->isInloading())
    {
        if (m_eType == type && m_eType != kTypeRewardedAds)
        {
            kAdsManager->showAds(type);
        }
    }
}

void LoadingLayerBase::onAdsLoadFailed(std::string errorMsg, ADS_TYPE type)
{
    switch (type)
    {
        case ADS_TYPE::kTypeInterstitialAds :
//        case ADS_TYPE::kTypeInterstitialAds :
        {
            kSceneManager->setAdClose(true);
            break;
        }
        case ADS_TYPE::kTypeRewardedAds :
        {
            this->closeRewardAds("\0", 0, true, SceneManager::RewardCode::kLoadFailed);
            break;
        }
        case ADS_TYPE::kTypeBannerAds :
//        case ADS_TYPE::kTypeRectAds :
        {
            /*暂时不知道Rect广告的需求形式，是否在loading上，暂不处理*/
            break;
        }
        default:
        {
            break;
        }
    }
}

void LoadingLayerBase::onAdsClicked(ADS_TYPE type)
{
}

void LoadingLayerBase::onAdsExpanded(ADS_TYPE type)
{
    switch (type)
    {
        case ADS_TYPE::kTypeInterstitialAds:
//        case ADS_TYPE::kTypeInterstitialAds:
        {
            kSceneManager->setAdClose(false);
            kSceneManager->setAdsShow();
            break;
        }
        case ADS_TYPE::kTypeRewardedAds :
        {
            kSceneManager->setShowRewardingAds(true);
            kSceneManager->setAdsShow();
            kSceneManager->setAdClose(false);
            break;
        }
        case ADS_TYPE::kTypeBannerAds :
//        case ADS_TYPE::kTypeRectAds :
        {
            /*暂时不知道广告的需求形式，是否在loading上，暂不处理*/
            break;
        }
        default:
        {
            break;
        }
    }
}

void LoadingLayerBase::onAdsCollapsed(ADS_TYPE type)
{
    switch (type)
    {
        case ADS_TYPE::kTypeInterstitialAds:
//        case ADS_TYPE::kTypeInterstitialAds:
        {
            kSceneManager->setAdClose(true);
            if (kSceneManager->isEnterBackground())
            {
                _willShowAds = false;
            }
            this->unRegisterAdsCallback();
            break;
        }
        case ADS_TYPE::kTypeRewardedAds :
        {
            //奖励回调要在几秒后才回执行，给5s缓冲时间，5s后没有回调强制关闭loading
            this->scheduleOnce(schedule_selector(LoadingLayerBase::rewardCallbackCheck), 5.0f);
            break;
        }
        case ADS_TYPE::kTypeBannerAds :
//        case ADS_TYPE::kTypeRectAds :
        {
            /*暂时不知道Rect广告的需求形式，是否在loading上，暂不处理*/
            break;
        }
        default:
        {
            break;
        }
    }
}

void LoadingLayerBase::onAdsRewarded(std::string productid, int count, bool isSkip)
{
    this->unschedule(schedule_selector(LoadingLayerBase::rewardCallbackCheck));
    this->closeRewardAds(productid, count, isSkip, SceneManager::RewardCode::kRewardOk);
}

void LoadingLayerBase::rewardCallbackCheck(float dt)
{
    this->closeRewardAds("\0",
                         0,
                         false,//回调reward超时的情况，我们目前默认处理为:没有跳过广告
                         SceneManager::RewardCode::kRewardTimeOut);
}

void LoadingLayerBase::closeRewardAds(const string& productid,int count,bool isSkip,
                                      SceneManager::RewardCode code)
{
    RewardAdsInfo info;
    info.isSkip = isSkip;
    info.productId = productid;
    info.resultCode = code;
    _eventDispatcher->dispatchCustomEvent(REWARD_CALLBACK,&info);
    
    this->unRegisterAdsCallback();
    kSceneManager->setShowRewardingAds(false);
    kSceneManager->setAdClose(true);
    if (kSceneManager->isEnterBackground())
    {
        _willShowAds = false;
    }
}