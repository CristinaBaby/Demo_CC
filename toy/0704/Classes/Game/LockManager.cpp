//
//  LockManager.cpp
//  ColorBook
//
//  Created by luoxp on 4/24/15.
//
//

#include "LockManager.h"
#include "UICpp.h"
#include "AppConfigs.h"
#include "IAPManager.h"
#include "CBPackScene.h"
#include "GameScene.h"
#include "TimerManager.hpp"
#include "RewardAdLoadingLayer.h"
#include "STSystemFunction.h"
#include "PackManager.h"


static LockManager *_sharedLock = nullptr;

LockManager* LockManager::getInstance()
{
    if (! _sharedLock)
    {
        _sharedLock = new LockManager();
    }
    
    return _sharedLock;
}

void LockManager::destroyInstance()
{
    CC_SAFE_DELETE(_sharedLock);
}

LockManager::LockManager()
{
    auto event_iap = EventListenerCustom::create(EVENT_IAP, CC_CALLBACK_1(LockManager::purchaseSuccessful, this));
    _eventDispatcher->addEventListenerWithFixedPriority(event_iap, 1);
    
    auto event_reward = EventListenerCustom::create(EVENT_REWARD, CC_CALLBACK_1(LockManager::rewardAdSuccessful, this));
    _eventDispatcher->addEventListenerWithFixedPriority(event_reward, 1);
}

LockManager::~LockManager()
{
    _eventDispatcher->removeEventListenersForTarget(this);
}

void LockManager::update(float dt)
{
}

void LockManager::purchase(const string& identifier)
{
    xIAP->purchase(identifier);
}

void LockManager::restore()
{
    xIAP->restore();
}

void LockManager::purchaseSuccessful(EventCustom* event)
{
    string identifier = ((Value*)(event->getUserData()))->asString();
    
    int index = getIdentifierIndex(identifier);
    auto scene = Director::getInstance()->getRunningScene();
    if (isPack(identifier))
    {
        if (scene->getName() == "PackScene") {
            ((PackScene*)scene)->updatePacks(index);
        }
    }
    else
    {
        if (scene->getName() == "GameScene")
            ((GameScene*)scene)->updatePensPack(index);
    }
}

void LockManager::rewardAd(int index)
{
    _index = index;
    
    auto rewardAd = RewardAdLoadingLayer::create();
    Director::getInstance()->getRunningScene()->addChild(rewardAd, 100);
}

void LockManager::rewardAdSuccessful(EventCustom* event)
{
    auto scene = Director::getInstance()->getRunningScene();
    if (scene->getName() == "PackScene") {
        xTimer->setPackTime(PACK_TYPE::PICTURE_PACK, xTimer->getCurrTime(), _index);
        ((PackScene*)scene)->updatePacks(_index);
    }
    else if (scene->getName() == "GameScene")
    {
        xTimer->setPackTime(PACK_TYPE::PENS_PACK, xTimer->getCurrTime(), _index);
        ((GameScene*)scene)->updatePensPack(_index);
    }
}

bool LockManager::isLocked(int index)
{
    return isLocked(xIAP->getIdentifierForPen(index));
}

bool LockManager::isLocked(const string& identifier)
{
    if (xIAP->isPurchased(identifier)) {
        return false;
    }
    else
    {
        int index = getIdentifierIndex(identifier);
        if (isPack(identifier)) {
            if(xTimer->isLocked(PACK_TYPE::PICTURE_PACK, index))
                return true;
            else
                return false;
        }
        else
        {
            if(xTimer->isLocked(PACK_TYPE::PENS_PACK, index))
                return true;
            else
                return false;
        }
    }
    
    return true;
}

bool LockManager::isPack(const string& identifier)
{
    if (xPack->isPackPurchasedIdentifier(identifier))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int LockManager::getIdentifierIndex(const string& identifier)
{
    return std::stoi(identifier.substr(identifier.length()- 1, 1)) - 1;
}

void LockManager::saveNetworkState()
{
    STSystemFunction sf;
    _networkState = sf.checkNetworkAvailable();
}