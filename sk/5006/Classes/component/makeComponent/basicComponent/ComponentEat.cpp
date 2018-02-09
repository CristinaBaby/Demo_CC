//
//  ComponentEat.cpp
//  BHUG1062
//
//  Created by maxiang on 4/22/16.
//
//

#include "ComponentEat.h"

ComponentEat::~ComponentEat()
{
    CC_SAFE_RELEASE_NULL(_eatShapeSprite);
    
    for (auto sp : _eatShapeVec){
        sp->release();
    }
}

bool ComponentEat::init()
{
    if (!ComponentBase::init())
        return false;
    
    return true;
}

void ComponentEat::onAdd()
{
    ComponentBase::onAdd();
    
}

void ComponentEat::start()
{
    CCASSERT(_owner, "* ComponentEat: 请将组件添加到一个节点上! *");
    CCASSERT(_owner->getParent(), "* ComponentEat: 请先将吃的精灵添加到视图上! *");

    ComponentBase::start();
    
    if (_clippingNode)
        return;
    
    _clippingNode = ClippingNode::create();
    
    auto parent = _owner->getParent();
    _owner->retain();
    _owner->removeFromParent();
    
    _clippingNode->setPosition(_owner->getPosition());
    parent->addChild(_clippingNode, _owner->getLocalZOrder());
    
    _stencilNode = Node::create();
    _stencilNode->setPosition(Vec2::ZERO);
    _clippingNode->setStencil(_stencilNode);
    _clippingNode->setInverted(true);
    
    _clippingNode->setAlphaThreshold(0.01);
    
    _owner->setPosition(Vec2::ZERO);
    _clippingNode->addChild(_owner);
    
    addTouchEventListener();
}

void ComponentEat::setEatShape(Sprite *shape)
{
    CCASSERT(shape, "* ComponentEat: 形状遮罩不能为空！ *");
    
    CC_SAFE_RELEASE_NULL(_eatShapeSprite);
    _eatShapeSprite = shape;
    _eatShapeSprite->retain();
}

void ComponentEat::reset()
{
    if (_stencilNode)
        _stencilNode->removeAllChildren();
    if (_clippingNode)
        _clippingNode->removeAllChildren();
    
    _clippingNode->addChild(_owner);
    _enabled = true;
    _isFinishEat = false;
}

float ComponentEat::checkEatFinshed()
{
    //将图片缩小, 可减少需要检测的图片像素, 提高性能
    float scale = 0.1;
    _clippingNode->setScale(scale);
    
    //只需要截出盘子部分可以吃的区域
    float renderWidth = _owner->getContentSize().width * scale;
    float renderHeight = _owner->getContentSize().height * scale;
    auto renderTexture = RenderTexture::create(renderWidth, renderHeight, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    
    auto ori = _clippingNode->getPosition();
    _clippingNode->setPosition(Vec2(renderWidth/2, renderHeight/2));

    renderTexture->begin();
    _clippingNode->cocos2d::Node::visit();
    renderTexture->end();
    Director::getInstance()->getRenderer()->render();
    _clippingNode->setScale(1.0);
    _clippingNode->setPosition(ori);
    
    auto image = renderTexture->newImage();
    unsigned char *data = image->getData();
    
    int width = image->getWidth();
    int height = image->getHeight();
    
    int pixOff = 0;
    int notTransparentPixelCount = 0;
    for(int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int alpha = (unsigned char)data[pixOff + 3];
            if (alpha >= _alphaThreshold)
                ++notTransparentPixelCount;
            
            pixOff += 4;
        }
    }
    
    float eatRate = (float)notTransparentPixelCount/(width*height);
    log("%f", eatRate);
    
    image->release();
    
    return eatRate;
}

void ComponentEat::addTouchEventListener()
{
    if (_eventListenerTouchOneByOne || !getOwner())
        return;
    
    _eventListenerTouchOneByOne = EventListenerTouchOneByOne::create();
    _eventListenerTouchOneByOne->onTouchBegan = CC_CALLBACK_2(ComponentEat::onTouchBegan, this);
    _eventListenerTouchOneByOne->onTouchMoved = CC_CALLBACK_2(ComponentEat::onTouchMoved, this);
    _eventListenerTouchOneByOne->onTouchEnded = CC_CALLBACK_2(ComponentEat::onTouchEnded, this);
    _eventListenerTouchOneByOne->onTouchCancelled = CC_CALLBACK_2(ComponentEat::onTouchCancelled, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_eventListenerTouchOneByOne, getOwner());
    
    _eventListenerTouchOneByOne->setSwallowTouches(false);
}

void ComponentEat::removeTouchEventListener()
{
    if (_eventListenerTouchOneByOne)
    {
        Director::getInstance()->getEventDispatcher()->removeEventListener(_eventListenerTouchOneByOne);
        _eventListenerTouchOneByOne = nullptr;
    }
}

bool ComponentEat::onTouchBegan(Touch* touch, Event* event)
{
    if (!_owner ||
        !_enabled ||
        !_clippingNode)
        return false;
    auto node = getOwner()->getParent();
    
    auto point = node->convertToNodeSpace(touch->getLocation());
    auto boundingbox = getOwner()->getBoundingBox();
    if (boundingbox.containsPoint(point))
    {
        if (eatingCallback)
            eatingCallback(touch);

        Sprite* eatShape = nullptr;
        if (!_isMultiShape)
        {
            eatShape = Sprite::createWithTexture(_eatShapeSprite->getTexture());
        }
        else
        {
            int indx = rand()%_eatShapeVec.size();
            eatShape = Sprite::createWithTexture(_eatShapeVec.at(indx)->getTexture());
        }
        CCASSERT(eatShape, "* ComponentEat: 形状遮罩不能为空! *");
        
        _stencilNode->addChild(eatShape);
        eatShape->setPosition(_clippingNode->convertToNodeSpace(touch->getLocation()));
        
        auto p = ParticleSystemQuad::create("particle/biting.plist");
        p->setPosition(Director::getInstance()->getRunningScene()->convertToNodeSpace(touch->getLocation()));
        
        Director::getInstance()->getRunningScene()->addChild(p,9999);

        float eatRate = checkEatFinshed();
        if (eatRate < _eatRate)
        {
//            _enabled = false;
            if (finishEatCallback && !_isFinishEat)
            {
                finishEatCallback();
            }
            _isFinishEat = true;
        }
        
        return false;
    }
    return false;
}

void ComponentEat::setEatShapeVec(vector<Sprite*> eatVec)
{
    _eatShapeVec = eatVec;
    
    for (auto sp :_eatShapeVec){
        sp->retain();
    }
    
    if (_eatShapeVec.size() > 1)
        _isMultiShape = true;
}

void ComponentEat::onTouchMoved(Touch* touch, Event* event)
{
    
}

void ComponentEat::onTouchEnded(Touch* touch, Event* event)
{
   
}

void ComponentEat::onTouchCancelled(Touch* touch, Event* event)
{
    
}

