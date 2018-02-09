//
//  ChooseFoodScene.cpp
//  ccats1061
//
//  Created by liji on 16/8/25.
//
//

#include "ChooseFoodScene.h"
#include "SceneHeader.h"
#include "AdsLoadingScene.h"
#include "RewardManager.h"
#include "LockUIManager.h"
#include "MiniShopScene.h"
REGIST_SECENE(ChooseFoodScene)
ChooseFoodScene::ChooseFoodScene()
{
    
}

ChooseFoodScene::~ChooseFoodScene()
{
    
}

bool ChooseFoodScene::init()
{
    if (!ESMakeBaseScene::init())
    {
        return false;
    }
    
    kAudioUtil->playEffect(kVoiceChooseFood);
    
    this->setBackground("res/start2/bg.png");
    
    auto call = [=](Ref* ref,Widget::TouchEventType type)
    {
        auto btn = dynamic_cast<Button*>(ref);
        if (type == Widget::TouchEventType::ENDED)
        {
            
            kAudioUtil->stopAllEffect();
            kAudioUtil->playEffect(kSoundClickButton);
            
            
            int tag = btn->getTag();

            auto foodstring = ShopScene::getAllFoodString();
            CCLOG("*************   food %s   ",foodstring.c_str());

            bool hasPurchased = kIAPManager->isPackagePurchased(foodstring);
            if (!hasPurchased)
            {
                if (kIAPManager->isFree("pack", tag))
                {
                    auto keyObject = btn->getUserObject();
                    if(keyObject)
                    {
                        string key = ((__String*)(keyObject))->getCString();
                        if(kRewardManager->isLocked(key))
                        {
                            //如果能找到reward名字的lock，显示reward，否则显示iap
                            if (btn->getChildByName(LOCK_NAME_REWARD))
                            {
                                static bool s_isClicked = false;
                                if (s_isClicked)
                                {
                                    return;
                                }
                                s_isClicked = true;
                                this->runAction(Sequence::create(DelayTime::create(0.5f),
                                                                 CallFunc::create([=]
                                                                                  {
                                                                                      s_isClicked = false;
                                                                                  }),
                                                                 NULL));

                                auto node = MiniShopScene::create();
                                this->addChild(node,9999);
                                node->setPosition(CMVisibleRect::getPosition(0, 0));
                                node->showAllFoodButton();
                                node->_onRewardButtonClicked = [=]
                                {
                                    kRewardManager->showRewardAds(key);
                                };
                                node->_onShopButtonClicked = [=]
                                {
                                    ShopScene* layer = ShopScene::create();
                                    layer->setPosition(CMVisibleRect::getPosition(0, 0));
                                    this->addChild(layer,9999);
                                };
                            }
                            return;
                        }
                    }
                }
            }
            _director->getEventDispatcher()->removeAllEventListeners();

            switch (btn->getTag())
            {
                case 1:
                {
                    kESDataManager->setCurrentFood(ESDataManager::FoodType::kFoodCakepop);
                    
                    auto call = [=]
                    {
//                        if (kESDataManager->showAds())
//                        {
//                            AdsLoadingScene::showLoading<AdsLoadingScene>(true,nullptr,INT_MAX);
//                            AdsLoadingScene::loadingDoneCallback = [=]
//                            {
//                                SceneManager::getInstance()->replaceWithDefaultTransition(RainbowMakeScene::scene());
//                                kAudioUtil->playBackGroundMusic(kBGMInModule);
//                            };
//                        }
//                        else
//                        {
//                            SceneManager::getInstance()->replaceWithDefaultTransition(RainbowMakeScene::scene());
//                            kAudioUtil->playBackGroundMusic(kBGMInModule);
//                        }
                        SceneManager::getInstance()->replaceWithDefaultTransition(RainbowMakeScene::scene());
                        kAudioUtil->playBackGroundMusic(kBGMInModule);
                    };
                    kAudioUtil->playEffect("sound/voice over1/foodChoosed/Rainbow cake pops are the best.mp3");
                    this->runAction(Sequence::create(DelayTime::create(1.3),
                                                     CallFunc::create(call),
                                                     NULL));
                }
                    break;
                case 2:
                {
                    kESDataManager->setCurrentFood(ESDataManager::FoodType::kFoodCupcake);

                    kAudioUtil->playEffect("sound/voice over1/foodChoosed/Rainbow cupcakes are the secret.mp3");
                    this->runAction(Sequence::create(DelayTime::create(1.2),
                                                     CallFunc::create([]
                                                                      {
//                                                                          if (kESDataManager->showAds())
//                                                                          {
//                                                                              AdsLoadingScene::showLoading<AdsLoadingScene>(true,nullptr,INT_MAX);
//                                                                              AdsLoadingScene::loadingDoneCallback = [=]
//                                                                              {
//                                                                                  SceneManager::getInstance()->replaceWithDefaultTransition(RainbowMakeScene::scene());
//                                                                                  kAudioUtil->playBackGroundMusic(kBGMInModule);
//                                                                              };
//                                                                          }
//                                                                          else
//                                                                          {
//                                                                              SceneManager::getInstance()->replaceWithDefaultTransition(RainbowMakeScene::scene());
//                                                                              kAudioUtil->playBackGroundMusic(kBGMInModule);
//                                                                          }
                                                                          SceneManager::getInstance()->replaceWithDefaultTransition(RainbowMakeScene::scene());
                                                                          kAudioUtil->playBackGroundMusic(kBGMInModule);

                                                                      }),
                                                     NULL));
                    
                    
                }
                    break;
                case 3:
                {
                    kAudioUtil->playEffect("sound/voice over1/foodChoosed/Rainbow slushy can make.mp3");
                    this->runAction(Sequence::create(DelayTime::create(1.5),
                                                     CallFunc::create([]
                                                                      {
//                                                                          if (kESDataManager->showAds())
//                                                                          {
//                                                                              AdsLoadingScene::showLoading<AdsLoadingScene>(true,nullptr,INT_MAX);
//                                                                              AdsLoadingScene::loadingDoneCallback = [=]
//                                                                              {
//                                                                                  SceneManager::getInstance()->replaceWithDefaultTransition(SodaMakeIceScene::scene());
//                                                                                  kAudioUtil->playBackGroundMusic(kBGMInModule);
//                                                                              };
//                                                                          }
//                                                                          else
//                                                                          {
//                                                                              SceneManager::getInstance()->replaceWithDefaultTransition(SodaMakeIceScene::scene());
//                                                                              kAudioUtil->playBackGroundMusic(kBGMInModule);
//                                                                          }
                                                                          SceneManager::getInstance()->replaceWithDefaultTransition(SodaMakeIceScene::scene());
                                                                          kAudioUtil->playBackGroundMusic(kBGMInModule);

                                                                      }),
                                                     NULL));
                    
                }
                    break;
                    
                default:
                    break;
            }
        }
    };
    
    //        kAudioUtil->playEffect(kVoiceChooseFood);
    
    Button* cakepopBtn = Button::create("res/start2/cakepop/cakepop_down.png");
    cakepopBtn->setPosition(CMVisibleRect::getPosition(200, 762));
    this->addToContentLayer(cakepopBtn,10,1);
    auto dec1 = Sprite::create("res/start2/cakepop/cakepop_up.png");
    dec1->setPosition(Vec2(186,218));
    cakepopBtn->addChild(dec1,1,1);
    log("======1==");
    {
        auto key = kRewardManager->getItemKey("rainbowcakepop", "pack", 1);
        cakepopBtn->setUserObject(__String::create(key));
        log("======2==");
        if(!LockUIManager::getInstance()->registerIAPLock(ShopScene::getAllFoodString(), "pack", 1, cakepopBtn))
        {
            log("======3==");
            LockUIManager::getInstance()->registerLock("rainbowcakepop", "pack", 1, cakepopBtn,Vec2(40,50));
        }
    }
    log("======4==");

    Button* cupcakeBtn = Button::create("res/start2/cupcake/cupcake_down.png");
    cupcakeBtn->setPosition(CMVisibleRect::getPosition(429,543));
    this->addToContentLayer(cupcakeBtn,10,2);
    auto dec2 = Sprite::create("res/start2/cupcake/cupcake_up.png");
    dec2->setPosition(Vec2(256,229));
    cupcakeBtn->addChild(dec2,2,2);
    
    Button* juiceBtn = Button::create("res/start2/slushy/slushy_down.png");
    juiceBtn->setPosition(CMVisibleRect::getPosition(295, 273));
    this->addToContentLayer(juiceBtn,10,3);
    auto dec3 = Sprite::create("res/start2/slushy/slushy_up.png");
    dec3->setPosition(Vec2(139,186));
    juiceBtn->addChild(dec3,2,3);
    {
        auto key = kRewardManager->getItemKey("soda", "pack", 1);
        juiceBtn->setUserObject(__String::create(key));
        if(!LockUIManager::getInstance()->registerIAPLock(ShopScene::getAllFoodString(), "pack", 1, juiceBtn))
        {
            LockUIManager::getInstance()->registerLock("soda", "pack", 1, juiceBtn,Vec2(40,50));
        }
    }


    cakepopBtn->addTouchEventListener(call);
    juiceBtn->addTouchEventListener(call);
    cupcakeBtn->addTouchEventListener(call);
        
    return true;
}

void ChooseFoodScene::onEnter()
{
    ESMakeBaseScene::onEnter();
    
    
}

