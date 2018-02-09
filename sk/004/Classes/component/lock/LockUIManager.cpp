//
//  LockUIManager.cpp
//  BHUG1062
//
//  Created by tanshoumei on 5/12/16.
//
//

#include "LockUIManager.h"
#include "SSCIAPManager.h"
#include "SSCInternalLibManager.h"
#include "scene/loading/LoadingTransitionScene.h"
#include "scene/shop/ShopScene.h"
#include "component/makeComponent/basicComponent/ComponentCycleScroll.h"
#include "DialogLayer.h"
static LockUIManager* _s_LockUIManager = nullptr;

LockUIManager* LockUIManager::getInstance()
{
    if (!_s_LockUIManager) {
        _s_LockUIManager = new LockUIManager();
    }
    return _s_LockUIManager;
}

LockUIManager::LockUIManager()
{
    Director::getInstance()->getEventDispatcher()->addCustomEventListener(kRewardItemStatusChangedNotify, [=](EventCustom*)
    {
        Director::getInstance()->getRunningScene()->runAction(CallFunc::create([=]
        {
            updateLockStatus();
        }));
    });
    
    auto lockStatusScheduleCall = [=](float dt)
    {
        this->updateLockStatus();
    };
    Director::getInstance()->getScheduler()->schedule(lockStatusScheduleCall, this, 1, kRepeatForever, 0, false, "lockStatusScheduleCall");
    RewardManager::getInstance()->showRewardFalseCall = [=]
    {
        Director::getInstance()->getRunningScene()->runAction(CallFunc::create([=]
                                                                               {
                                                                                   showNoRewardAdDialog();
                                                                               }));
    };
    RewardManager::getInstance()->showRewardLoadingCall = [=]
    {
        Director::getInstance()->getRunningScene()->runAction(CallFunc::create([=]
                                                                               {
                                                                                   showRewardLoading();
                                                                               }));
        
    };
    RewardManager::getInstance()->removeRewardLoadingCall = [=]
    {
        Director::getInstance()->getRunningScene()->runAction(CallFunc::create([=]
                                                                               {
                                                                                   removeRewardLoading();
                                                                               }));
    };
}

bool LockUIManager::registerLock(string moduleName, string keyInModule, const int inx, cocos2d::Node *lockParent,Vec2 pos)
{
    auto rewardInfo = RewardManager::getInstance()->getRewardInfoItem(moduleName, keyInModule, inx);
    _rewardPos = pos;
    if(rewardInfo.isNull())
        return false;
    _mapRewardInfos[rewardInfo.getKey()] = rewardInfo;
    _mapLockParents[rewardInfo.getKey()] = lockParent;
    lockParent->setOnExitCallback([=](){
        _mapLockParents.erase(rewardInfo.getKey());
    });
    lockParent->setOnEnterCallback([=](){
        _mapLockParents[rewardInfo.getKey()] = lockParent;
    });
    return true;
}

bool LockUIManager::registerIAPLock(string iapId, string iapModuelName, const int inx, cocos2d::Node *lockParent)
{
    //if not iap
    bool hasPurchased = SSCIAPManager::getInstance()->isPackagePurchased(iapId);
    if(hasPurchased)
        return false;
    if(!SSCIAPManager::getInstance()->isFree(iapModuelName, inx))
    {
        addIAPLock(lockParent);
        _mapIAPLockParentNodes.insert(make_pair(lockParent, iapId));
        return true;
    }
    return false;
}

void LockUIManager::updateLockStatus()
{

    for(auto itor: _mapRewardInfos)
    {
        auto parent = _mapLockParents[itor.first];
        if(!parent)
            continue;
        if(SSCIAPManager::getInstance()->isPackagePurchased(itor.second.iapId))
        {
            if(parent->getChildByTag(TAG_REWARD_LOCK))
                parent->removeChildByTag(TAG_REWARD_LOCK);
            continue;
        }
        if(RewardManager::getInstance()->isLocked(itor.second))
        {
            addLock(parent);
            continue;
        }
        if(parent->getChildByTag(TAG_REWARD_LOCK))
            parent->removeChildByTag(TAG_REWARD_LOCK);
    }
}

void LockUIManager::updateIAPLockStatus()
{
    for(map<Node*, string>::iterator itor = _mapIAPLockParentNodes.begin(); itor != _mapIAPLockParentNodes.end(); )
    {
        auto parent = itor->first;
        if(SSCIAPManager::getInstance()->isPackagePurchased(itor->second))
        {
            if(parent->getChildByTag(TAG_IAP_LOCK))
                parent->removeChildByTag(TAG_IAP_LOCK);
            _mapIAPLockParentNodes.erase(itor++);
            continue;
        }
        ++itor;
    }
}

void LockUIManager::addLock(cocos2d::Node *parent)
{
    if(!SSCIAPManager::getInstance()->isShowAds())
        return;
    if(parent->getChildByTag(TAG_REWARD_LOCK))
        return;
    string lockPath = REWARD_LOCK_FILE;
    
    bool netWorkAvailable = SSCInternalLibManager::getInstance()->checkNetworkAvailable();
    
    netWorkAvailable = true;
    //如果网络不可用，将reward标记为iap
    if (!netWorkAvailable)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        lockPath = LOCK_FILE;
#endif
    }
    Sprite* lock = Sprite::create(lockPath);
    lock->setScale(1 + (1 - parent->getScale()));
    parent->addChild(lock, INT_MAX, TAG_REWARD_LOCK);
    if (_rewardPos != Vec2(1.f, 1.f))
    {
        lock->setPosition(_rewardPos);
    }
    else
    {
        lock->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        lock->setPosition(Vec2(parent->getContentSize().width, 0));
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (!netWorkAvailable)
        lock->setName(LOCK_NAME_IAP);
    else
        lock->setName(LOCK_NAME_REWARD);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    lock->setName(LOCK_NAME_REWARD);
#endif
}

