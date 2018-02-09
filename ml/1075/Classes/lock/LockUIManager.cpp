//
//  LockUIManager.cpp
//  BHUG1062
//
//  Created by tanshoumei on 5/12/16.
//
//

#include "LockUIManager.h"
//#include "IAPManager.h"
#include "STSystemFunction.h"
#include "ConfigManager.h"
#include "ShopLayer.h"
//#include "AdsLoadingScene.h"
#include "Dialog.h"

//#include "scene/loading/LoadingTransitionScene.h"
//#include "scene/shop/ShopScene.h"
//#include "component/makeComponent/basicComponent/ComponentCycleScroll.h"

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
    Director::getInstance()->getEventDispatcher()->addCustomEventListener(kRewardItemStatusChangedNotify, CC_CALLBACK_0(LockUIManager::updateLockStatus, this));
    Director::getInstance()->getEventDispatcher()->addCustomEventListener(kRewardAdsClosedNotify, CC_CALLBACK_0(LockUIManager::rewardsSuccessShowClosed, this));
    
    onRewardSuccess = nullptr;
    auto lockStatusScheduleCall = [=](float dt)
    {
        this->updateLockStatus();
    };
    Director::getInstance()->getScheduler()->schedule(lockStatusScheduleCall, this, 1, kRepeatForever, 0, false, "lockStatusScheduleCall");
    
    RewardManager::getInstance()->showRewardFalseCall = CC_CALLBACK_0(LockUIManager::showNoRewardAdDialog, this);
    RewardManager::getInstance()->showRewardLoadingCall = CC_CALLBACK_0(LockUIManager::showRewardLoading, this);
    RewardManager::getInstance()->removeRewardLoadingCall = CC_CALLBACK_0(::LockUIManager::removeRewardLoading, this);
}

bool LockUIManager::registerLock(string typeName, const int inx, Node* lockParent)
{
    auto rewardInfo = RewardInfoItem(typeName,inx);
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

//bool LockUIManager::registerIAPLock(string iapId, string iapModuelName, const int inx, cocos2d::Node *lockParent)
//{
//    //if not iap
//    bool hasPurchased = IAPManager::getInstance()->isPackagePurchased(iapId);
//    if(hasPurchased)
//        return false;
//    if(!IAPManager::getInstance()->isFree(iapModuelName, inx))
//    {
//        addIAPLock(lockParent);
//        _mapIAPLockParentNodes.insert(make_pair(lockParent, iapId));
//        return true;
//    }
//    return false;
//}

void LockUIManager::rewardsSuccessShowClosed()
{
    if (onRewardSuccess) {
//        onRewardSuccess();
        Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(0.5),
                                                                               CallFunc::create([=](){
            onRewardSuccess();
        }), NULL));
    }
}

