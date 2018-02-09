//
//  RainbowCupcakeNode.cpp
//  shake5008
//
//  Created by liji on 17/1/11.
//
//

#include "RainbowCupcakeNode.h"
#include "ESDataManager.h"
#include "ScribbleNode.h"
#include "ComponentScribblePercent.h"
#include "ComponentTouchMove.h"

bool RainbowCupcakeNode::init()
{
    if (!FoodNode::init())
    {
        return false;
    }
    kESDataManager->initRainbowCupcakeColor();
    auto food = kESDataManager->getRainbowcupcakeFood();
    
    auto plate = Sprite::create("res/rainbow_cupcake/8/plate.png");
    this->addChild(plate,-1);
    plate->setPosition(0,-145);

    auto cupBtmPath = StringUtils::format("res/rainbow_cupcake/8/cup/cup%d_0.png",food.cupIndex);
    auto cupTopPath = StringUtils::format("res/rainbow_cupcake/8/cup/cup%d_1.png",food.cupIndex);
    auto btm = Sprite::create(cupBtmPath);
    auto cover = Sprite::create(cupTopPath);
    this->addChild(btm,1);
    this->addChild(cover,20);
    
    _eatNode = Node::create();
    this->addChild(_eatNode, 19);
    _noneEatNode.push_back(plate);
    _noneEatNode.push_back(btm);
    _noneEatNode.push_back(cover);
    
    int colorNumber = food.colorIndexVec.size();
    for (int i = 0; i<colorNumber-1; i++)
    {
        string path;
        if (i == 0)
        {
            path = StringUtils::format("res/rainbow_cupcake/8/cake_down/d%d.png",food.colorIndexVec.at(i));
        }
        if (i == 1)
        {
            path = StringUtils::format("res/rainbow_cupcake/8/cake_down/u%d.png",food.colorIndexVec.at(i));
        }
        auto sp = Sprite::create(path);
        _eatNode->addChild(sp,2);
        _foodVec.push_back(sp);

    }
    if (colorNumber == 1)
    {
        auto path = StringUtils::format("res/rainbow_cupcake/8/cake_down/d%d.png",food.colorIndexVec.back());
        auto sp = Sprite::create(path);
        _eatNode->addChild(sp,2);
        _foodVec.push_back(sp);
    }
    auto cakeTop = Sprite::create(StringUtils::format("res/rainbow_cupcake/8/cake_up/%d.png",food.colorIndexVec.back()));
    _eatNode->addChild(cakeTop,21);

    _foodVec.push_back(cakeTop);
    
    return true;
}


void RainbowCupcakeNode::scribbleTopping()
{
    auto food = kESDataManager->getRainbowcupcakeFood();
    int index = food.toppingIndex;
    
    if (_scribbleNode)
    {
        _scribbleNode->removeFromParent();_scribbleNode = nullptr;
    }
    
    _scribbleNode = ScribbleNode::create(264,321);
    _eatNode->addChild(_scribbleNode,999);
    _scribbleNode->useTarget(StringUtils::format("res/rainbow_cupcake/8/cream_topping/%d.png",index));
    _scribbleNode->useBrush("res/solid_brush.png", Scribble::eBrush);

    _compScribbleComplete = ComponentScribblePercent::create();
    _scribbleNode->addComponent(_compScribbleComplete);
    _compScribbleComplete->setScribbleTargetPath(StringUtils::format("res/rainbow_cupcake/8/cream_topping/%d.png",index));
    _compScribbleComplete->setIsScribbleBrushTypeEraser(false);
    _compScribbleComplete->start();
    this->updateTopping();
    _topping->setOpacity(45);

}

void RainbowCupcakeNode::scribble(Vec2 pos)
{
    if (_scribbleNode)
        _scribbleNode->paint(this->convertToNodeSpace(pos)+Vec2(0,-80));
    log ("%f,%f========",this->convertToNodeSpace(pos).x,this->convertToNodeSpace(pos).y);
    
}

