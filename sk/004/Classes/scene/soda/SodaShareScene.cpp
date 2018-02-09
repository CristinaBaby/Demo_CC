//
//  SodaShareScene.cpp
//  shake004
//
//  Created by liji on 16/10/31.
//
//

#include "SodaShareScene.h"
#include "ESDataManager.h"
#include "JuiceBottleNode.h"
#include "SceneHeader.h"
SodaShareScene::SodaShareScene()
{

}

SodaShareScene::~SodaShareScene()
{

}

bool SodaShareScene::init()
{
    if (!ShareBaseScene::init())
    {
        return false;
    }
    this->initFood();
    
    
    return true;
}

void SodaShareScene::initFood()
{
    auto food = JuiceBottleNode::create();
    this->addToContentLayer(food,4);
    food->setPosition(CMVisibleRect::getPosition(320, 440));
    food->showDrink();

    if (!kESDataManager->getSodaFood().isServe)
    {
        this->setBackground("res/popcorn/eat_bg.png");
        auto resetDrink = [=]
        {
            kSoundInstance->playRandomStepFinishVoice();

            auto eatAgainButton = Button::create("ui/tap/drinkagain.png");
            eatAgainButton->setPosition(CMVisibleRect::getCenterOfScene()+Vec2(0,-100));
            this->addToUILayer(eatAgainButton, 20);
            
            eatAgainButton->runAction(ActionHelper::createFloatAction());
            
            eatAgainButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
                
                if (type == Widget::TouchEventType::ENDED)
                {
                    food->resetDrink();
                    
                    eatAgainButton->getActionManager()->removeAllActionsFromTarget(eatAgainButton);
                    eatAgainButton->runAction(Sequence::create(EaseExponentialIn::create(Spawn::create(RotateBy::create(0.2, 180.0), ScaleTo::create(0.2, 0.0), NULL)), RemoveSelf::create(), NULL));
                }
            });
        };
        
        food->enableDrink(nullptr, resetDrink);
        this->initEatHint(false);
        

        
    }
    else
    {
        this->chooseServe();
        food->setScale(0.7f);
        food->setPosition(CMVisibleRect::getPosition(320, 470));
        _girlWalkedIn = [=]
        {
            this->registServe(food, [=]
            {
                food->runAction(ScaleTo::create(1.2f, 0));
                this->girlEat();
                this->runAction(Sequence::create(DelayTime::create(1.2f),
                                                 CallFunc::create(CC_CALLBACK_0(SodaShareScene::girlSmile, this)),
                                                 NULL));
            });
        };
        this->_foodServedOver=[=]
        {
            this->showServeAgain([]
                                 {
                                     SceneManager::getInstance()->replaceWithDefaultTransition(SodaShareScene::scene());
                                 });
        };
    }
}
