//
//  HomeScene.h
//  Bedtime Story Mermaid
//
//  Created by Luo Xiaopeng.
//
//
#include "HomeScene.h"
#include "../AppGlobal.h"
#include "../utilities/STVisibleRect.h"
#include "../utilities/UICpp.h"
#include "../modules/STSystemFunction.h"
#include "../modules/AnalyticX.h"
#include "../widgets/Dialog.h"
#include "../helpers/AudioHelper.h"
#include "../helpers/AdHelper.h"
#include "RecordScene.h"

#define EXIT_DIALOG_TAG 700

//tag
enum UI_TAG{
    ui01_play = 8,
    ui01_policy = 10,
};


Scene* HomeScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = HomeScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool HomeScene::init()
{
    if (Layer::init()) {
        
        auto root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromBinaryFile("ui01_home.csb"));
        addChild(root);
        
        quickButton("ui01_play", root, CC_CALLBACK_2(HomeScene::onButton, this));
        auto policy = quickButton("ui01_policy", root, CC_CALLBACK_2(HomeScene::onButton, this));
        quickButton("ui01_more", root, CC_CALLBACK_2(HomeScene::onButton, this));

#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
        policy->setVisible(false);
#endif
        
        //Dialog
        auto listener = EventListenerKeyboard::create();
        listener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* event)
        {
            xDialog->showExit(CC_CALLBACK_1(HomeScene::onDialog, this), this);
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

        return true;
    }
    
    return false;
}

void HomeScene::onButton(Ref* sender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    auto widget = (Widget*)sender;
    const string& name = widget->getName();
    
    if (name.compare("ui01_play") == 0) {
        xAudio->playSound("sfx_btn_play.mp3");
    }
    else {
        AudioHelper::getInstance()->playSound(sound_button);
    }
    
    if (name.compare("ui01_play") == 0) {
        xScene->forward(S_PREFACE);
    }
    else if (name.compare("ui01_policy") == 0) {
        ParentGate* lParentGate = ParentGate::create();
        lParentGate->setLinkString("privacy");
        lParentGate->setDelegate(this);
        addChild(lParentGate , 999);
    }
    else if (name.compare("ui01_more") == 0) {
        ParentGate* lParentGate=ParentGate::create();
        lParentGate->setLinkString("more");
        lParentGate->setDelegate(this);
        addChild(lParentGate , 999);
    }
}

void HomeScene::onDialog(const string& name)
{
    xAudio->playDefaultSound();
    
    //yes
    if (name.compare("ui07_btn_yes") == 0) {
        STSystemFunction* sf = new STSystemFunction();
        sf->endSession();
        CC_SAFE_DELETE(sf);
        
        AnalyticX::flurryEndSession();
        
        Director::getInstance()->end();
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    }
}

void HomeScene::parentGateBack(std::string &key_word)
{
    
}

void HomeScene::onExit()
{
    Layer::onExit();
}

void HomeScene::onQuitCallback(const string& name)
{
    if (name.compare("ui07_btn_yes"))
    {
        STSystemFunction* sf = new STSystemFunction();
        sf->endSession();
        CC_SAFE_DELETE(sf);
        
        AnalyticX::flurryEndSession();
        
        Director::getInstance()->end();
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    }
}






