//
//  JPDifficultyScene.h
//  Bedtime Story Mermaid
//
//  Created by Luo Xiaopeng.
//
//
#include "JPDifficultyScene.h"
#include "../AppGlobal.h"
#include "../utilities/STVisibleRect.h"
#include "../utilities/UICpp.h"
#include "../modules/STSystemFunction.h"
#include "../modules/AnalyticX.h"
#include "../helpers/AudioHelper.h"
#include "../helpers/AdHelper.h"
#include "PuzzleScene.h"
#include "../gameconfig/MyGameConfig.h"

#define EXIT_DIALOG_TAG 700

Scene* JPDifficultyScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = JPDifficultyScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool JPDifficultyScene::init()
{
    if (Layer::init()) {
        
        auto root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromBinaryFile("ui03_puzzle.csb"));
        addChild(root);
        
        quickButton("ui03_very_esay", root, CC_CALLBACK_2(JPDifficultyScene::onButton, this));
        quickButton("ui03_esay", root, CC_CALLBACK_2(JPDifficultyScene::onButton, this));
        quickButton("ui03_medium", root, CC_CALLBACK_2(JPDifficultyScene::onButton, this));
        quickButton("ui03_hard", root, CC_CALLBACK_2(JPDifficultyScene::onButton, this));
        quickButton("ui06_back", root, CC_CALLBACK_2(JPDifficultyScene::onButton, this));
        
        auto box = (ImageView*)Helper::seekWidgetByName(root, "ui03_jigsaw_box");
        auto image = ImageView::create(xGame->getFullFileName());
        image->setScale((box->getContentSize().width / image->getContentSize().width) - 0.02);      //不要显示边
        image->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
        box->addChild(image, -1);
        
        return true;
    }
    
    return false;
}

void JPDifficultyScene::onButton(Ref* sender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    AudioHelper::getInstance()->playSound(sound_button);
    
    auto widget = (Widget*)sender;
    const string& name = widget->getName();
    
    bool forward = true;
    if (name.compare("ui03_very_esay") == 0) {
        xGame->setSideLen(2);
    }
    else if (name.compare("ui03_esay") == 0) {
        xGame->setSideLen(3);
    }
    else if (name.compare("ui03_medium") == 0) {
        xGame->setSideLen(4);
    }
    else if (name.compare("ui03_hard") == 0) {
        xGame->setSideLen(6);
    }
    else if (name.compare("ui06_back") == 0) {
        forward = false;
    }
    
    if (forward) {
        xScene->forward(S_PUZZLE);
    }
    else
    {
        xScene->back();
    }
}

void JPDifficultyScene:: onExit()
{
    Layer::onExit();
    
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}






