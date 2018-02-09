//
//  LoadingLayer.cpp
//  OreoMaker
//
//  Created by huxixiang on 15-2-1.
//
//

#include "LoadingLayer.h"
#include "AdapterScreen.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "AdsManager.h"
#include "SoundPlayer.h"
#include "SSCInternalLibManager.h"
#include "GameData.h"

using namespace cocostudio;

Scene* LoadingLayer::scene()
{
    Scene* pScene = Scene::create();
    auto pLayer = LoadingLayer::create();
    pScene->addChild(pLayer);
    
    return pScene;
}

LoadingLayer::LoadingLayer()
{
    
}

LoadingLayer::~LoadingLayer()
{
    ArmatureDataManager::getInstance()->removeArmatureFileInfo("images/loading/loading.ExportJson");
}

bool LoadingLayer::init()
{
    bool isInit = false;
    
    do{
        CC_BREAK_IF(!Layer::init());
        
        CC_BREAK_IF(!preLoadingData());
        
        CC_BREAK_IF(!initUI());
        
        isInit = true;
        
    }while (0);
    
    return isInit;
}

bool LoadingLayer::preLoadingData()
{
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(startBgMusick);
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(bgMusick);
    return true;
}

bool LoadingLayer::initUI()
{
    Sprite* bgSprite = Sprite::create("images/loading/loading_bg.jpg");
    kAdapterScreen->setExactPosition(bgSprite, 480, 320);
    this->addChild(bgSprite);
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("images/loading/loading0.png", "images/loading/loading0.plist", "images/loading/loading.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("images/loading/loading1.png", "images/loading/loading1.plist", "images/loading/loading.ExportJson");
    
    
    Armature* lArmature = Armature::create("loading");
    kAdapterScreen->setExactPosition(lArmature, 480, 320);
    this->addChild(lArmature);
    ArmatureAnimation* lArmation = lArmature->getAnimation();
//    lArmation->playWithIndex(0,-1,0);
    
    
    this->runAction(Sequence::create(DelayTime::create(5.5f), CallFunc::create([=](){
    
        SceneManager::getInstance()->enterHomeScene();
    }), nullptr));
    
    return true;
}

void LoadingLayer::onEnter()
{
    Layer::onEnter();
    AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
}

void LoadingLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    SoundPlayer::getInstance()->playEffect("sounds/loading.mp3");
}
