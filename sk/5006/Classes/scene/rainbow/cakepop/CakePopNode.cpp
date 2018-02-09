//
//  CakePopNode.cpp
//  BHUG1066
//
//  Created by maxiang on 7/5/16.
//
//

#include "CakePopNode.h"
#include "ComponentTouchMove.h"
#include "ComponentMixer.h"
#include "ESDataManager.h"
CakePopNode::~CakePopNode()
{
    
    
}

bool CakePopNode::init()
{
    if (!MakeBaseNode::init())
        return false;
    
    string stickPath = StringUtils::format("res/cake_pop/step6_sticks/stick%d.png",kESDataManager->getRainbowcakepopFood().stickIndex);
    
    _eatNode = Node::create();
    addChild(_eatNode, 6);
    
    _stick = Sprite::create(stickPath);
    addChild(_stick, 1);
//    _stick->setScale(0.8f);
    _stick->setPosition(Vec2(0, -160));
    
    int flavorIndex = kESDataManager->getRainbowcakepopFood().chocolateIndex;
    auto file = StringUtils::format("res/cake_pop/step8/cake_pop/cake_pop%d.png",flavorIndex);

    _cottonCandy1 = Sprite::create(file);
    _eatNode->addChild(_cottonCandy1, 2);
    
    _mainContent = _cottonCandy1;
    
    //    auto name = kESDataManager->getCurrentCottonFlavorName();
    
    auto sp = Sprite::create(file);
    _sprinkleNode = ClippingNode::create(sp);
    _sprinkleNode->setAlphaThreshold(0);
    _eatNode->addChild(_sprinkleNode,10);
    
    
    return true;
}

void CakePopNode::addOther(const string &candyPath)
{
    auto candy = Sprite::create(candyPath);
    _eatNode->addChild(candy,10);
    
    auto moveComp = ComponentTouchMove::create();
    Rect r;
    r.origin = convertToWorldSpace(Vec2(-_cottonCandy1->getContentSize().width/2, -_cottonCandy1->getContentSize().height/2));
    r.size = _cottonCandy1->getContentSize();
    
    moveComp->setMoveArea(r);
    candy->addComponent(moveComp);
    
}

void CakePopNode::addSprinkle(Node* s)
{
    _cottonCandy1->addChild(s);
}

void CakePopNode::updateFace(string facePath)
{
    if (!_face)
    {
        _face = Sprite::create();
        _eatNode->addChild(_face, 3);
    }
    _face->setTexture(facePath);
}


void CakePopNode::addSprinkle(string sprinklePath, Vec2 worldPos)
{
    Rect r;
    r.origin = Vec2(-_cottonCandy1->getContentSize().width/2, -_cottonCandy1->getContentSize().height/2);
    r.size = _cottonCandy1->getContentSize()+Size(0,50);
    if (!r.containsPoint(_eatNode->convertToNodeSpaceAR(worldPos)))
    {
        return;
    }
    
    auto sprinkle = Sprite::create(sprinklePath);
    _sprinkleNode->addChild(sprinkle,10);
    sprinkle->setScale(0);
    sprinkle->setPosition(_eatNode->convertToNodeSpaceAR(worldPos));
    
    float scale = rand()%4*0.1 + 0.6;
    sprinkle->runAction(EaseBackOut::create(ScaleTo::create(0.3f, scale)));
    sprinkle->setRotation(rand()%360);
}

void CakePopNode::updateStick(string stickPath)
{
    _stick->setTexture(stickPath);
}

Rect CakePopNode::getCottonRect()
{
    Rect r;
    r.origin = convertToWorldSpace(Vec2(-_cottonCandy1->getContentSize().width/2, -_cottonCandy1->getContentSize().width/2+137));
    r.size = _cottonCandy1->getContentSize();
    
    return r;
}

void CakePopNode::saveImage(const std::function<void(RenderTexture*, const std::string&)>& callback)
{
    auto renderTexture1 = RenderTexture::create(getContentSize().width+80, getContentSize().height+110, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    _eatNode->setPosition(getContentSize()/2+Size(80/2,110/2));
    _stick->setVisible(false);
    renderTexture1->begin();
    _eatNode->visit();
    renderTexture1->end();
    _director->getRenderer()->render();
    renderTexture1->saveToFile(shareNameCottonCandy, true, callback);
    _eatNode->setPosition(Vec2(0, 0));
    _stick->setVisible(true);
    //    _director->getRenderer()->render();
}

void CakePopNode::showShare(bool canEat)
{
    auto path = FileUtils::getInstance()->getWritablePath()+ shareNameCottonCandy;
    if (!FileUtils::getInstance()->isFileExist(path))
        return;
    if (!_eatSprite)
    {
        _eatSprite = Sprite::create();
        _eatNode->addChild(_eatSprite);
//        _eatSprite->setPosition(0, 100);
    }
    _cottonCandy1->setVisible(false);
    _eatSprite->setTexture(path);
    if (_director->getTextureCache()->getTextureForKey("paint_cakepop"))
    {
        _director->getTextureCache()->removeTextureForKey("paint_cakepop");
    }
    Image *img = new Image();
    if (img->initWithImageFile(path))
    {
        _eatSprite->setTexture(_director->getTextureCache()->addImage(img, "paint_cakepop"));
        img->autorelease();
    }
    
    if (canEat)
    {
        this->registEat(_eatSprite);
    }
    
}



