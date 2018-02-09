//
//  RainbowCakepopShareScene.cpp
//  shake5008
//
//  Created by liji on 17/1/13.
//
//

#include "RainbowCakepopShareScene.h"
#include "ESDataManager.h"
#include "CakePopNode.h"
#include "SceneHeader.h"
REGIST_SECENE(RainbowCakepopShareScene)

RainbowCakepopShareScene::RainbowCakepopShareScene()
{
}

RainbowCakepopShareScene::~RainbowCakepopShareScene()
{
}

bool RainbowCakepopShareScene::init()
{
    if (!ShareBaseScene::init())
    {
        return false;
    }
    this->initFood();
    
    
    return true;
}

void RainbowCakepopShareScene::initFood()
{
    auto food = CakePopNode::create();
    this->addToContentLayer(food,10);
    food->setPosition(CMVisibleRect::getPosition(320, 620));
    //    food->showCottonCandy(true);
    
    
    food->showShare(true);
//    food->setScale(0.9f);
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
    
    this->_preBtnClicked = [=]
    {
        this->_preBtnClicked = nullptr;
        SceneManager::getInstance()->replaceWithDefaultTransition(RainbowCakepopDecScene::scene());
    };
    
    this->initEatHint(true);
}