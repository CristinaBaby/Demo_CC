//
//  ShopLayer.cpp
//  PaintDress
//
//  Created by maxiang on 6/25/15.
//
//

#include "ShopLayer.h"
#include "cocostudio/CocoStudio.h"
#include "../Model/AppManager.h"
#include "../Scene/ChoosePaintScene.h"
#include "../Module/STSystemFunction.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

#define RES_COCOSTUDIO_JSON         "ui07_shop.json"
#define NAME_BUTTON_CLOSE           "ui08_btn_close"
#define NAME_BUTTON_1               "ui07_btn_1"
#define NAME_BUTTON_2               "ui07_btn_2"
#define NAME_BUTTON_3               "ui07_btn_3"
#define NAME_BUTTON_4               "ui07_btn_1_2"
#define NAME_BUTTON_RESTOR          "ui07_btn_restore"

#define ANIMATION_DURATION 0.27

ShopLayer::~ShopLayer()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool ShopLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0))) {
        return false;
    }
    
    bool rcode = false;
    
    do {
        //iap delegate
        _appBilling.setIABDelegate(this);
        
        //init ui
        _ui = GUIReader::getInstance()->widgetFromJsonFile(RES_COCOSTUDIO_JSON);
        _ui->setPosition(Vec2(0, getContentSize().height));
        addChild(_ui);
        
        _ui->runAction(MoveTo::create(ANIMATION_DURATION, Vec2(0, 20)));
        runAction(FadeTo::create(ANIMATION_DURATION, 210));

        //buttons
        auto closeButton = Helper::seekWidgetByName(_ui, NAME_BUTTON_CLOSE);
        closeButton->setZOrder(100);
        closeButton->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                closeAction();
            }
        });
        
        auto button1 = Helper::seekWidgetByName(_ui, NAME_BUTTON_1);
        button1->setZOrder(100);
        button1->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                purchase1Action();
            }
        });
        
        auto button2 = Helper::seekWidgetByName(_ui, NAME_BUTTON_2);
        button2->setZOrder(100);
        button2->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                purchase2Action();
            }
        });
        
        auto button3 = Helper::seekWidgetByName(_ui, NAME_BUTTON_3);
        button3->setZOrder(100);
        button3->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                purchase3Action();
            }
        });
        
        auto button4 = Helper::seekWidgetByName(_ui, NAME_BUTTON_4);
        button4->setZOrder(100);
        button4->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                purchase4Action();
            }
        });
        
        auto restoreButton = Helper::seekWidgetByName(_ui, NAME_BUTTON_RESTOR);
        restoreButton->setZOrder(100);
        restoreButton->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                restoreAction();
            }
        });
        
        //touch event listen
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [](Touch* touch, Event* event) {
            
            return true;
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        rcode = true;
    } while (0);
    
    return rcode;
}

#pragma mark- Action methods

void ShopLayer::closeAction()
{
    AppManager::getInstance()->playButtonEffect();

    _ui->runAction(MoveTo::create(ANIMATION_DURATION, Vec2(0, getContentSize().height)));
    runAction(Sequence::create(FadeOut::create(ANIMATION_DURATION), RemoveSelf::create(), NULL));
}

void ShopLayer::purchase1Action()
{
    AppManager::getInstance()->playButtonEffect();

    _appBilling.purchaseUnmanaged(IAP_FEW_COINS);
    _ispurchasing = true;
}

void ShopLayer::purchase2Action()
{
    AppManager::getInstance()->playButtonEffect();

    _appBilling.purchaseUnmanaged(IAP_SOME_COINS);
    _ispurchasing = true;
}

void ShopLayer::purchase3Action()
{
    AppManager::getInstance()->playButtonEffect();

    _appBilling.purchaseUnmanaged(IAP_LOTS_COINS);
    _ispurchasing = true;
}

void ShopLayer::purchase4Action()
{
    AppManager::getInstance()->playButtonEffect();

    if (AppManager::getInstance()->isRemoveAds()) {
        auto function = STSystemFunction();
        function.makeToast("You have already purchased.");
        return;
    }
    _appBilling.purchase(IAP_REMOVE_ADS);
    _ispurchasing = true;
}

void ShopLayer::restoreAction()
{
    AppManager::getInstance()->playButtonEffect();

    _appBilling.restore();
    _ispurchasing = true;
}

#pragma mark- STIABDelegate

void ShopLayer::purchaseSuccessful(const char* pid)
{
    _ispurchasing = false;
    if (std::string(pid) == IAP_FEW_COINS)
    {
        AppManager::getInstance()->addCoin(4999);
    }
    else if(std::string(pid) == IAP_SOME_COINS)
    {
        AppManager::getInstance()->addCoin(19999);
    }
    else if(std::string(pid) == IAP_LOTS_COINS)
    {
        AppManager::getInstance()->addCoin(29999);
    }
    else if(std::string(pid) == IAP_REMOVE_ADS)
    {
        AppManager::getInstance()->removeAds();
    }
    
    ChoosePaintScene *layer = dynamic_cast<ChoosePaintScene*>(Director::getInstance()->getRunningScene()->getChildren().at(0));
    if (layer) {
        layer->refreshCoinLabel();
    }
}

void ShopLayer::purchaseFailed(const char *pid, int errorCode)
{
    _ispurchasing = false;
}

void ShopLayer::restoreSuccessful(const char* pid)
{
    _ispurchasing = false;
    if(std::string(pid) == IAP_REMOVE_ADS)
    {
        AppManager::getInstance()->removeAds();
    }
}

void ShopLayer::restoreFailed(const char* pid, int errorCode)
{
    _ispurchasing = false;
    
}
