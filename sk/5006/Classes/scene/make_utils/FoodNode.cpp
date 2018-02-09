//
//  FoodNode.cpp
//  ccats024
//
//  Created by liji on 16/12/5.
//
//

#include "FoodNode.h"

bool FoodNode::init()
{
    if (!MakeBaseNode::init())
    {
        return false;
    }
    
    return true;
}

Rect FoodNode::getBoundingRect()
{
    auto original = this->convertToWorldSpace(Vec2::ZERO-_mainContent->getContentSize()/2);
    Rect r;
    r.origin = original;
    r.size = _mainContent->getContentSize();
    return r;
};

void FoodNode::resetEat()
{
    for (auto e : _eatVec)
    {
        e->reset();
    }

}

void FoodNode::registEat(Node* node)
{
    auto eatShape = Sprite::create("ui/eat1.png");
    auto eatComp = ComponentEat::create();
    eatComp->setEatShape(eatShape);
    eatComp->setAlphaThreshold(10);
    eatComp->setEatRate(0.05);
    eatComp->setName("eatComp");
    node->addComponent(eatComp);
    eatComp->eatingCallback = [=](Touch* touch)
    {
        static int x = 0;
        x++;
        if (x == 5)
        {
            x = 0;
            kSoundInstance->playRandomEatVoice();
        }
        kAudioUtil->playEffect(kSoundEat);
    };
    
    eatComp->finishEatCallback = [=]()
    {
        //            this->resetEat();
        if (_eatOver)
        {
            _eatOver();
        }
    };
    
    eatComp->start();
    _eatVec.push_back(eatComp);

}