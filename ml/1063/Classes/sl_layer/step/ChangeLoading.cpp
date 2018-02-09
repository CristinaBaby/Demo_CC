//
//  ChangeLoading.cpp
//  DeepFryAnythin
//
//  Created by luotianqiang1 on 16/1/20.
//
//

#include "ChangeLoading.h"
//#include "DrawLine.h"
#include "SimpleAudioEngine.h"
#include "AdsManager.h"
#include "Global.h"
#include "MainScene.h"
#include "AdapterScreen.h"
REGIST_SECENE(ChangeLoading)
using namespace CocosDenshion;


ChangeLoading::ChangeLoading():endFunc(nullptr){
    
}

void ChangeLoading::loading(std::function<void()> end){
    auto _scene = Scene::create();
    auto _layer = ChangeLoading::create();
    _layer->endFunc = end;
    _scene->addChild(_layer);
    Director::getInstance()->popToRootScene();
    Director::getInstance()->replaceScene(_scene);
}

void ChangeLoading::onEnter()
{
      AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds,false);
    Layer::onEnter();
    this->runAction(Sequence::create(DelayTime::create(0.1),CallFunc::create(CC_CALLBACK_0(ChangeLoading::show, this)),nullptr));
    //show();
//    m_pBeads->runAction(Sequence::create(MoveBy::create(1.0, Vec2(0,-500)),CallFunc::create([=](){
//    
//        show();
//    
//    }), NULL));
    
}
void ChangeLoading::onExit()
{
    Layer::onExit();
      AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds,true);
//    AdsManager::getInstance()->onAdsExpanded=nullptr;
//    AdsManager::getInstance()->onAdsCollapsed=nullptr;
//    
}
void ChangeLoading::onEnterTransitionDidFinish(){
    Layer::onEnterTransitionDidFinish();
}
bool ChangeLoading::init(){
    if(!Layer::init())
        return false;

    m_pBg = Sprite::create(g_path + "loadingAnimation/bg.jpg");
    m_pBg->setPosition(Director::getInstance()->getVisibleSize()/2);
    this->addChild(m_pBg);
    

    
    return true;
}

void ChangeLoading::show(){
    
    cocostudio::ArmatureDataManager::getInstance() -> addArmatureFileInfo("cocostudio/png/Home page/ACG/wedding/wedding/wedding.ExportJson");
    m_pArmature = cocostudio::Armature::create("wedding");
    
    m_pArmature->setPosition(Vec2(390,600 - 50));
    m_pArmature->setScale(0.8);
    this->addChild(m_pArmature);
    
    auto table = Sprite::create(g_path + "loadingAnimation/table.png");
    table->setAnchorPoint(Vec2(0.5,0));
    table->setPosition(Director::getInstance()->getVisibleSize().width/2,0 - 150);
    this->addChild(table);
    
    
    m_pCurtain = Sprite::create(g_path + "loadingAnimation/curtain0.png");
    m_pCurtain->setPosition(Director::getInstance()->getVisibleSize()/2);
    this->addChild(m_pCurtain);
    
    m_pBeads = Sprite::create(g_path + "loadingAnimation/beads.png");
    m_pBeads->setAnchorPoint(Vec2(0.5,1));
    m_pBeads->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height + 500));
    this->addChild(m_pBeads);
    
    
    auto pOvenAnimation = Animation::create();
    pOvenAnimation->addSpriteFrameWithFile(g_path + "loadingAnimation/curtain0.png");
    pOvenAnimation->addSpriteFrameWithFile(g_path + "loadingAnimation/curtain1.png");
    pOvenAnimation->addSpriteFrameWithFile(g_path + "loadingAnimation/curtain2.png");
    pOvenAnimation->addSpriteFrameWithFile(g_path + "loadingAnimation/curtain3.png");
    pOvenAnimation->addSpriteFrameWithFile(g_path + "loadingAnimation/curtain4.png");
    pOvenAnimation->addSpriteFrameWithFile(g_path + "loadingAnimation/curtain5.png");
    pOvenAnimation->setDelayPerUnit(0.25);
    pOvenAnimation->setLoops(1);
    
    auto pOvenAnimat = Animate::create(pOvenAnimation);
    
    m_pCurtain->runAction(Sequence::create(pOvenAnimat,CallFunc::create([=](){
    
        m_pBeads->runAction(Sequence::create(MoveBy::create(1.0, Vec2(0,-500)),CallFunc::create([=](){
            
            m_pArmature->getAnimation()->play(__String::createWithFormat("Animation%d",rand()%4 + 1)->getCString(),-1,0);
            
        }), NULL));
        
    
    }),DelayTime::create(1.5),CallFunc::create([=](){
    
        //回调
        if(endFunc != nullptr){
            
            endFunc();
                
        }
        
    }),nullptr));
}

