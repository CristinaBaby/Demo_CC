#include "LoadingScene.h"
#include "CMVisibleRect.h"
#include "SceneManager.h"
#include "cocostudio/CCArmature.h"
#include "ConfigManager.h"
#include "AdsManager.h"
#include "PolygonHelp.h"

USING_NS_CC;
using namespace cocostudio;

Scene* LoadingScene::scene(){
    Scene* scene = Scene::create();
    LoadingScene* layer = LoadingScene::create();
    scene->addChild(layer);
    return scene;
}

bool LoadingScene::init(){
    if (!LayerColor::initWithColor(Color4B(255, 125, 1,255))) {
        return false;
    }
    m_nCount = 0;
    
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("loading/loading.plist");
    
    auto sprite3 = Sprite::createWithSpriteFrameName("001.png");
    CMVisibleRect::setPositionAdapted(sprite3, 320, 480);
    this->addChild(sprite3);
    
    Vector<cocos2d::SpriteFrame *> arr;
    char str[100] = {0};
    for (int i=1; i<10; i++)
    {
        sprintf(str, "00%d.png", i);
        auto frame_2 = cache->SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        arr.pushBack(frame_2);
    }
    for (int i=10; i<55; i++)
    {
        if (i==29) {
            continue;
        }
        sprintf(str, "0%d.png", i);
        auto frame_2 = cache->SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        arr.pushBack(frame_2);
    }
    
    
    
    auto animation3 = Animation::createWithSpriteFrames(arr, 0.1f);
    sprite3->runAction(Sequence::create(Animate::create(animation3),
                                        CallFunc::create(CC_CALLBACK_0(LoadingScene::onCallBack, this)),NULL));
    
    
//    auto func = CallFunc::create([&](){
//        
//        AudioHelp::getInstance()->playEffect("loading.mp3");
//        
//    });
//    ActionUtils::delayTimeAction(this,0.3,func);
    
    this->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(LoadingScene::preLoad, this)),
                                     CallFunc::create(CC_CALLBACK_0(LoadingScene::onCallBack, this)),
                                     NULL));
    AudioHelp::getInstance()->playEffect("loading.mp3");
    
    return true;
}

void LoadingScene::preLoad()
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #else
    ShopAdapter::getInstance()->registerAndroidIAP();
    #endif
    
    FileUtils::getInstance()->addSearchPath("res/");
    PolygonHelp::getInstance()->loadFile("config/polygon.plist");
    ConfigManager::getInstance()->loadFile("config/config.plist");
    //    IAPManager初始化要放在ConfigManager后面
    GameDataManager::getInstance()->setup(); //要放在ConfigManager后面
    ConfigManager::getInstance()->restoreDecorateConfigData();
    
//    AdsManager::getInstance()->preloadAllAds();
    
//    IAPManager::getInstance()->loadIapInfo("Config/IAPConfig.plist");
    
    
    //    AudioHelp::getInstance()->setAudioVolumeMode(true);
    
}

void LoadingScene::onCallBack()
{
    m_nCount++;
    if (m_nCount>=2) {
        
//        GameDataManager::getInstance()->m_nFlavorIndex = 3;
//        GameDataManager::getInstance()->m_nShapeIndex = 3;
        SceneManager::replaceTheScene<HomeScene>();
    }
}

