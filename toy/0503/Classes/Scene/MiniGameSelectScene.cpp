//
//  MiniGameSelectScene.cpp
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#include "MiniGameSelectScene.h"
#include "HomeScene.h"
#include "ChoosePageScene.h"
#include "JigsawPuzzleScene.h"
#include "ColorBookScene.h"
#include "AppManager.h"

USING_NS_CC;
using namespace extension;
using namespace ui;

bool MiniGameSelectScene::needShowCross = false;

MiniGameSelectScene::MiniGameSelectScene()
{};

MiniGameSelectScene::~MiniGameSelectScene()
{}

bool MiniGameSelectScene::init()
{
    if (!BaseScene::init()) {
        return false;
    }
    
    //init ui
    _rootLayout = GUIReader::getInstance()->widgetFromJsonFile("minigame_select.json");
    addChild(_rootLayout, 1);
    
    //back
    Utils::addEventToButton(_rootLayout, "back_button", Widget::TouchEventType::ENDED, [&](){
        
        AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

        auto scene = BaseScene::createScene<HomeScene>();
        auto fadeTransition = TransitionFade::create(0.3, scene);
        
        Director::getInstance()->replaceScene(fadeTransition);
    });
    
    //colorbook
    Utils::addEventToButton(_rootLayout, "colorbook_button", Widget::TouchEventType::ENDED, [&](){
        
        colorBookAction();
    });
    
    //spot
    Utils::addEventToButton(_rootLayout, "spot_button", Widget::TouchEventType::ENDED, [&](){
        
        spotAction();
    });
    
    //jigsaw
    Utils::addEventToButton(_rootLayout, "jigsaw_button", Widget::TouchEventType::ENDED, [&](){
        
        jigsawAction();
    });
    
    runAction(CallFunc::create([=](){
        
        if (needShowCross)
        {
            AppManager::getInstance()->requestCrossPromoAd();
        }
        
        needShowCross = false;
    }));
    
    //bgm
    AppManager::getInstance()->playBgm(SOUND_BGM_START);
    
    return true;
}

void MiniGameSelectScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    AppManager::getInstance()->setBannerAdVisible(true);
}

void MiniGameSelectScene::onExit()
{
    AppManager::getInstance()->setBannerAdVisible(false);
    Layer::onExit();
}

#pragma mark- Game Enter

void MiniGameSelectScene::colorBookAction()
{
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

    auto scene = BaseScene::createScene<ChoosePageScene>();
    auto fadeTransition = TransitionFade::create(0.3, scene);
    auto layer = dynamic_cast<ChoosePageScene*>(scene->getChildren().at(0));

    layer->setupGameType(GameType::ColorBook);
    Director::getInstance()->replaceScene(fadeTransition);
}

void MiniGameSelectScene::spotAction()
{
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

    auto scene = BaseScene::createScene<ChoosePageScene>();
    auto fadeTransition = TransitionFade::create(0.3, scene);
    auto layer = dynamic_cast<ChoosePageScene*>(scene->getChildren().at(0));
    
    layer->setupGameType(GameType::Spot);
    Director::getInstance()->replaceScene(fadeTransition);
}

void MiniGameSelectScene::jigsawAction()
{
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

    auto scene = BaseScene::createScene<ChoosePageScene>();
    auto fadeTransition = TransitionFade::create(0.3, scene);
    auto layer = dynamic_cast<ChoosePageScene*>(scene->getChildren().at(0));
    
    layer->setupGameType(GameType::Jigsaw);
    Director::getInstance()->replaceScene(fadeTransition);
}
