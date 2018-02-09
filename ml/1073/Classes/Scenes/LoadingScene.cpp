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
    if (!LayerColor::init()) {
        return false;
    }
    m_nCount = 0;
    
    Size size = Director::getInstance()->getVisibleSize();
    Sprite* bg = Sprite::create("loading/loading.jpg");
    bg->setPosition(Point(size.width/2, size.height/2));
    this->addChild(bg);
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("loading/loading0.png", "loading/loading0.plist", "loading/loading.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("loading/loading1.png", "loading/loading1.plist", "loading/loading.ExportJson");
    Armature* lArmature = Armature::create("loading");
    lArmature->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2-200));
    this->addChild(lArmature);
    ArmatureAnimation* lArmation = lArmature->getAnimation();
    lArmation->playWithIndex(0,-1,0);
    
    lArmation->setMovementEventCallFunc([=](Armature *, MovementEventType type, const std::string &){
        if (type==MovementEventType::COMPLETE) {
            onCallBack();
        }
    });
    
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
    
    if(!UserDefault::getInstance()->getBoolForKey("setup", false)){
        ConfigManager::getInstance()->setVidioUnlocked("pack", 0);
        UserDefault::getInstance()->setBoolForKey("pack0",true);
        UserDefault::getInstance()->setBoolForKey("setup", true);
    }
    
//    IAPManager::getInstance()->loadIapInfo("Config/IAPConfig.plist");
    
    
    //    AudioHelp::getInstance()->setAudioVolumeMode(true);
    
}

void LoadingScene::onCallBack()
{
    m_nCount++;
    if (m_nCount>=2) {
        
        SceneManager::replaceTheScene<HomeScene>();
    }
}

