//
//  HomeScene.cpp
//  BedtimeStory
//
//  Created by maxiang on 8/28/15.
//
//

#include "HomeScene.h"
#include "MiniGameSelectScene.h"
#include "AppManager.h"
#include "PlayAudioScene.h"
#include "RecordAudioScene.h"
#include "ChoosePageScene.h"
#include "STSystemFunction.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

bool HomeScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    //init ui
    _rootLayout = GUIReader::getInstance()->widgetFromJsonFile("home(4).json");
    addChild(_rootLayout, 1);

    //play audio book button
    Utils::addEventToButton(_rootLayout, "story_button", Widget::TouchEventType::ENDED, [&](){
        
        storyAction();
    });

    //parent gate
    Utils::addEventToButton(_rootLayout, "parents_button", Widget::TouchEventType::ENDED, [&](){
        
        parentsAction();
    });
    
    //mini game button
    Utils::addEventToButton(_rootLayout, "mimigame_button", Widget::TouchEventType::ENDED, [&](){
        
        miniGameAction();
    });

    //shop button
    Utils::addEventToButton(_rootLayout, "shop_button", Widget::TouchEventType::ENDED, [&](){
        
        shopAction();
    });
    
    //more game
    Utils::addEventToButton(_rootLayout, "more_button", Widget::TouchEventType::ENDED, [&](){
        
        moreGameAction();
    });
    
    //bgm
    AppManager::getInstance()->playBgm(SOUND_BGM_STORY);
    
    //iap delegate
    _iap.setIABDelegate(this);
 
    return true;
}

#pragma mark- Action methods

void HomeScene::storyAction()
{
    AppManager::getInstance()->playEffect(SOUND_BTN_PLAY);
    
    auto scene = BaseScene::createScene<ChoosePageScene>();
    ChoosePageScene::needShowCross = true;
    auto layer = dynamic_cast<ChoosePageScene*>(scene->getChildren().at(0));
    layer->setupGameType(GameType::Story);
    
    auto fadeTransition = TransitionFade::create(0.3, scene);
    Director::getInstance()->replaceScene(fadeTransition);
}

void HomeScene::parentsAction()
{
    AppManager::getInstance()->playEffect(SOUND_POPUB);

    showParentGate();
}

void HomeScene::miniGameAction()
{
    AppManager::getInstance()->playEffect(SOUND_BTN_PLAY);

    auto scene = BaseScene::createScene<MiniGameSelectScene>();
    MiniGameSelectScene::needShowCross = true;
    auto fadeTransition = TransitionFade::create(0.3, scene);
    Director::getInstance()->replaceScene(fadeTransition);
}

void HomeScene::shopAction()
{
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

    _iap.restore();
}

void HomeScene::moreGameAction()
{
    AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

    auto system = STSystemFunction();
    system.showMoreGame();
}

void HomeScene::showParentGate()
{
    auto layer = LayerColor::create(Color4B(0.0, 0.0, 0.0, 200.0));
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    eventListener->onTouchBegan = [](Touch *touch, Event *unusedEvent){
        
        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, layer);
    addChild(layer, 110);
    
    auto ui = GUIReader::getInstance()->widgetFromJsonFile("parentgate_dialog(1).json");
    ui->setAnchorPoint(Point::ANCHOR_MIDDLE);
    ui->setPosition(getContentSize()/2);
    layer->addChild(ui, 1);
    
    ui->setScale(0.1);
    ui->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.0), 0.7));
    
    int number1 = arc4random() % 10 + 1;
    int number2 = arc4random() % 10 + 1;
    
    auto answerFiled = (TextField *)Helper::seekWidgetByName(ui, "answer_textfiled");
    auto numberLabel = (Text *)Helper::seekWidgetByName(ui, "number_label");
    
    std::string numberString = std::to_string(number1) + "+" + std::to_string(number2) + "=";
    numberLabel->setString(numberString);
    
    Utils::addEventToButton(ui, "close_button", Widget::TouchEventType::ENDED, [=](){
        
        AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

        layer->removeFromParent();
    });
    
    Utils::addEventToButton(ui, "send_button", Widget::TouchEventType::ENDED, [=](){
        
        AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

        std::string answerString = answerFiled->getStringValue();
        
        if (!AppManager::getInstance()->isDigit(answerString))
            return;
        
        int answer = std::stoi(answerString);
        if (answer == number1 + number2)
        {
            auto scene = BaseScene::createScene<ChoosePageScene>();
            ChoosePageScene::needShowCross = true;
            auto layer = dynamic_cast<ChoosePageScene*>(scene->getChildren().at(0));
            layer->setupGameType(GameType::Record);
            
            auto fadeTransition = TransitionFade::create(0.3, scene);
            Director::getInstance()->replaceScene(fadeTransition);
        }
    });
}


#pragma mark- STIABDelegate

void HomeScene::purchaseSuccessful(const char* pid)
{
    if (std::string(pid) == IAP_UNLOCK_All)
    {
        std::string userDefault = UserDefaultKey_HasPurchasedItem_ + std::string(pid);
        UserDefault::getInstance()->setBoolForKey(userDefault.c_str(), true);
        UserDefault::getInstance()->flush();
        
        AppManager::getInstance()->setHasUnlockAll(true);
    }
}

void HomeScene::purchaseFailed(const char *pid, int errorCode)
{
    
}

void HomeScene::restoreSuccessful(const char* pid)
{
    if (std::string(pid) == IAP_UNLOCK_All)
    {
        std::string userDefault = UserDefaultKey_HasPurchasedItem_ + std::string(pid);
        UserDefault::getInstance()->setBoolForKey(userDefault.c_str(), true);
        UserDefault::getInstance()->flush();
        
        AppManager::getInstance()->setHasUnlockAll(true);
    }
}

void HomeScene::restoreFailed(const char* pid, int errorCode)
{
    
}

