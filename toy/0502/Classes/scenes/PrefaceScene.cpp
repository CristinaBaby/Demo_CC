//
//  PrefaceScene.h
//  Bedtime Story Mermaid
//
//  Created by Luo Xiaopeng.
//
//
#include "PrefaceScene.h"
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


Scene* PrefaceScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = PrefaceScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool PrefaceScene::init()
{
    if (Layer::init()) {
        
        auto root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromBinaryFile("ui08_preface.csb"));
        addChild(root);
        
        quickImageView("preface_bg", root, CC_CALLBACK_2(PrefaceScene::onButton, this));
        
        return true;
    }
    
    return false;
}

void PrefaceScene::onButton(Ref* sender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    AudioHelper::getInstance()->playSound(sound_button);
    
    auto widget = (Widget*)sender;
    const string& name = widget->getName();
    
    if (name.compare("preface_bg") == 0) {
        xScene->forward(S_RECORD);
    }
}







