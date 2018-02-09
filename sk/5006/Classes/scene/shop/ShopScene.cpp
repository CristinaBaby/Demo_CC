//
//  ShopScene.cpp
//  BHUG1062
//
//  Created by liji on 16/5/3.
//
//

#include "ShopScene.h"
#include "depends/component/screenadapter/CMVisibleRect.h"
#include "AdsManager.h"
#include "AdsLoadingScene.h"
#include "component/makeComponent/basicComponent/ComponentCycleScroll.h"
#include "SoundConfig.h"
#include "IAPAapter.h"

ShopScene::ShopScene()
{
}

ShopScene::~ShopScene()
{
}


bool ShopScene::init()
{
    if (!StudioLayer::initWithCSBFile("shop.csb"))
    {
        return false;
    }
    
    _uiAction->gotoFrameAndPause(0);
    auto layer = LayerColor::create(Color4B(0, 0, 0, 160));
    this->addChild(layer,-1);
    layer->setContentSize(Size(9999,9999));
    layer->setPosition(-200, -200);

//    auto bg = Sprite::create("ui/store/bg.png");
//    this->addToContentLayer(bg);
//    bg->setPosition(CMVisibleRect::getCenterOfScene());
//    
//    kAudioUtil->playEffect(kSoundShopPop);
//    auto alpha = LayerColor::create(Color4B(0, 0, 0, 188), 9999, 9999);
//    alpha->ignoreAnchorPointForPosition(false);
//    alpha->setAnchorPoint(Vec2(0.5, 0.5));
//    this->addChild(alpha);
//    
//    auto lis = EventListenerTouchOneByOne::create();
//    lis->setSwallowTouches(true);
//    lis->onTouchBegan=[](Touch* t,Event* e)
//    {
//        
//        return true;
//    };
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(lis, alpha);
////    _eventDispatcher->addEventListenerWithFixedPriority(lis, -2);
//    _eventDispatcher->setPriority(lis, -2);
    
    SSCIAPManager::getInstance()->setAfterRestoreCallback(([=](bool isScucess , bool isHasData)
    {
        if(isScucess)
        {
            if(isHasData)
            {
                MessageBox("Restore successfully!", "");
            }
            else
            {
                MessageBox("Sorry! It looks like you haven't purchased anything yet!", "");
            }
        }
        else
        {
            MessageBox("Sorry, restore transaction failed!", "");
        }
        if(!SSCIAPManager::getInstance()->isShowAds()){
            AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
        }
    }));
    
    
   SSCIAPManager::getInstance()->setAfterPurchaseCallback([=](bool flag){
        
        if (flag && !kIAPManager->isShowAds())
        {
            AdsManager::getInstance()->removeAds(kTypeBannerAds);
        }
        
    });
    
    auto restoreBtn = dynamic_cast<ui::Button*>(Helper::seekNodeByName(_rootNode, "Button_restore"));
    restoreBtn->addTouchEventListener(CC_CALLBACK_2(ShopScene::onButtonClick, this));
//
    auto closeBtn = dynamic_cast<ui::Button*>(Helper::seekNodeByName(_rootNode, "Button_close"));
    closeBtn->addTouchEventListener(CC_CALLBACK_2(ShopScene::onButtonClick, this));

    auto fullBtn = dynamic_cast<ui::Button*>(Helper::seekNodeByName(_rootNode, "Button_full"));
    fullBtn->addTouchEventListener(CC_CALLBACK_2(ShopScene::onButtonClick, this));
    auto allFoodBtn = dynamic_cast<ui::Button*>(Helper::seekNodeByName(_rootNode, "Button_allfood"));
    allFoodBtn->addTouchEventListener(CC_CALLBACK_2(ShopScene::onButtonClick, this));
    auto decBtn = dynamic_cast<ui::Button*>(Helper::seekNodeByName(_rootNode, "Button_decoration"));
    decBtn->addTouchEventListener(CC_CALLBACK_2(ShopScene::onButtonClick, this));
    auto adsBtn = dynamic_cast<ui::Button*>(Helper::seekNodeByName(_rootNode, "Button_noads"));
    adsBtn->addTouchEventListener(CC_CALLBACK_2(ShopScene::onButtonClick, this));

    return true;
}

