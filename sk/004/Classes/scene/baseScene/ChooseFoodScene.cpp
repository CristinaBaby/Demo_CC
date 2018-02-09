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

    this->setBackground("res/select/bg.png");
    
    auto showBtnCall =[=]
    {
        auto call = [=](Ref* ref,Widget::TouchEventType type)
        {
            auto btn = dynamic_cast<Button*>(ref);
            if (type == Widget::TouchEventType::ENDED)
            {
                
                kAudioUtil->stopAllEffect();
                kAudioUtil->playEffect(kSoundClickButton);

                _director->getEventDispatcher()->removeAllEventListeners();
                
                switch (btn->getTag())
                {
                    case 1:
                    {

                        if (kESDataManager->showAds())
                        {
                            AdsLoadingScene::showLoading<AdsLoadingScene>(true,nullptr,INT_MAX);
                            AdsLoadingScene::loadingDoneCallback = [=]
                            {
                                SceneManager::getInstance()->replaceWithDefaultTransition(TexasPizzaMakeScene::scene());
                            };
                        }
                        else
                        {
                            SceneManager::getInstance()->replaceWithDefaultTransition(TexasPizzaMakeScene::scene());
                        }
                        
                    }
                        break;
                    case 2:
                    {
                        kAudioUtil->playEffect("sound/voice over1/Popcorn/popcorn chosen.mp3");
                        
                        this->runAction(Sequence::create(DelayTime::create(3.6),
                                                         CallFunc::create([]
                        {
                            if (kESDataManager->showAds())
                            {
                                AdsLoadingScene::showLoading<AdsLoadingScene>(true,nullptr,INT_MAX);
                                AdsLoadingScene::loadingDoneCallback = [=]
                                {
                                    SceneManager::getInstance()->replaceWithDefaultTransition(PopcornMakeScene::scene());
                                };
                            }
                            else
                            {
                                SceneManager::getInstance()->replaceWithDefaultTransition(PopcornMakeScene::scene());
                            }
                        }),
                                                         NULL));
                        

                    }
                        break;
                    case 3:
                    {
                        kAudioUtil->playEffect("sound/voice over1/Soda/soda chosen.mp3");

                        
                        this->runAction(Sequence::create(DelayTime::create(4.1f),
                                                         CallFunc::create([]
                                                                          {
                                                                              if (kESDataManager->showAds())
                                                                              {
                                                                                  AdsLoadingScene::showLoading<AdsLoadingScene>(true,nullptr,INT_MAX);
                                                                                  AdsLoadingScene::loadingDoneCallback = [=]
                                                                                  {
                                                                                      SceneManager::getInstance()->replaceWithDefaultTransition(SodaMakeScene::scene());
                                                                                  };
                                                                              }
                                                                              else
                                                                              {
                                                                                  SceneManager::getInstance()->replaceWithDefaultTransition(SodaMakeScene::scene());
                                                                              }
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

        auto desk = Sprite::create("res/select/desk.png");
        desk->setPosition(CMVisibleRect::getCenterOfScene());
        this->addToContentLayer(desk,9);
        
        Button* pizzaBtn = Button::create("res/select/pizza.png");
        pizzaBtn->setPosition(CMVisibleRect::getPosition(302,392-600));
        this->addToContentLayer(pizzaBtn,10,1);
//        pizzaBtn->runAction(ActionHelper::createFloatAction());
//        pizzaBtn->setScale(0);
        pizzaBtn->runAction(Sequence::create(DelayTime::create(1.8),
                                             MoveBy::create(0.5f, Vec2(0, 600)),
                                             NULL));
        
        Button* juiceBtn = Button::create("res/select/soda.png");
        juiceBtn->setPosition(CMVisibleRect::getPosition(110-400, 614));
        this->addToContentLayer(juiceBtn,10,3);
//        juiceBtn->runAction(ActionHelper::createFloatAction());
//        juiceBtn->setScale(0);
        juiceBtn->runAction(Sequence::create(DelayTime::create(0.6),
                                             MoveBy::create(0.5f, Vec2(400, 0)),
                                             NULL));

        Button* popcornBtn = Button::create("res/select/popcorn.png");
        popcornBtn->setPosition(CMVisibleRect::getPosition(522+600,591));
        this->addToContentLayer(popcornBtn,10,2);
//        popcornBtn->runAction(ActionHelper::createFloatAction());
//        popcornBtn->setScale(0);
        popcornBtn->runAction(Sequence::create(DelayTime::create(1.2),
                                               MoveBy::create(0.5f, Vec2(-600, 0)),
                                               CallFunc::create([=]
        {
            pizzaBtn->addTouchEventListener(call);
            juiceBtn->addTouchEventListener(call);
            popcornBtn->addTouchEventListener(call);
        }),
                                             NULL));

        
        

        
    };
    
    showBtnCall();
    
    
    
    return true;
}

void ChooseFoodScene::onEnter()
{
    ESMakeBaseScene::onEnter();


}

