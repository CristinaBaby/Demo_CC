//
//  ChooseServeScene.cpp
//  shake004
//
//  Created by liji on 16/10/28.
//
//

#include "ChooseServeScene.h"
#include "ESDataManager.h"
#include "JuiceBottleNode.h"
#include "PopcornNode.h"
#include "TexasPizzaNode.h"
#include "SceneHeader.h"

ChooseServeScene::ChooseServeScene()
{

}

ChooseServeScene::~ChooseServeScene()
{

}

bool ChooseServeScene::init()
{
    if (!ESMakeBaseScene::init())
    {
        return false;
    }
    
    _screenEventName = "food0.0_movienight_serve";

    this->setBackground("res/popcorn/eat_bg.png");

    auto plat = Sprite::create("ui/chooseserve/bg.png");
    this->addToContentLayer(plat,3);
    plat->setPosition(CMVisibleRect::getPosition(320,308));
    
    auto light = Sprite::create("ui/chooseserve/light.png");
    this->addToContentLayer(light,3);
    light->setPosition(CMVisibleRect::getPosition(320,600));
    
    switch (kESDataManager->getCurrentFood())
    {
        case ESDataManager::FoodType::kFoodPizza:
        {
            auto food = TexasPizzaNode::create();
            this->addToContentLayer(food,4);
            food->setPosition(CMVisibleRect::getPosition(320, 440));
            food->showServe();
            food->setScaleX(0);
            food->runAction(EaseBackOut::create(ScaleTo::create(0.4f, 1)));
        }
            break;
        case ESDataManager::FoodType::kFoodPopcorn:
        {
            auto food = PopcornNode::createPopcorn();
            this->addToContentLayer(food,4);
            food->setPosition(CMVisibleRect::getPosition(320, 554));
            food->showShare();
            food->setScaleX(0);
            food->runAction(EaseBackOut::create(ScaleTo::create(0.4f, 1)));
        }
            break;
        case ESDataManager::FoodType::kFoodSoda:
        {
            auto food = JuiceBottleNode::create();
            this->addToContentLayer(food,4);
            food->setPosition(CMVisibleRect::getPosition(320, 584));
            food->showDrink();
            food->setScaleX(0);
            food->runAction(EaseBackOut::create(ScaleTo::create(0.4f, 1)));
        }
            break;
            
        default:
            break;
    }
    
    kAudioUtil->playEffect(kVoiceChooseServe);
    
    
    auto serveBtn = Button::create("ui/chooseserve/serve_btn.png");
    this->addToContentLayer(serveBtn,10,11);
    
    
    auto eatBtn = Button::create("ui/chooseserve/eat_btn.png");
    this->addToContentLayer(eatBtn,10,10);
    auto clickedCall = [=](Ref* f,Widget::TouchEventType type)
    {
        auto btn = dynamic_cast<Button*>(f);
        if (type == Widget::TouchEventType::ENDED)
        {
            if (btn->getTag() == 10)
            {
                switch (kESDataManager->getCurrentFood())
                {
                    case ESDataManager::FoodType::kFoodPizza:
                    {
                        auto food = kESDataManager->getTexasPizzaFood();
                        food.isServe = false;
                        kESDataManager->setTexasPizzaFood(food);
                        SceneManager::getInstance()->replaceWithDefaultTransition(TexasPizzaShareScene::scene());
                    }
                        break;
                    case ESDataManager::FoodType::kFoodPopcorn:
                    {
                        auto food = kESDataManager->getPopcornFood();
                        food.isServe = false;
                        kESDataManager->setPopcornFood(food);
                        SceneManager::getInstance()->replaceWithDefaultTransition(PopcornShareScene::scene());
                    }
                        break;
                    case ESDataManager::FoodType::kFoodSoda:
                    {
                        auto food = kESDataManager->getSodaFood();
                        food.isServe = false;
                        kESDataManager->setSodaFood(food);
                        SceneManager::getInstance()->replaceWithDefaultTransition(SodaShareScene::scene());
                    }
                        break;
                        
                    default:
                        break;
                }
            }
            else if (btn->getTag() == 11)
            {
                switch (kESDataManager->getCurrentFood())
                {
                    case ESDataManager::FoodType::kFoodPizza:
                    {
                        auto food = kESDataManager->getTexasPizzaFood();
                        food.isServe = true;
                        kESDataManager->setTexasPizzaFood(food);
                        SceneManager::getInstance()->replaceWithDefaultTransition(TexasPizzaShareScene::scene());
                    }
                        break;
                    case ESDataManager::FoodType::kFoodPopcorn:
                    {
                        auto food = kESDataManager->getPopcornFood();
                        food.isServe = true;
                        kESDataManager->setPopcornFood(food);
                        SceneManager::getInstance()->replaceWithDefaultTransition(PopcornShareScene::scene());
                    }
                        break;
                    case ESDataManager::FoodType::kFoodSoda:
                    {
                        auto food = kESDataManager->getSodaFood();
                        food.isServe = true;
                        kESDataManager->setSodaFood(food);
                        SceneManager::getInstance()->replaceWithDefaultTransition(SodaShareScene::scene());
                    }
                        break;
                        
                    default:
                        break;
                }
            }

        }
        
    };

    
    ActionHelper::moveInToSceneBounce(serveBtn, CMVisibleRect::getPosition(475, 229), kActionDirectionRight,[=]
                                      {
                                          serveBtn->addTouchEventListener(clickedCall);
                                      });

    ActionHelper::moveInToSceneBounce(eatBtn, CMVisibleRect::getPosition(177, 229), kActionDirectionLeft,[=]
    {
        eatBtn->addTouchEventListener(clickedCall);
    });
    
    
    return true;
}
