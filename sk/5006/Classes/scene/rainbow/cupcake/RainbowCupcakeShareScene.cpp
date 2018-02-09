//
//  RainbowCupcakeShareScene.cpp
//  shake5008
//
//  Created by liji on 17/1/12.
//
//

#include "RainbowCupcakeShareScene.h"
#include "ESDataManager.h"
#include "RainbowCupcakeNode.h"
#include "SceneHeader.h"
REGIST_SECENE(RainbowCupcakeShareScene)

RainbowCupcakeShareScene::RainbowCupcakeShareScene()
{
}

RainbowCupcakeShareScene::~RainbowCupcakeShareScene()
{
}

bool RainbowCupcakeShareScene::init()
{
    if (!ShareBaseScene::init())
    {
        return false;
    }
    this->initFood();
    
    
    return true;
}

void RainbowCupcakeShareScene::initFood()
{
    auto food = RainbowCupcakeNode::create();
    this->addToContentLayer(food,10);
    food->setPosition(CMVisibleRect::getPosition(320, 520));
    //    food->showCottonCandy(true);
    food->setScale(1.2f);

    
    food->showShare(true);
    this->setBackground("res/eat.png");
    food->_eatOver = [=]
    {
        auto eatAgainButton = Button::create("ui/tap/eatagain.png");
        eatAgainButton->setPosition(CMVisibleRect::getCenterOfScene()+Vec2(0,-100));
        this->addToUILayer(eatAgainButton, 20);
        
        eatAgainButton->runAction(ActionHelper::createFloatAction());
        
        eatAgainButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
            
            if (type == Widget::TouchEventType::ENDED)
            {
                food->resetEat();
                
                eatAgainButton->getActionManager()->removeAllActionsFromTarget(eatAgainButton);
                eatAgainButton->runAction(Sequence::create(EaseExponentialIn::create(Spawn::create(RotateBy::create(0.2, 180.0), ScaleTo::create(0.2, 0.0), NULL)), RemoveSelf::create(), NULL));
            }
        });
    };
    
    this->initEatHint(true);

    this->_preBtnClicked = [=]
    {
        this->_preBtnClicked = nullptr;
        SceneManager::getInstance()->replaceWithDefaultTransition(RainbowCupcakeDec2Scene::scene());
    };

//    if (!kESDataManager->getCottonCandyFood().isServe)
//    {
//        food->showShare(true);
//        food->setScale(0.9f);
//        this->setBackground("res/cotton_candy/cotton_candy_bg.png");
//        food->_eatOver = [=]
//        {
//            auto eatAgainButton = Button::create("ui/tap/eatagain.png");
//            eatAgainButton->setPosition(CMVisibleRect::getCenterOfScene()+Vec2(0,-100));
//            this->addToUILayer(eatAgainButton, 20);
//            
//            eatAgainButton->runAction(ActionHelper::createFloatAction());
//            
//            eatAgainButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
//                
//                if (type == Widget::TouchEventType::ENDED)
//                {
//                    food->resetEat();
//                    
//                    eatAgainButton->getActionManager()->removeAllActionsFromTarget(eatAgainButton);
//                    eatAgainButton->runAction(Sequence::create(EaseExponentialIn::create(Spawn::create(RotateBy::create(0.2, 180.0), ScaleTo::create(0.2, 0.0), NULL)), RemoveSelf::create(), NULL));
//                }
//            });
//        };
//        
//        this->initEatHint(true);
//    }
//    else
//    {
//        food->setPosition(CMVisibleRect::getPosition(320, 300));
//        food->showShare(false);
//        resetServe(food);
//        
//        this->_foodServedOver = [=]
//        {
//            auto food = CottonCandyNode::create();
//            this->addToContentLayer(food,90);
//            food->setPosition(CMVisibleRect::getPosition(320, 300));
//            //            food->showCottonCandy(true);
//            food->showShare(false);
//            resetServe(food);
//        };
//    }
}