void LockUIManager::addIAPLock(cocos2d::Node *parent)
{
    if(parent->getChildByTag(TAG_IAP_LOCK))
        return;
    Sprite* lock = Sprite::create(LOCK_FILE);
    lock->setScale(1 + (1 - parent->getScale()));
    if(lock)
    {
        parent->addChild(lock, INT_MAX, TAG_IAP_LOCK);
        lock->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        lock->setPosition(Vec2(parent->getContentSize().width, 0));
        parent->setOnExitCallback([=](){
            _mapIAPLockParentNodes.erase(parent);
        });
    }
}

void LockUIManager::showNoRewardAdDialog()
{
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CSIgnoreAllTouchNotification);

//    Node* dialogNode = Node::create();
//    Sprite* bg = Sprite::create("ui/popup/tips_bg.png");
//    dialogNode->addChild(bg);
//    auto shopButton = Button::create("ui/popup/buy_btn.png");
//    shopButton->setPosition(Vec2(330, 44));
//    bg->addChild(shopButton);
//    shopButton->addTouchEventListener([=](Ref* r, Widget::TouchEventType t){
//        if(t != Widget::TouchEventType::ENDED)
//            return;
//        
//        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CSCancelIgnoreAllTouchNotification);
//
//        ShopScene* layer = ShopScene::create();
//        layer->onCloseCallback = [=](){
//            LockUIManager::getInstance()->updateIAPLockStatus();
//        };
//        Director::getInstance()->getRunningScene()->addChild(layer, INT_MAX);
//        dialogNode->removeFromParent();
//    });
//    shopButton->setVisible(CC_TARGET_PLATFORM == CC_PLATFORM_IOS);
//    
//    auto cancelButton = Button::create("ui/popup/cancel_btn.png");
//    bg->addChild(cancelButton);
//    cancelButton->setPosition(Vec2(158, 44));
//    cancelButton->addTouchEventListener([=](Ref* r, Widget::TouchEventType t){
//        if(t != Widget::TouchEventType::ENDED)
//            return;
//        
//        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CSCancelIgnoreAllTouchNotification);
//
//        dialogNode->removeFromParent();
//    });
//    
//    auto messageLabel = Label::createWithTTF("", "ui/fonts/AxureHandwriting.otf", 20);
//    messageLabel->setAlignment(TextHAlignment::CENTER);
//    messageLabel->setMaxLineWidth(310);
//    messageLabel->setLineBreakWithoutSpace(true);
//    messageLabel->setTextColor(Color4B(250, 252, 246, 255));
//    messageLabel->setPosition(Vec2(bg->getContentSize().width/2, 230.0));
//    bg->addChild(messageLabel);
//    TTFConfig ttfConfig("ui/fonts/AxureHandwriting.otf", 25, GlyphCollection::DYNAMIC);
//    messageLabel->setTTFConfig(ttfConfig);
//    string str = "Sorry! No video clips available at present! Please check network or buy it.";
//    if(CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
//    {
//        cancelButton->setPosition(Vec2(250, 44));
//        cancelButton->loadTextureNormal("ui/popup/ok_btn.png");
//        str = "Sorry! No video clips available at present! Please check network.";
//    }
//    messageLabel->setString(str);
//    
//    Director::getInstance()->getRunningScene()->addChild(dialogNode, INT_MAX);
//    dialogNode->setPosition(Director::getInstance()->getVisibleOrigin() + Director::getInstance()->getVisibleSize() * 0.5);
    
    auto dialogNode = DialogLayer::showWithMessage("Sorry! No video clips available at present! Please check network or buy it.", [=]
    {
                Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CSCancelIgnoreAllTouchNotification);
    }, nullptr,Director::getInstance()->getRunningScene(),40,true,true);
    dialogNode->setZOrder(INT_MAX);
    
    auto touchLs = EventListenerTouchOneByOne::create();
    touchLs->setSwallowTouches(true);
    touchLs->onTouchBegan = [](Touch* t, Event* e)
    {
        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchLs, dialogNode);
}

void LockUIManager::showRewardLoading()
{
    if(Director::getInstance()->getRunningScene()->getChildByTag(TAG_REWARD_LOADING))
        return;
    auto loading = LoadingTransitionScene::create();
    loading->setTag(TAG_REWARD_LOADING);
    Director::getInstance()->getRunningScene()->addChild(loading,99);
}

void LockUIManager::removeRewardLoading()
{
    if(Director::getInstance()->getRunningScene()->getChildByTag(TAG_REWARD_LOADING))
    {
        log("TAG_REWARD_LOADING is true");
        Director::getInstance()->getRunningScene()->removeChildByTag(TAG_REWARD_LOADING);
    }
}

void LockUIManager::setRewardLockPos(Vec2 pos)
{
    _rewardPos = pos;
}

void LockUIManager::setIAPLockPos(Vec2 pos)
{
    for(map<Node*, string>::iterator itor = _mapIAPLockParentNodes.begin(); itor != _mapIAPLockParentNodes.end(); )
    {
        auto parent = itor->first;
        Node* lock = nullptr;
        if (parent->getChildByTag(TAG_IAP_LOCK))
            lock = parent->getChildByTag(TAG_IAP_LOCK);
        if(lock != nullptr)
            lock->setPosition(pos);
        ++itor;
    }
}


