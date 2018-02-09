//
// ShopLayer.cpp
// ANIM3203
//
// Created by dengcheng on 15/10/8.
//
//

#include "ShopLayer.h"
#include "ShopAdapter.h"
#include "IAPManager.h"

ShopLayer::ShopLayer()
{
    
}

void ShopLayer::onEnter()
{
    LayerColor::onEnter();
    
}

ShopLayer* ShopLayer::create()
{
    auto pRet = new ShopLayer();
    if(pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    pRet = nullptr;
    return nullptr;
}

bool ShopLayer::init()
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 255 * 0.8)) )
    {
        return false;
    }
    
    EventListenerTouchOneByOne *pTouch = EventListenerTouchOneByOne::create();
    pTouch->onTouchBegan = [](Touch *, Event *)->bool{
        return true;
    };
    pTouch->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(pTouch, this);
    
    auto *pUI = GUIReader::getInstance()->widgetFromJsonFile("cocoStudio/shop.json");
    addChild(pUI);
    
    auto pClose = Helper::seekWidgetByName(pUI, "public_btn_close");
    pClose->addTouchEventListener([this](Ref *, Widget::TouchEventType aType){
        if (aType != Widget::TouchEventType::ENDED) {
            return;
        }
        this->removeFromParent();
    });
    
    auto pAllBuy = Helper::seekWidgetByName(pUI, "shop_btn_all_buy");
    pAllBuy->addTouchEventListener([this](Ref *, Widget::TouchEventType aType){
        if (aType != Widget::TouchEventType::ENDED) {
            return;
        }
        ShopAdapter::getInstance()->requestBuyItem(IAPManager::getInstance()->getItemKey(kIAPUnlockAll));
    });
    
    auto pPetsBuy = Helper::seekWidgetByName(pUI, "shop_btn_pets_buy");
    pPetsBuy->addTouchEventListener([this](Ref *, Widget::TouchEventType aType){
        if (aType != Widget::TouchEventType::ENDED) {
            return;
        }
        ShopAdapter::getInstance()->requestBuyItem(IAPManager::getInstance()->getItemKey(kIAPUnlockPets));
    });
    
    auto pDressUpBuy = Helper::seekWidgetByName(pUI, "shop_btn_dressup_buy");
    pDressUpBuy->addTouchEventListener([this](Ref *, Widget::TouchEventType aType){
        if (aType != Widget::TouchEventType::ENDED) {
            return;
        }
        ShopAdapter::getInstance()->requestBuyItem(IAPManager::getInstance()->getItemKey(kIAPUnlockDressUp));
    });
    
    auto pRestore = Helper::seekWidgetByName(pUI, "shop_btn_restore");
    pRestore->addTouchEventListener([this](Ref *, Widget::TouchEventType aType){
        if (aType != Widget::TouchEventType::ENDED) {
            return;
        }
        ShopAdapter::getInstance()->requestRestore();
    });
    
    if (IAPManager::getInstance()->getItemIsBought(kIAPUnlockAll)) {
        pAllBuy->setTouchEnabled(false);
        pAllBuy->setColor(Color3B::GRAY);
        
        pPetsBuy->setTouchEnabled(false);
        pPetsBuy->setColor(Color3B::GRAY);
        
        pDressUpBuy->setTouchEnabled(false);
        pDressUpBuy->setColor(Color3B::GRAY);
    } else {
        if (IAPManager::getInstance()->getItemIsBought(kIAPUnlockPets)) {
            pPetsBuy->setTouchEnabled(false);
            pPetsBuy->setColor(Color3B::GRAY);
        }
        
        if (IAPManager::getInstance()->getItemIsBought(kIAPUnlockDressUp)) {
            pDressUpBuy->setTouchEnabled(false);
            pDressUpBuy->setColor(Color3B::GRAY);
        }
    }
    
    m_pNOtifaction = _eventDispatcher->addCustomEventListener("ShopBuySuccess", [pAllBuy, pPetsBuy, pDressUpBuy](EventCustom *){
        if (IAPManager::getInstance()->getItemIsBought(kIAPUnlockAll)) {
            pAllBuy->setTouchEnabled(false);
            pAllBuy->setColor(Color3B::GRAY);
            
            pPetsBuy->setTouchEnabled(false);
            pPetsBuy->setColor(Color3B::GRAY);
            
            pDressUpBuy->setTouchEnabled(false);
            pDressUpBuy->setColor(Color3B::GRAY);
        } else {
            if (IAPManager::getInstance()->getItemIsBought(kIAPUnlockPets)) {
                pPetsBuy->setTouchEnabled(false);
                pPetsBuy->setColor(Color3B::GRAY);
            }
            
            if (IAPManager::getInstance()->getItemIsBought(kIAPUnlockDressUp)) {
                pDressUpBuy->setTouchEnabled(false);
                pDressUpBuy->setColor(Color3B::GRAY);
            }
        }
    });
    
    auto pBox = Helper::seekWidgetByName(pUI, "shop_box");
    pBox->setScale(0.1);
    
    pBox->runAction(EaseBackOut::create(ScaleTo::create(0.3, 1)));
    
    return true;
}

void ShopLayer::onExit()
{
    
    LayerColor::onExit();
}
ShopLayer::~ShopLayer()
{
    _eventDispatcher->removeEventListener(m_pNOtifaction);
}