//
//  HomeScene.cpp
//  PaintDress
//
//  Created by maxiang on 6/16/15.
//
//

#include "HomeScene.h"
#include "cocostudio/CocoStudio.h"
#include "CocosGUI.h"
#include "AppManager.h"
#include "ChoosePaintScene.h"
#include "DressScene.h"
#include "PhotoScene.h"
#include "../Layer/ShopLayer.h"
#include "../Module/STSystemFunction.h"
#include "../Module/AnalyticX.h"
#include "../Layer/QuitGameLayer.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

#define RES_COCOSTUDIO_JSON          "ui01_home.json"
#define NAME_BUTTON_PLAY             "ui01_btn_play"
#define NAME_BUTTON_DRESS            "ui01_btn_dress"
#define NAME_BUTTON_PICTURE          "ui01_btn_pictures"
#define NAME_BUTTON_SOUND            "ui01_btn_music_on"
#define NAME_BUTTON_SHOP             "ui01_btn_shop"
#define NAME_BUTTON_MORE             "ui01_btn_more"

#define RES_BUTTON_SOUND_CLOSE       "ui01_btn_music_off.png"

#define TAG_QUIT_GAME_LAYER          123

bool HomeScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    bool rcode = false;
    do {
        //init ui
        auto ui = GUIReader::getInstance()->widgetFromJsonFile(RES_COCOSTUDIO_JSON);
        addChild(ui);
        
        //play button
        auto playButton = Helper::seekWidgetByName(ui, NAME_BUTTON_PLAY);
        playButton->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                AppManager::getInstance()->playPlayEffect();
  
                auto scene = ChoosePaintScene::createScene(ResType::ResType_Top);
                ChoosePaintScene *layer = dynamic_cast<ChoosePaintScene*>(scene->getChildren().at(0));
                if (layer) {
                    layer->setNeedShowFullScreenAds(true);
                }
                Director::getInstance()->replaceScene(scene);
            }
        });
        
        //dress button
        auto dressButton = Helper::seekWidgetByName(ui, NAME_BUTTON_DRESS);
        dressButton->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                AppManager::getInstance()->playButtonEffect();
                
                auto scene = DressScene::createScene(ResType::ResType_Hair);
                DressScene *layer = dynamic_cast<DressScene*>(scene->getChildren().at(0));
                if (layer) {
                    layer->setNeedShowFullScreenAds(true);
                }
                Director::getInstance()->replaceScene(scene);
            }
        });
        
        //picture button
        auto pictureButton = Helper::seekWidgetByName(ui, NAME_BUTTON_PICTURE);
        pictureButton->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                AppManager::getInstance()->playButtonEffect();
                
                auto scene = PhotoScene::createScene();
                PhotoScene *layer = dynamic_cast<PhotoScene*>(scene->getChildren().at(0));
                if (layer) {
                    layer->setNeedShowFullScreenAds(true);
                }
                Director::getInstance()->replaceScene(scene);
            }
        });
        
        //sound button
        auto soundButton = dynamic_cast<CheckBox *>(Helper::seekWidgetByName(ui, NAME_BUTTON_SOUND));
        if (!AppManager::getInstance()->isOpenSound())
        {
            soundButton->setSelectedState(true);
        }
        soundButton->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                if (AppManager::getInstance()->isOpenSound())
                {
                    AppManager::getInstance()->setOpenSound(false);
                }
                else
                {
                    AppManager::getInstance()->setOpenSound(true);
                }
                AppManager::getInstance()->playButtonEffect();
            }
        });
        
        //shop button
        auto shopButton = Helper::seekWidgetByName(ui, NAME_BUTTON_SHOP);
        shopButton->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                AppManager::getInstance()->playButtonEffect();
                auto layer = ShopLayer::create();
                Director::getInstance()->getRunningScene()->addChild(layer);
            }
        });
        
        //more button
        auto moreButton = Helper::seekWidgetByName(ui, NAME_BUTTON_MORE);
        moreButton->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                AppManager::getInstance()->playButtonEffect();
                
                auto sysFunction = STSystemFunction();
                sysFunction.showMoreGame();
            }
        });
        
        auto listener = EventListenerKeyboard::create();
        listener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* event)
        {
            if (code == EventKeyboard::KeyCode::KEY_BACK)
            {
                if (getChildByTag(TAG_QUIT_GAME_LAYER) == NULL)
                {
                    auto quitLayer = QuitGameLayer::create();
                    quitLayer->setYesActionCallback([]{
                        
                        AnalyticX::flurryEndSession();
                        Director::getInstance()->end();
                        
                        exit(0);
                    });
                    quitLayer->setNoActionCallback([]{
                        
                        
                    });
                    quitLayer->setTag(TAG_QUIT_GAME_LAYER);
                    this->addChild(quitLayer, 123);
                }
            }
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        //hide banner ads
        AppManager::getInstance()->hideBannerAd();
        
        //play bgm
        AppManager::getInstance()->playStartBgm();

        rcode = true;
    } while (0);
    
    return rcode;
}


