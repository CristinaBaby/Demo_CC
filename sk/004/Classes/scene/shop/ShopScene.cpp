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

ShopScene::ShopScene():
_girl(nullptr),
_pageView(nullptr)
{
}

ShopScene::~ShopScene()
{
}


bool ShopScene::init()
{
    if (!StudioLayer::init())
    {
        return false;
    }
    
    this->setBackground("ui/shop/store.png");
    
    auto color = LayerColor::create(Color4B(0,0,0,205));
    this->addToBgLayer(color,-1);
    
    kAudioUtil->playEffect(kSoundShopPop);
    auto alpha = LayerColor::create(Color4B(0, 0, 0, 0), 9999, 9999);
    alpha->ignoreAnchorPointForPosition(false);
    alpha->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(alpha);
    
    auto lis = EventListenerTouchOneByOne::create();
    lis->setSwallowTouches(true);
    lis->onTouchBegan=[](Touch* t,Event* e)
    {
        
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(lis, alpha);
//    _eventDispatcher->addEventListenerWithFixedPriority(lis, -2);
    _eventDispatcher->setPriority(lis, -2);
    
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
    
    auto restoreBtn = Button::create("ui/shop/restore.png");
    restoreBtn->setPosition(CMVisibleRect::getPosition(103,147));
    this->addToUILayer(restoreBtn);
    restoreBtn->setName("shop_btn_restore");
    restoreBtn->addTouchEventListener(CC_CALLBACK_2(ShopScene::onButtonClick, this));

    auto closeBtn = Button::create("ui/shop/x.png");
    closeBtn->setPosition(CMVisibleRect::getPosition(577,797));
    this->addToUILayer(closeBtn);
    closeBtn->setName("shop_btn_close");
    closeBtn->addTouchEventListener(CC_CALLBACK_2(ShopScene::onButtonClick, this));

    
    auto fullBtn = Button::create("ui/shop/unlock-ALL.png");
    fullBtn->setPosition(CMVisibleRect::getPosition(327,606));
    this->addToUILayer(fullBtn);
    fullBtn->setName("shop_btn_BuyFull");
    fullBtn->addTouchEventListener(CC_CALLBACK_2(ShopScene::onButtonClick, this));

    
    auto contentBtn = Button::create("ui/shop/allcontents.png");
    contentBtn->setPosition(CMVisibleRect::getPosition(320,321));
    this->addToUILayer(contentBtn);
    contentBtn->setName("shop_btn_BuyContent");
    contentBtn->addTouchEventListener(CC_CALLBACK_2(ShopScene::onButtonClick, this));

    
//    icon->addTouchEventListener([=](Ref* obj,Widget::TouchEventType type)
//                                {
//                                    auto item = dynamic_cast<Button*>(obj);
//                                    if (type == Widget::TouchEventType::ENDED)
//                                    {
//                                        if(!SSCIAPManager::getInstance()->isPackagePurchased(item->getTag()+1))
//                                        {
//                                            SSCIAPManager::getInstance()->purchase(item->getTag()+1);
//                                        }
//                                        else
//                                        {
//                                            MessageBox("You have already purchased this package!", "");
//                                        }
//                                    }
//                                });

    return true;
}

void ShopScene::onEnter()
{
    StudioLayer::onEnter();
    AdsLoadingScene::s_showAdsBanner = false;
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CSIgnoreAllTouchNotification);
}

void ShopScene::onExit()
{
    StudioLayer::onExit();
    AdsLoadingScene::s_showAdsBanner = true;
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CSCancelIgnoreAllTouchNotification);
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
            if (name == "shop_btn_close")
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
            if (name == "shop_btn_back")
            {
                _curIapIndex--;
                if (_curIapIndex<0)
                {
                    _curIapIndex=4;
                }
                _pageView->scrollToPage(_curIapIndex);
                
            }
            if (name == "shop_btn_next")
            {
                _curIapIndex++;
                if (_curIapIndex>4)
                {
                    _curIapIndex=0;
                }
                _pageView->scrollToPage(_curIapIndex);
                
            }
            if (name == "shop_btn_buy")
            {
                if(!SSCIAPManager::getInstance()->isPackagePurchased(_curIapIndex+1))
                {
                   SSCIAPManager::getInstance()->purchase(_curIapIndex+1);
                }
                else
                {
                    MessageBox("You have already purchased this package!", "");
                }
                
            }
//            for (int i=0; i<8; i++)
//            {
//                string btnName = StringUtils::format("shop_li_%d",i+1);
//                if (name == btnName)
//                {
//                    if(!IAPManager::getInstance()->isPackagePurchased(i+1))
//                    {
//                       SSCIAPManager::getInstance()->purchase(i+1);
//                    }
//                    else
//                    {
//                        MessageBox("You have already purchased this package!", "");
//                    }
//                    
//                }
//            }
            
            if (name == "shop_btn_noads")
            {
                if(!SSCIAPManager::getInstance()->isPackagePurchased(6))
                {
                   SSCIAPManager::getInstance()->purchase(6);
                }
                else
                {
                    MessageBox("You have already purchased this package!", "");
                }
                
            }
            if (name == "shop_btn_restore")
            {
               SSCIAPManager::getInstance()->restore();
            }
            if (name == "shop_btn_BuyFull")
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
            if (name == "shop_btn_BuyContent")
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

}

