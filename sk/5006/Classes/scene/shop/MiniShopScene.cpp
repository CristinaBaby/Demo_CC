//
//  MiniShopScene.cpp
//  BHUG1062
//
//  Created by liji on 16/5/3.
//
//

#include "MiniShopScene.h"
#include "depends/component/screenadapter/CMVisibleRect.h"
#include "AdsManager.h"
#include "AdsLoadingScene.h"
#include "component/makeComponent/basicComponent/ComponentCycleScroll.h"
#include "SoundConfig.h"
#include "IAPAapter.h"

MiniShopScene::MiniShopScene()
{
}

MiniShopScene::~MiniShopScene()
{
}


bool MiniShopScene::init()
{
    if (!StudioLayer::initWithCSBFile("shop_mini.csb"))
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
    auto lis = EventListenerTouchOneByOne::create();
    lis->setSwallowTouches(true);
    lis->onTouchBegan=[](Touch* t,Event* e)
    {
        
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(lis, layer);
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
    
    auto closeBtn = dynamic_cast<ui::Button*>(Helper::seekNodeByName(_rootNode, "Button_close"));
    closeBtn->addTouchEventListener(CC_CALLBACK_2(MiniShopScene::onButtonClick, this));
    
    auto rewardBtn = dynamic_cast<ui::Button*>(Helper::seekNodeByName(_rootNode, "Button_video"));
    rewardBtn->addTouchEventListener(CC_CALLBACK_2(MiniShopScene::onButtonClick, this));
    
    auto shopBtn = dynamic_cast<ui::Button*>(Helper::seekNodeByName(_rootNode, "Button_shop"));
    shopBtn->addTouchEventListener(CC_CALLBACK_2(MiniShopScene::onButtonClick, this));

    auto fullBtn = dynamic_cast<ui::Button*>(Helper::seekNodeByName(_rootNode, "Button_full"));
    fullBtn->addTouchEventListener(CC_CALLBACK_2(MiniShopScene::onButtonClick, this));

    
//    auto restoreBtn = Button::create("ui/store/restore.png");
//    restoreBtn->setPosition(CMVisibleRect::getPosition(320,157));
//    this->addToUILayer(restoreBtn);
//    restoreBtn->setName("shop_btn_restore");
//    restoreBtn->addTouchEventListener(CC_CALLBACK_2(MiniShopScene::onButtonClick, this));
//
//    auto closeBtn = Button::create("ui/store/x_btn.png");
//    closeBtn->setPosition(CMVisibleRect::getPosition(557,784));
//    this->addToUILayer(closeBtn);
//    closeBtn->setName("shop_btn_close");
//    closeBtn->addTouchEventListener(CC_CALLBACK_2(MiniShopScene::onButtonClick, this));

//    auto full = Sprite::create("ui/store/fullversion.png");
//    this->addToContentLayer(full);
//    full->setPosition(CMVisibleRect::getPosition(300,337));
    
//    auto fullBtn = Button::create("ui/store/fullversion.png");
//    fullBtn->setPosition(CMVisibleRect::getPosition(323,531));
//    this->addToUILayer(fullBtn);
//    fullBtn->setName("shop_btn_BuyFull");
//    fullBtn->addTouchEventListener(CC_CALLBACK_2(MiniShopScene::onButtonClick, this));
//    
//    auto contentBtn = Button::create("ui/store/allcontents.png");
//    contentBtn->setPosition(CMVisibleRect::getPosition(323,293));
//    this->addToUILayer(contentBtn);
//    contentBtn->setName("shop_btn_BuyContent");
//    contentBtn->addTouchEventListener(CC_CALLBACK_2(MiniShopScene::onButtonClick, this));

    

    return true;
}

void MiniShopScene::onEnter()
{
    StudioLayer::onEnter();
    AdsLoadingScene::s_showAdsBanner = false;
    getActionTimeLine()->play("in", false);

}

void MiniShopScene::onExit()
{
    StudioLayer::onExit();
    AdsLoadingScene::s_showAdsBanner = true;
    
}

void MiniShopScene::showDecorationButton()
{
    auto all = dynamic_cast<ui::Button*>(Helper::seekNodeByName(_rootNode, "Button_allFood"));
    all->setVisible(false);
    all->setEnabled(false);
    
    auto decoration = dynamic_cast<ui::Button*>(Helper::seekNodeByName(_rootNode, "Button_decoration"));
    decoration->addTouchEventListener(CC_CALLBACK_2(MiniShopScene::onButtonClick, this));
    decoration->setVisible(true);

}

void MiniShopScene::showAllFoodButton()
{
    auto all = dynamic_cast<ui::Button*>(Helper::seekNodeByName(_rootNode, "Button_allFood"));
    all->addTouchEventListener(CC_CALLBACK_2(MiniShopScene::onButtonClick, this));
    all->setVisible(true);
    
    auto decoration = dynamic_cast<ui::Button*>(Helper::seekNodeByName(_rootNode, "Button_decoration"));
    decoration->setVisible(false);
    decoration->setEnabled(false);

}


void MiniShopScene::onButtonClick(Ref* ref,Widget::TouchEventType type)
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
                this->runAction(Sequence::create(
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
            if (name == "Button_video")
            {
                this->runAction(Sequence::create(
                                                 CallFunc::create([this]()
                                                                  {
                                                                      kAudioUtil->playEffect(kSoundShopClose);
                                                                      if (_onRewardButtonClicked)
                                                                          _onRewardButtonClicked();
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
            if (name == "Button_allFood")
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
            if (name == "Button_shop")
            {
                this->runAction(Sequence::create(
                                                 CallFunc::create([this]()
                                                                  {
                                                                      kAudioUtil->playEffect(kSoundShopClose);
                                                                      if (_onShopButtonClicked)
                                                                      {
                                                                          _onShopButtonClicked();
                                                                      }
                                                                  }),
                                                 DelayTime::create(0.1),
                                                 RemoveSelf::create(),
                                                 NULL));
                
                
            }

        }
            break;
            
        default:
            break;
    }
    
}
void MiniShopScene::onCheckBoxClick(Ref*,CheckBox::EventType)
{
    
}


