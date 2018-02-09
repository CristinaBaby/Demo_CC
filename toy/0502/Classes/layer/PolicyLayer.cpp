

//
//  PolicyLayer.cpp
//  MyGame_Project
//
//  Created by liyang1 on 12/1/14.
//
//

#include "PolicyLayer.h"
#include "../AppGlobal.h"
#include "../utilities/SceneManager.h"
#include "ParentGate.h"
#include "../helpers/AudioHelper.h"
#include "../utilities/UICpp.h"
#include "../utilities/STUserDefault.h"

USING_NS_CC;

enum {OPEN = 100, play_tag , unlock_tag , term_tag };

//const char* closeSoundStr = "ui02_settings_switch_off.png";
//const char* openSoundStr = "ui02_settings_switch_on.png";
//const char* cur_plist_path = "ui02_settings.plist";
PolicyLayer* PolicyLayer::create(){
    
    PolicyLayer *layer = new PolicyLayer();
    if (layer) {
        layer->init();
    }
    
    return layer;
}

bool PolicyLayer::init()
{
    //实现模态对话框
    Layer::init();
        
    auto _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("Declaim_1.json"));
    addChild(_root);
    
    quickButton("btn_play", _root, CC_CALLBACK_2(PolicyLayer::onButton, this));
    quickButton("btn_terms_of_use", _root, CC_CALLBACK_2(PolicyLayer::onButton, this));
    
    return true;
}


void PolicyLayer::onButton(Ref* sender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    xAudio->playSound(sound_button);
    
    auto widget = (Widget*)sender;
    
    const string& name = widget->getName();
    
    if (name.compare("btn_play") == 0) {
        STSaveBooleanToUD("mypolicy", true);
        xScene->forward(S_HOME);
    }
    else if (name.compare("btn_terms_of_use") == 0)
    {
        ParentGate *gate = ParentGate::create();
        gate->setLinkString("support");
        Director::getInstance()->getRunningScene()->addChild(gate);
    }
}


void PolicyLayer::backCallback(cocos2d::Ref *sender){
    xAudio->playSound(sound_button);
    removeAllChildren();
    getParent()->removeChild(this);
}

PolicyLayer::~PolicyLayer(){
};