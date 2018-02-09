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
#include "SceneHeader.h"
#include "RateUsNode.h"
//#include "PizzaconeNode.h"
//#include "CorndogPlate.h"
//#include "CottonCandyNode.h"


REGIST_SECENE(ChooseServeScene)

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
//    _screenEventName = "food0.0_milkshake_serve";
    this->setBackground("res/select/bg2.png");
    
    auto alpha = LayerColor::create(Color4B(0, 0, 0, 188), 9999, 9999);
    alpha->ignoreAnchorPointForPosition(false);
    alpha->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(alpha);

    
    switch (kESDataManager->getCurrentFood())
    {
//        case ESDataManager::FoodType::kFoodPizzacone:
//        {
////            auto food = PizzaconeNode::create();
////            this->addToContentLayer(food,4);
////            food->setPosition(CMVisibleRect::getPosition(320, 240));
////            food->showShare(false);
////            food->setScaleX(0);
////            food->runAction(EaseBackOut::create(ScaleTo::create(0.4f, 0.65f)));
//        }
//            break;
//        case ESDataManager::FoodType::kFoodCottonCandy:
//        {
////            auto food = CottonCandyNode::create();
////            this->addToContentLayer(food,99);
////            food->setPosition(CMVisibleRect::getPosition(320, 220));
////            food->showShare(false);
////            food->setScaleX(0);
////            food->runAction(EaseBackOut::create(ScaleTo::create(0.4f, 0.85)));
//        }
//            break;
//        case ESDataManager::FoodType::kFoodCorndog:
//        {
////            auto food = CorndogPlate::create();
////            this->addToContentLayer(food,4);
////            food->setPosition(CMVisibleRect::getPosition(320, 240));
////            food->showShare(false);
////            food->setScaleX(0);
////            food->runAction(EaseBackOut::create(ScaleTo::create(0.4f, 1)));
//        }
//            break;
            
        default:
            break;
    }
    
    if (kESDataManager->getShowRate())
    {
        auto rate = RateUsNode::create();
        this->addToUILayer(rate,999);
        rate->setPosition(CMVisibleRect::getPosition(0, 0));
    }

    kESDataManager->setShowRate(false);
    
//    this->hideHomeButton();

//    auto preBtn = Button::create("ui/tap/back.png");
//    preBtn->setAnchorPoint(Vec2(0, 1));
//    preBtn->setPosition(CMVisibleRect::getPosition(20, 20,kBorderLeft,kBorderTop));
//    this->addToUILayer(preBtn);
//    preBtn->setTag(11);
//
//    preBtn->addTouchEventListener([=](Ref* r , Widget::TouchEventType type)
//    {
//        if (type == Widget::TouchEventType::ENDED)
//        {
//            SceneManager::getInstance()->replaceWithDefaultTransition(SodaDecScene::createSceneWithStepIndex(1));
//        }
//    });
    
//    kAudioUtil->playEffect(kVoiceChooseServe);
    
    auto light = Sprite::create("ui/serveoreat/bg.png");
    this->addToContentLayer(light,1);
    light->setPosition(CMVisibleRect::getPosition(320, 600));
    
    auto serveBtn = Button::create("ui/serveoreat/serve.png");
    serveBtn->loadTexturePressed("ui/serveoreat/serve.png");
    this->addToContentLayer(serveBtn,10,11);
    serveBtn->setPosition(CMVisibleRect::getPosition(422,624+30));
    registTouchEvent(serveBtn);
    
    auto eatBtn = Button::create("ui/serveoreat/eat.png");
    eatBtn->loadTexturePressed("ui/serveoreat/eat.png");
    this->addToContentLayer(eatBtn,10,10);
    eatBtn->setPosition(CMVisibleRect::getPosition(190,552+30));
    registTouchEvent(eatBtn);
    

    this->_btnClicked = [=](Button* btn)
    {
        _director->getEventDispatcher()->removeAllEventListeners();
        kAudioUtil->playEffect(kSoundClickButton);
        
        
        if (btn->getTag() == 10)//eat
        {
            
//            switch (kESDataManager->getCurrentFood())
//            {
//                case ESDataManager::FoodType::kFoodPizzacone:
//                {
////                    auto food = kESDataManager->getPizzaconeFood();
////                    food.isServe = false;
////                    kESDataManager->setPizzaconeFood(food);
////                    SceneManager::getInstance()->replaceWithDefaultTransition(PizzaconeShareScene::scene());
//                }
//                    break;
//                case ESDataManager::FoodType::kFoodCottonCandy:
//                {
////                    auto food = kESDataManager->getCottonCandyFood();
////                    food.isServe = false;
////                    kESDataManager->setCottonCandyFood(food);
////                    SceneManager::getInstance()->replaceWithDefaultTransition(CottonCandyShareScene::scene());
//                }
//                    break;
//                case ESDataManager::FoodType::kFoodCorndog:
//                {
////                    auto food = kESDataManager->getCorndogFood();
////                    food.isServe = false;
////                    kESDataManager->setCorndogFood(food);
////                    SceneManager::getInstance()->replaceWithDefaultTransition(CorndogShareScene::scene());
//                }
//                    break;
//                    
//                default:
//                    break;
//            }
//        }
//        else if (btn->getTag() == 11)//serve
//        {
//            switch (kESDataManager->getCurrentFood())
//            {
//                case ESDataManager::FoodType::kFoodPizzacone:
//                {
////                    auto food = kESDataManager->getPizzaconeFood();
////                    food.isServe = true;
////                    kESDataManager->setPizzaconeFood(food);
////                    SceneManager::getInstance()->replaceWithDefaultTransition(PizzaconeShareScene::scene());
//                }
//                    break;
//                case ESDataManager::FoodType::kFoodCottonCandy:
//                {
////                    auto food = kESDataManager->getCottonCandyFood();
////                    food.isServe = true;
////                    kESDataManager->setCottonCandyFood(food);
////                    SceneManager::getInstance()->replaceWithDefaultTransition(CottonCandyShareScene::scene());
//                }
//                    break;
//                case ESDataManager::FoodType::kFoodCorndog:
//                {
////                    auto food = kESDataManager->getCorndogFood();
////                    food.isServe = true;
////                    kESDataManager->setCorndogFood(food);
////                    SceneManager::getInstance()->replaceWithDefaultTransition(CorndogShareScene::scene());
//                }
//                    break;
//                    
//                default:
//                    break;
//            }
        }
        
    };
    
    return true;
}
