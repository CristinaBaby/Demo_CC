//
//  TexasPizzaShareScene.cpp
//  shake004
//
//  Created by liji on 16/10/31.
//
//

#include "TexasPizzaShareScene.h"
#include "ESDataManager.h"
#include "TexasPizzaNode.h"
#include "SceneHeader.h"
TexasPizzaShareScene::TexasPizzaShareScene()
{

}

TexasPizzaShareScene::~TexasPizzaShareScene()
{

}

bool TexasPizzaShareScene::init()
{
    if (!ShareBaseScene::init())
    {
        return false;
    }
    this->initFood();
    

    
    return true;
}

void TexasPizzaShareScene::initFood()
{
    auto food = TexasPizzaNode::create();
    this->addToContentLayer(food,4);
    food->setPosition(CMVisibleRect::getPosition(320, 420));
    if (!kESDataManager->getTexasPizzaFood().isServe)
    {
        this->setBackground("res/popcorn/eat_bg.png");
        this->registEat(food->getEatSprite());
        food->showEat();
        this->initEatHint();
    }
    else
    {
        food->showServe();
        auto plate = Sprite::create("res/serve/tray.png");
        plate->setPosition(CMVisibleRect::getPosition(320, 420,kBorderNone,kBorderBottom));
        this->addToContentLayer(plate);
        this->chooseServe();
        food->setPosition(CMVisibleRect::getPosition(320, 450));

        _girlWalkedIn = [=]
        {
            this->registServe(food, [=]
            {
                food->runAction(ScaleTo::create(1.2f, 0));
                this->girlEat();
                this->runAction(Sequence::create(DelayTime::create(1.2f),
                                                 CallFunc::create(CC_CALLBACK_0(TexasPizzaShareScene::girlSmile, this)),
                                                 NULL));
            });
        };
        this->_foodServedOver=[=]
        {
            this->showServeAgain([]
                                 {
                                     SceneManager::getInstance()->replaceWithDefaultTransition(TexasPizzaShareScene::scene());
                                 });
        };

    }
    
    
}
