//
//  PopcornNode.cpp
//  shake004
//
//  Created by liji on 16/10/25.
//
//

#include "PopcornNode.h"
#include "ComponentTouchMove.h"
PopcornNode* PopcornNode::createPopcorn(bool canEat)
{
    PopcornNode *pRet = new PopcornNode();
    if (pRet && pRet->initPopcorn(canEat))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}


bool PopcornNode::initPopcorn(bool canEat)
{
    if (!MakeBaseNode::init())
    {
        return false;
    }
    
    _boxBottom = Sprite::create("res/popcorn/7/box/box_down.png");
    this->addChild(_boxBottom,2);
    
    _boxTop = Sprite::create();
    this->addChild(_boxTop,INT_MAX);
    
    if (canEat)
    {
//        this->initBoxClosed();
    }
    else
    {
        this->showPopcorn();
    }
    
    this->updateBox();

    return true;
}

void PopcornNode::showPopcorn()
{
    auto food = kESDataManager->getPopcornFood();
    _popcorn = Sprite::create(StringUtils::format("res/popcorn/7/box_popcorn%d.png",food.flavorIndex));
    _popcorn->setPosition(0, 229);
    this->addChild(_popcorn,2);
    
    auto mask = Sprite::create(StringUtils::format("res/popcorn/7/box_popcorn%d.png",food.flavorIndex));
    _maskNode = ClippingNode::create(mask);
    _maskNode->setAlphaThreshold(0);
    _popcorn->addChild(_maskNode,2);
    _maskNode->setPosition(_popcorn->getContentSize()/2);

}

void PopcornNode::updateBox()
{
    auto food = kESDataManager->getPopcornFood();
    
    if (food.boxIndex == -1)
    {
        food.boxIndex = 1;
        kESDataManager->setPopcornFood(food);
    }
    
    _boxTop->setTexture(StringUtils::format("res/popcorn/7/box/box_%d.png",kESDataManager->getPopcornFood().boxIndex));
}

void PopcornNode::addSprinkle(string sprinklePath, Vec2 worldPos)
{
    auto sprinkle = Sprite::create(sprinklePath);
    _maskNode->addChild(sprinkle,10);
    sprinkle->setScale(0);
    sprinkle->setPosition(_maskNode->convertToNodeSpace(worldPos));
    
    float scale = rand()%4*0.1 + 0.6;
    sprinkle->runAction(EaseBackOut::create(ScaleTo::create(0.3f, scale)));
    sprinkle->setRotation(rand()%360);
}

void PopcornNode::addFood(string foodPath)
{
    auto food = Sprite::create(foodPath);
    _popcorn->addChild(food,10);
    food->setPosition(_popcorn->getContentSize()/2);
    food->setScale(0);
    float scale = rand()%4*0.1 + 1;
    food->runAction(EaseBackOut::create(ScaleTo::create(0.3f, scale)));
    food->setRotation(rand()%360);
    
    auto cTouch = ComponentTouchMove::create();
    food->addComponent(cTouch);
    Rect r = Rect(81,57, 224,180);
    r.origin = _popcorn->convertToWorldSpace(r.origin);
    cTouch->setMoveArea(r);
    cTouch->setNeedTouchTarget(true);
    
}

void PopcornNode::addSyrup(string foodPath)
{
    if (!_syrup)
    {
        _syrup = Sprite::create();
        _popcorn->addChild(_syrup,11);
        _syrup->setPosition(185,167);
    }
    _syrup->setTexture(foodPath);
}


void PopcornNode::savePopcornImage(const std::function<void(RenderTexture*, const std::string&)>& callback)
{
    Rect r = _popcorn->getBoundingBox();
    
    auto renderTexture1 = RenderTexture::create(r.size.width, r.size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    renderTexture1->setKeepMatrix(true);
    
    Rect fullRect1 = Rect(0.0, 0.0,  Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
    Rect fullViewport1 = Rect(0.0, 0.0, Director::getInstance()->getWinSizeInPixels().width, Director::getInstance()->getWinSizeInPixels().height);
    
    renderTexture1->setVirtualViewport(Vec2(0,0), fullRect1, fullViewport1);
    _popcorn->setPosition(r.size/2);
    renderTexture1->begin();
    _popcorn->visit();
    renderTexture1->end();
    _director->getRenderer()->render();
    _popcorn->setPosition(Vec2(0, 229));
    
    renderTexture1->saveToFile(shareNamePopCorn, true, callback);

}

void PopcornNode::showShare()
{
    if (_popcorn)
    {
        auto path = FileUtils::getInstance()->getWritablePath()+ shareNamePopCorn;
        if (_director->getTextureCache()->getTextureForKey("paint_popcorn"))
        {
            _director->getTextureCache()->removeTextureForKey("paint_popcorn");
        }
        Image *img = new Image();
        if(img->initWithImageFile(path))
        {
            _popcorn->setTexture(_director->getTextureCache()->addImage(img, "paint_popcorn"));
            img->autorelease();
        }
    }
    
}

