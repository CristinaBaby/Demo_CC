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
#include "CFSystemFunction.h"
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
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("images/loading/loading.ExportJson");
    
    
    Armature* m_pModel = Armature::create("loading");
    m_pModel->setScale(0.7f);
    m_pModel->ignoreAnchorPointForPosition(false);
    m_pModel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_pModel->getAnimation()->play("Animation1");
    kAdapterScreen->setExactPosition(m_pModel, 480, 320);
    this->addChild(m_pModel);
    
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