void RainbowCupcakeNode::checkScribble(const function<void()> &finishedCallback)
{
    if (!_compScribbleComplete)
        return;
    if(_compScribbleComplete->getPercentComplete() > 0.85)
    {
        _compScribbleComplete = nullptr;
        if (_scribbleNode)
        {
            _scribbleNode->removeFromParent();_scribbleNode = nullptr;
        }
        this->updateTopping();
        
        if (finishedCallback)
            finishedCallback();
    }
}

void RainbowCupcakeNode::updateTopping()
{
    if (!_topping)
    {
        _topping = Sprite::create();
        _eatNode->addChild(_topping,22);
    }
    auto food = kESDataManager->getRainbowcupcakeFood();
    int index = food.toppingIndex;
    _topping->setTexture(StringUtils::format("res/rainbow_cupcake/8/cream_topping/%d.png",index));
    _topping->setOpacity(255);
}

void RainbowCupcakeNode::addMarshmallow(int index)
{
    auto m = Sprite::create(StringUtils::format("res/rainbow_cupcake/9/marshmallow%ds.png",index));
    _eatNode->addChild(m,31);
    m->setPosition(Vec2(0, 60));
}

void RainbowCupcakeNode::addRainBow()
{
    auto r = Sprite::create("res/rainbow_cupcake/9/topping_rainbow.png");
    _eatNode->addChild(r,30);
    r->setPosition(Vec2(0, 60));
}

void RainbowCupcakeNode::addOther(const string &path)
{
    auto food = Sprite::create(path);
    _topping->addChild(food,20);
    food->setPosition(Vec2(136, 176));
    food->setScale(0);
    float scale = rand()%4*0.1 + 0.7f;
    food->runAction(EaseBackOut::create(ScaleTo::create(0.3f, scale)));
    food->setRotation(random(-10,10));
    
    auto cTouch = ComponentTouchMove::create();
    food->addComponent(cTouch);
    Rect r = Rect(-96,-24, 189,128);
    r.origin = this->convertToWorldSpace(r.origin);
    cTouch->setMoveArea(r);
}

void RainbowCupcakeNode::showDec()
{
    this->updateTopping();
    this->addRainBow();
    this->addMarshmallow(1);
    this->addMarshmallow(2);
    

}

void RainbowCupcakeNode::addSprinkle(Node* s)
{
    _topping->addChild(s);
}

void RainbowCupcakeNode::saveImage()
{
    auto size = Size(264+50,321);
    auto renderTexture1 = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    _eatNode->setPosition(size/2);
//    for (auto n : _noneEatNode)
//    {
//        n->setVisible(false);
//    }
    
    renderTexture1->begin();
    _eatNode->visit();
    renderTexture1->end();
    _director->getRenderer()->render();
    renderTexture1->saveToFile(shareNameRainbowcupcake, true, nullptr);
    _eatNode->setPosition(Vec2(0, 0));
//    for (auto n : _noneEatNode)
//    {
//        n->setVisible(true);
//    }

    //    _director->getRenderer()->render();
}

void RainbowCupcakeNode::showShare(bool canEat)
{
    auto path = FileUtils::getInstance()->getWritablePath()+ shareNameRainbowcupcake;
    if (!FileUtils::getInstance()->isFileExist(path))
        return;
    if (!_eatSprite)
    {
        _eatSprite = Sprite::create();
        _eatNode->addChild(_eatSprite,10);
//        _eatSprite->setPosition(0, 100);
    }
    for (auto n : _foodVec)
    {
        n->setVisible(false);
    }
    _eatSprite->setTexture(path);
    if (_director->getTextureCache()->getTextureForKey("paint_rainbowCupcake"))
    {
        _director->getTextureCache()->removeTextureForKey("paint_rainbowCupcake");
    }
    Image *img = new Image();
    if (img->initWithImageFile(path))
    {
        _eatSprite->setTexture(_director->getTextureCache()->addImage(img, "paint_cottoncandy"));
        img->autorelease();
    }
    if (canEat)
    {
        this->registEat(_eatSprite);
    }
    
}


