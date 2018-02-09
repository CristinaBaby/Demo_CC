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
#include "CBPackScene.h"
#include "UICpp.h"
#include "../Module/STSystemFunction.h"
#include "CBAppManager.h"
#include "AsyncManager.h"
#include "CBAppManager.h"


HomeScene::~HomeScene(){
    
}

bool HomeScene::init(){
    
    if(Scene::init())
    {
        setName("Home");
        
        auto _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("start.json"));
        addChild(_root);
        _root->cocos2d::Node::visit();
        
        _play = quickButton("play", _root, CC_CALLBACK_2(HomeScene::onButton, this));
        quickButton("game", _root, CC_CALLBACK_2(HomeScene::onButton, this));
        
        auto setting = quickButton("setting", _root, CC_CALLBACK_2(HomeScene::onButton, this));
        xApp->setBackPosition(setting->getWorldPosition());
        
        //////////////////////////////////////////////////////////// add next scene resource ////////////////////////////////////////////////////////////
        xAudio->playMusic("bg/bg_01.mp3");
        xAds->setVisiable(kTypeBannerAds, false);
        
        auto listener = EventListenerKeyboard::create();
        listener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* event)
        {
            if(code == EventKeyboard::KeyCode::KEY_BACK)
            {
                auto settingLayer = getChildByName("settingLayer");
                if (settingLayer != nullptr) {
                    xAds->setVisiable(kTypeBannerAds, false);
                    removeChild(settingLayer);
                }
                else
                {
                    xDialog->show("text2", "no", "yes", CC_CALLBACK_1(HomeScene::onDialog, this));
                }
            }
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        return true;
    }

    return false;
}

void HomeScene::onDialog(const string& name)
{
    if(name == "yes"){
        xAudio->stopMusic();
        Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    }
    else if (name.compare("no") == 0)
    {
        removeFromParent();
    }
}

void HomeScene::onEnterTransitionDidFinish()
{
    if(_beBack){
        xApp->requestCrossAd(this, 101);
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
    if (type != Widget::TouchEventType::ENDED || _block) return;
    
    string name = ((Node*)pSender)->getName();
    
    if (name == "setting") {
        xAudio->playSound("sfx/sfx_pop.mp3");
    } else {
        xAudio->playDefaultSound();
    }
    
    if (name.compare("play") == 0) {
        _block = true;
        _play->setTouchEnabled(false);
        xAds->setVisiable(kTypeBannerAds, true);
        auto scene = PackScene::create();
        Director::getInstance()->replaceScene(scene);
    }
    else if(name.compare("setting") == 0)
    {
        settingAction();
    }
    else if (name.compare("game") == 0)
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
    //AnalyticX::flurryEndSession();
    Director::getInstance()->end();
    exit(0);
}

ActionInterval * quickButterflyAnimation(int animationIndex, const string &name, const Vec2 &pos)
{
    Armature *armature = Armature::create(name);
    armature->getAnimation()->playWithIndex(animationIndex);
    armature->setPosition(pos);
    
    return nullptr;
}


