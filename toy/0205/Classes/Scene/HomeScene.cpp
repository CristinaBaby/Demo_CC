//
//  HomeScene.cpp
//  BabyPlay
//
//  Created by luoxp on 3/18/15.
//
//

#include "HomeScene.h"
#include "AppConfigs.h"
#include "CBSettingLayer.h"
#include "PackLayer.h"
#include "UICpp.h"
#include "../Module/STSystemFunction.h"
#include "CBAppManager.h"
#include "ChooseScene.h"
#include "AsyncManager.h"
#include "ExitLayer.h"


HomeScene::~HomeScene(){
    
}

bool HomeScene::init(){
    
    if(Scene::init())
    {
        auto _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("play.json"));
        addChild(_root);
        
        _playBtn = quickButton("logo", _root, CC_CALLBACK_2(HomeScene::onButton, this));
        quickButton("setting", _root, CC_CALLBACK_2(HomeScene::onButton, this));
        quickButton("monkey", _root, CC_CALLBACK_2(HomeScene::onButton, this));
        
        ArmatureDataManager::getInstance()->removeArmatureFileInfo("Choose_2.ExportJson");
        ArmatureDataManager::getInstance()->addArmatureFileInfo("Home.ExportJson");
        auto pos = Vec2(Director::getInstance()->getWinSize()/2) + Vec2(12, -15);
        
        //animation
        auto sun = quickGetAnimation(5, "Home", pos);
        _root->addChild(sun, 0);
        
        auto girl = quickGetAnimation(4, "Home", pos);
        _root->addChild(girl, 41);
        
        auto plant = quickGetAnimation(2, "Home", pos + Vec2(2, -3));
        _root->addChild(plant, 41);
        
        auto monkey = quickGetAnimation(0, "Home", pos);
        _root->addChild(monkey, 41);
        
        auto bird = quickGetAnimation(1, "Home", pos);
        _root->addChild(bird, 41);
        
        _butterfly = quickGetAnimation(3, "Home", Vec2(220, 105));
        _playBtn->addChild(_butterfly, 41);
        _butterfly->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(HomeScene::onMovementEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        
        auto listener = EventListenerKeyboard::create();
        listener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* event)
        {
            if(code == EventKeyboard::KeyCode::KEY_BACK)
            {
                auto settingLayer = getChildByName("settingLayer");
                if (settingLayer != nullptr) {
                    removeChild(settingLayer);
                }
                else
                {
                    auto exitLayer = getChildByName("exitLayer");
                    if (exitLayer == nullptr) {
                        auto exit = ExitLayer::create();
                        addChild(exit, 1);
                    }
                }
            }
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
        //////////////////////////////////////////////////////////// add next scene resource ////////////////////////////////////////////////////////////
        xAsync->add("animation/Model01.png");
        xAsync->add("animation/Model02.png");
        xAsync->add("choose-1.json");
        xAsync->add("Choose_1.ExportJson");
        xAsync->add("Book0.png");
        xAsync->add("Book1.png");
        xAsync->add("Book2.png");
        xAsync->add("Book3.png");
        xAsync->add("Book4.png");
        xAsync->add("Book5.png");
        xAsync->add("Book6.png");
        
        xAudio->playMusic("bg/bg_1.mp3");
        xAds->setVisiable(kTypeBannerAds, false);
        xAdsEx->setBannerShow(false);
        return true;
    }

    return false;
}

void HomeScene::onEnterTransitionDidFinish()
{
    if(_beBack){
        xApp->requestCrossAd(this, 10);
    }
    
}

void HomeScene::onMovementEvent(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
    if (movementType == COMPLETE)
    {
        armature->getAnimation()->gotoAndPlay(60);
        _dock = true;
    }
}

void HomeScene::onButton(Ref* pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::BEGAN)
    {
        string name = ((Node*)pSender)->getName();
        if (name.compare("logo") == 0 && _dock) {
            auto action = Repeat::create(
                                         Sequence::create(
                                                          Spawn::create(MoveBy::create(0.8f, Vec2(5, -5)), MoveBy::create(0.8f, Vec2(-5, -5)), NULL),
                                                          MoveBy::create(1.5f, Vec2(10, 10)),
                                                          Spawn::create(MoveBy::create(1.2f, Vec2(-5, -5)), MoveBy::create(1.2f, Vec2(-5, 5)), NULL),
                                                          nullptr),1);
            _butterfly->runAction(action);
        
        }
    }
    
    if (type != Widget::TouchEventType::ENDED) return;
    
    string name = ((Node*)pSender)->getName();
    
    if (name == "setting") {
        xAudio->playSound("sfx/sfx_pop.mp3");
    } else {
        xAudio->playDefaultSound();
    }
    
    if (name.compare("logo") == 0) {
        auto scene = ChooseScene::create(true);
        Director::getInstance()->replaceScene(scene);
    }
    else if(name.compare("setting") == 0)
    {
        settingAction();
    }
    else if (name.compare("monkey") == 0)
    {
        moreAction();
    }
}

void HomeScene::settingAction(){
    SettingLayer* layer = SettingLayer::create();
    layer->setPosition(Point::ZERO);
    addChild(layer);
}

void HomeScene::parentalGatePass(int tag)
{
    if (tag == 1)
    {
        //for more game
        STSystemFunction *sf = new STSystemFunction;
        sf->showMoreGame();
        CC_SAFE_DELETE(sf);
        log("More Action!");
    }
    else if(tag == 2)
    {
        //for restore
        log("restore Action!");
        
        _billing.setIABDelegate(this);
        _billing.restore();
    }
}

void HomeScene::moreAction(){
    
    STSystemFunction *sf = new STSystemFunction;
    sf->showMoreGame();
    CC_SAFE_DELETE(sf);
    
    return;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ParentalGateLayer *layer = ParentalGateLayer::create();
    layer->setDelegate(this, 1);
    layer->showInLayer(this);
#else
    STSystemFunction *sff = new STSystemFunction;
    sff->showMoreGame();
    CC_SAFE_DELETE(sff);
    log("More Action!");
#endif
}

#pragma mark- Quit game for android

void HomeScene::quitGame()
{
    Director::getInstance()->end();
    
    exit(0);
}

void HomeScene::cancelQuitGame()
{
    xAds->removeAds(kTypeBannerAds);
}

#pragma mark- STIABDelegate

void HomeScene::purchaseSuccessful(const char* pid)
{
    log("MX: Purchase success!");
}

void HomeScene::purchaseFailed(const char *pid, int errorCode)
{
    log("MX: Purchase failed!");
}

void HomeScene::restoreSuccessful(const char* pid)
{
    if (std::string(pid) == iap_1)
    {
        //ColouringManager::getInstance()->unlockPatternPen();
    }
    else
    {
        std::string userDefaultKey = UserDefaultKey_HasPurchasedPack + std::string(pid);
        UserDefault::getInstance()->setBoolForKey(userDefaultKey.c_str(), true);
        UserDefault::getInstance()->flush();
    }
    
    log("MX: Restore success!");
}

void HomeScene::restoreFailed(const char* pid, int errorCode)
{
    log("MX: Restore failed!");
}

ActionInterval * quickButterflyAnimation(int animationIndex, const string &name, const Vec2 &pos)
{
    Armature *armature = Armature::create(name);
    armature->getAnimation()->playWithIndex(animationIndex);
    armature->setPosition(pos);
    
    return nullptr;
}