void ShopScene::onEnter()
{
    StudioLayer::onEnter();
    AdsLoadingScene::s_showAdsBanner = false;
    getActionTimeLine()->play("in", false);

}

void ShopScene::onExit()
{
    StudioLayer::onExit();
    AdsLoadingScene::s_showAdsBanner = true;
    
}


void ShopScene::onButtonClick(Ref* ref,Widget::TouchEventType type)
{
    auto btn = dynamic_cast<Button*>(ref);
    string name = btn->getName();

    switch (type)
    {
        case Widget::TouchEventType::BEGAN:
        {
            //当点击到button的时候，屏蔽cycleScroll事件
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CSIgnoreTouchOnceNotification);
            break;
        }
        case Widget::TouchEventType::ENDED:
        {
            if (name == "Button_close")
            {
                this->runAction(Sequence::create(DelayTime::create(0.1),
                                                 CallFunc::create([this]()
                {
                    kAudioUtil->playEffect(kSoundShopClose);
                    if (onCloseCallback)
                        onCloseCallback();
                }),
                                                 DelayTime::create(0.1),
                                                 RemoveSelf::create(),
                                                 NULL));
            }
            if (name == "Button_full")
            {
                if(!SSCIAPManager::getInstance()->isPackagePurchased(1))
                {
                    SSCIAPManager::getInstance()->purchase(1);
                }
                else
                {
                    MessageBox("You have already purchased this package!", "");
                }
            }
            if (name == "Button_allfood")
            {
                if(!SSCIAPManager::getInstance()->isPackagePurchased(2))
                {
                    SSCIAPManager::getInstance()->purchase(2);
                }
                else
                {
                    MessageBox("You have already purchased this package!", "");
                }
            }
            if (name == "Button_decoration")
            {
                if(!SSCIAPManager::getInstance()->isPackagePurchased(3))
                {
                    SSCIAPManager::getInstance()->purchase(3);
                }
                else
                {
                    MessageBox("You have already purchased this package!", "");
                }
            }
            if (name == "Button_noads")
            {
                if(!SSCIAPManager::getInstance()->isPackagePurchased(4))
                {
                    SSCIAPManager::getInstance()->purchase(4);
                }
                else
                {
                    MessageBox("You have already purchased this package!", "");
                }
            }
            if (name == "Button_restore")
            {
               SSCIAPManager::getInstance()->restore();
            }
        }
            break;
            
        default:
            break;
    }
    
}
void ShopScene::onCheckBoxClick(Ref*,CheckBox::EventType)
{
    
}

string ShopScene::getAllContentString()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    return IAP_AllContent;
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //    kIAPManager->initIAP("config/IAP.csv");
    IAPAapter a;
    int index = a.getAndroidPlatformCode();
    log("===%d==" , index);
    if (index == 33)
    {
        return IAP_AllContent_A;

    }
    if (index == 32)
    {
        return IAP_AllContent_G;

    }
    
#endif
    
    return IAP_AllContent_G;

}

string ShopScene::getAllFoodString()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    return IAP_AllFood;
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //    kIAPManager->initIAP("config/IAP.csv");
    IAPAapter a;
    int index = a.getAndroidPlatformCode();
    log("getAndroidPlatformCode ===%d==" , index);
    if (index == 33)
    {
        return IAP_AllFood_A;
        
    }
    if (index == 32)
    {
        return IAP_AllFood_G;
        
    }
    
   
    
#endif
    
     return IAP_AllFood_G;//default google platform =.= ............... function must had return value
}