void LockUIManager::updateLockStatus()
{
    for(auto itor: _mapRewardInfos)
    {
        auto parent = _mapLockParents[itor.first];
        if(!parent)
            continue;
        
        if(ConfigManager::getInstance()->getUnLocked(itor.second.keyInModule))
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
        if(IAPManager::getInstance()->getItemIsBought(itor->second))
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
    //    if(!IAPManager::getInstance()->isShowAds())
    //        return;
    if(parent->getChildByTag(TAG_REWARD_LOCK))
        return;
    STSystemFunction sys;
    string lockPath = REWARD_LOCK_FILE;
    
    bool netWorkAvailable = sys.checkNetworkAvailable();
    //如果网络不可用，将reward标记为iap
//    if (!netWorkAvailable)
//    {
//        lockPath = LOCK_FILE;
//    }
    Sprite* lock = Sprite::create(lockPath);
    lock->setScale(1 + (1 - parent->getScale()));
    parent->addChild(lock, INT_MAX, TAG_REWARD_LOCK);
//    lock->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    //    lock->setPosition(Vec2(parent->getContentSize().width, 0));
    lock->setPosition(_lockPos);
//    if (!netWorkAvailable)
//        lock->setName(LOCK_NAME_IAP);
//    else
        lock->setName(LOCK_NAME_REWARD);
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
//        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CSIgnoreAllTouchNotification);
//    AudioHelp::getInstance()->pauseBackGroundMusic();
    Dialog* dialog=Dialog::create(Size(559,436), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_SINGLE);
    
    dialog->setContentText("There is a problem with Internet connection. Please try again later.");
    Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
    dialog->setPosition(Director::getInstance()->getVisibleOrigin());
    dialog->onPositiveClick = [=](void*){
        
    };
    return;
//    Dialog* dialog=Dialog::create(Size(504,360), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
//    dialog->setContentText("Rewarded Ads is currently unavailable");
//    Director::getInstance()->getRunningScene()->addChild(dialog, INT_MAX);
//    dialog->setPosition(Director::getInstance()->getVisibleOrigin());
//    dialog->setNegativeBtnText("dialog/btn_cancel.png");
//    dialog->setPositiveBtnText("dialog/btn_buy.png");
//    dialog->onPositiveClick = [=](void*){
//        ShopLayer* pLayer = ShopLayer::create();
//        Director::getInstance()->getRunningScene()->addChild(pLayer, INT_MAX);
//        pLayer->shopDismissed = [=](){
//            LockUIManager::getInstance()->updateIAPLockStatus();
//        };
//    };
//    dialog->onNegativeClick = [=](void*){
//    };

    
        Node* dialogNode = Node::create();
    LayerColor* pLayer = LayerColor::create(Color4B(0,0,0,160));
    dialogNode->addChild(pLayer);
    pLayer->setPosition(Vec2(-visibleSize.width/2, -visibleSize.height/2));
    
        Sprite* bg = Sprite::create("dialog/popup_bg.png");
        dialogNode->addChild(bg);
        auto shopButton = Button::create("dialog/btn_buy.png");
        shopButton->setPosition(Vec2(310, 80));
        bg->addChild(shopButton);
        shopButton->addTouchEventListener([=](Ref* r, Widget::TouchEventType t){
            if(t != Widget::TouchEventType::ENDED)
                return;
    
    //        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CSCancelIgnoreAllTouchNotification);
    
            ShopLayer* pLayer = ShopLayer::create();
            Director::getInstance()->getRunningScene()->addChild(pLayer, INT_MAX);
            pLayer->shopDismissed = [=](){
                LockUIManager::getInstance()->updateIAPLockStatus();
            };
            dialogNode->removeFromParent();
        });
        shopButton->setVisible(CC_TARGET_PLATFORM == CC_PLATFORM_IOS);
    
        auto cancelButton = Button::create("dialog/btn_cancel.png");
        bg->addChild(cancelButton);
        cancelButton->setPosition(Vec2(100, 80));
        cancelButton->addTouchEventListener([=](Ref* r, Widget::TouchEventType t){
            if(t != Widget::TouchEventType::ENDED)
                return;
    
    //        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CSCancelIgnoreAllTouchNotification);
    
            dialogNode->removeFromParent();
        });
        if(CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
            cancelButton->setPosition(Vec2(300, 65));
    
    Label* pContent= Label::createWithTTF("Rewarded Ads is currently unavailable",font_felt.c_str(), fontsize(40),Size(504*0.9,360*0.8),TextHAlignment::CENTER, TextVAlignment::CENTER);
    pContent->setDimensions(504*0.7,360*0.5);
    pContent->setPosition(Vec2(0, 50));
    dialogNode->addChild(pContent);

    Director::getInstance()->getRunningScene()->addChild(dialogNode, INT_MAX);
        dialogNode->setPosition(Director::getInstance()->getVisibleOrigin() + Director::getInstance()->getVisibleSize() * 0.5);
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
//    if(Director::getInstance()->getRunningScene()->getChildByTag(TAG_REWARD_LOADING))
//        return;
//    auto loading = AdsLoadingScene::create();
//    loading->setTag(TAG_REWARD_LOADING);
//    Director::getInstance()->getRunningScene()->addChild(loading);
}

void LockUIManager::removeRewardLoading()
{
    if(Director::getInstance()->getRunningScene()->getChildByTag(TAG_REWARD_LOADING))
    {
        Director::getInstance()->getRunningScene()->removeChildByTag(TAG_REWARD_LOADING);
    }
}

void LockUIManager::setLockPos(Vec2 pos)
{
    for(auto itor: _mapRewardInfos)
    {
        auto parent = _mapLockParents[itor.first];
        if(!parent)
            continue;
        Node* lock = nullptr;
        if(parent->getChildByTag(TAG_REWARD_LOCK))
            lock = parent->getChildByTag(TAG_REWARD_LOCK);
        if(lock != nullptr)
            lock->setPosition(pos);
    }
    
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

