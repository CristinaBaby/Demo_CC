//
//  TexasPizzaNode.cpp
//  shake004
//
//  Created by liji on 16/10/27.
//
//

#include "TexasPizzaNode.h"
#include "ESDataManager.h"
#include "ComponentTouchMove.h"

bool TexasPizzaNode::init()
{
    if (!MakeBaseNode::init())
    {
        return false;
    }
    
    int pizzaIndex = kESDataManager->getTexasPizzaFood().foodIndex;

    string path = FileUtils::getInstance()->getWritablePath() + StringUtils::format("pizza_%d.png",pizzaIndex);
    if (!FileUtils::getInstance()->isFileExist(path))
    {
        path = "res/pizza/pizza/overlook/afterbake/garlicbread_1.png";
        _pizza = Sprite::create(path);
        this->addChild(_pizza);
    }
    else
    {
        Image *img = new Image();
        if(img->initWithImageFile(path))
        {
            _pizza = Sprite::createWithTexture(_director->getTextureCache()->addImage(img, "paint_pizza"));
            img->autorelease();
            this->addChild(_pizza);
        }

    }
    this->updateBox(kESDataManager->getTexasPizzaFood().boxIndex);

    auto plate = Sprite::create("res/popcorn/5/butter_plate.png");
    plate->setPosition(Vec2(-5, -27));
    this->addChild(plate,-1);
    plate->setScale(1.1);
    
    return true;
}

void TexasPizzaNode::updateBox(int index)
{
    if (!_box)
    {
        _box = Sprite::create();
        this->addChild(_box, 2);
        _box->setPosition(5, 74);
        auto btm = Sprite::create("res/decoration/box/box_down.png");
        this->addChild(btm,-1);
        btm->setPosition(5, 20);
        _boxBtm = btm;
    }
    string path = StringUtils::format("res/decoration/box/box_%d.png",index);
    _box->setTexture(path);
}

void TexasPizzaNode::updateBowtie(const string& filePath,bool canMove)
{
    if (!_bowtie)
    {
        _bowtie = Sprite::create();
        this->addChild(_bowtie, 5);
        if (canMove)
        {
            auto cTouch = ComponentTouchMove::create();
            _bowtie->addComponent(cTouch);
            cTouch->touchEndedCall=[=](Touch* t)
            {
                auto tPizza = kESDataManager->getTexasPizzaFood();
                tPizza.bowPos = _bowtie->getPosition();
                kESDataManager->setTexasPizzaFood(tPizza);
            };
            Rect r = _box->getBoundingBox();
            r.origin = this->convertToWorldSpaceAR(r.origin);
            cTouch->setMoveArea(r);
        }
    }
    _bowtie->setTexture(filePath);
    auto tPizza = kESDataManager->getTexasPizzaFood();
    _bowtie->setPosition(tPizza.bowPos);

}

void TexasPizzaNode::updateSticker(const string& filePath,bool canMove)
{
    if (!_sticker)
    {
        _sticker = Sprite::create();
        this->addChild(_sticker, 4);
        if (canMove)
        {
            auto cTouch = ComponentTouchMove::create();
            _sticker->addComponent(cTouch);
            cTouch->touchEndedCall=[=](Touch* t)
            {
                auto tPizza = kESDataManager->getTexasPizzaFood();
                tPizza.stickerPos = _sticker->getPosition();
                kESDataManager->setTexasPizzaFood(tPizza);
            };
            Rect r = _box->getBoundingBox();
            r.origin = this->convertToWorldSpace(r.origin);
            cTouch->setMoveArea(r);
        }
    }
    _sticker->setTexture(filePath);
    auto tPizza = kESDataManager->getTexasPizzaFood();
    _sticker->setPosition(tPizza.stickerPos);

}

void TexasPizzaNode::showEat()
{
    if (_bowtie)
        _bowtie->runAction(FadeOut::create(0.4f));
    if (_box)
    {
        _boxBtm->runAction(FadeOut::create(0.4f));
        _box->runAction(FadeOut::create(0.4f));
    }
    if (_sticker)
        _sticker->runAction(FadeOut::create(0.4f));
}

void TexasPizzaNode::showServe()
{
    this->updateBox(kESDataManager->getTexasPizzaFood().boxIndex);
    this->updateBowtie(StringUtils::format("res/decoration/bowknot/bowknot_%d.png",kESDataManager->getTexasPizzaFood().bowIndex));
    this->updateSticker(StringUtils::format("res/decoration/paster/paster%d.png",kESDataManager->getTexasPizzaFood().stickerIndex));
}

Sprite* TexasPizzaNode::getEatSprite()
{
    return _pizza;
}

