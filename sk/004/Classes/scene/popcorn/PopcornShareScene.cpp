//
//  PopcornShareScene.cpp
//  shake004
//
//  Created by liji on 16/10/31.
//
//

#include "PopcornShareScene.h"
#include "ESDataManager.h"
#include "SceneHeader.h"

PopcornShareScene::PopcornShareScene():
_popcorn(nullptr)
{

}

PopcornShareScene::~PopcornShareScene()
{

}

bool PopcornShareScene::init()
{
    if (!ShareBaseScene::init())
    {
        return false;
    }
    this->initFood();

    
    
    return true;
}

void PopcornShareScene::initFood()
{
    auto food = PopcornNode::createPopcorn();
    this->addToContentLayer(food,4);
    food->setPosition(CMVisibleRect::getPosition(320, 420));
    food->showShare();
    
    if (!kESDataManager->getPopcornFood().isServe)
    {
        this->registEat(food->getEatSprite());
        this->setBackground("res/popcorn/eat_bg.png");
        this->initEatHint();

    }
    else
    {
//        auto plate = Sprite::create("res/serve/tray.png");
//        plate->setPosition(CMVisibleRect::getPosition(320, 420,kBorderNone,kBorderBottom));
//        this->addToContentLayer(plate);
        this->chooseServe();
        food->setScale(0.7f);
        food->setPosition(CMVisibleRect::getPosition(320, 480));
        _girlWalkedIn = [=]
        {
            this->registServe(food, [=]
            {
                food->runAction(ScaleTo::create(1.2f, 0));
                this->girlEat();
                this->runAction(Sequence::create(DelayTime::create(1.2f),
                                                 CallFunc::create(CC_CALLBACK_0(PopcornShareScene::girlSmile, this)),
                                                 NULL));
            });
        };
        this->_foodServedOver=[=]
        {
            this->showServeAgain([]
                                 {
                                     SceneManager::getInstance()->replaceWithDefaultTransition(PopcornShareScene::scene());
                                 });
        };

    }
    
    
}
