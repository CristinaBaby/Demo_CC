//
//  ComponentMultiLayer.cpp
//  BHUG1062
//
//  Created by maxiang on 3/23/16.
//
//

#include "ComponentMultiLayer.h"

ComponentMultiLayer::ComponentMultiLayer():
_layerMaskSprite(nullptr),
_enableAnimation(true),
_layerClippingNode(nullptr),
_animationSpeed(50.0),
_currentLayerHeight(0.0),
_isAppending(false)
{
    
}

ComponentMultiLayer::~ComponentMultiLayer()
{
    if (_layerClippingNode)
        _layerClippingNode->removeFromParent();
    CC_SAFE_RELEASE_NULL(_layerMaskSprite);
    CC_SAFE_RELEASE_NULL(_layerClippingNode);
}

ComponentMultiLayer *ComponentMultiLayer::create(Sprite *maskSprite)
{
    auto ret = new (std::nothrow) ComponentMultiLayer();
    if (ret)
    {
        ret->init(maskSprite);
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ComponentMultiLayer::init(Sprite *maskSprite)
{
    CCASSERT(maskSprite, "* ComponentMultiLayer: 遮罩不能为空! *");
    
    CC_SAFE_RELEASE_NULL(_layerMaskSprite);
    _layerMaskSprite = maskSprite;
    _layerMaskSprite->retain();
    
    if (!_layerClippingNode)
    {
        _layerClippingNode = ClippingNode::create();
        _layerClippingNode->retain();
    }

    return true;
}

void ComponentMultiLayer::onAdd()
{
    ComponentBase::onAdd();
    
    if (!_owner)
        return;
    
    if (!_layerClippingNode->getParent())
    {
        _owner->addChild(_layerClippingNode);
        _layerClippingNode->setStencil(_layerMaskSprite);
        _layerClippingNode->setAlphaThreshold(0.9);
    }
}

void ComponentMultiLayer::setClippingZorder(const int zorder)
{
   if (!_layerClippingNode)
       return;
    
    _layerClippingNode->setLocalZOrder(zorder);
}

void ComponentMultiLayer::setPosition(const Vec2& position)
{
    if (!_layerClippingNode)
        return;
    
    _layerClippingNode->setPosition(position);
}

void ComponentMultiLayer::appendLayer(Sprite *layerSprite,
                                      const int zOrder,
                                      const float height,
                                      const std::function<void()>& finishCallback,
                                      const Vec2& positionOffset)
{
    if (!_owner || _isAppending)
        return;
    
    CCASSERT(height <= layerSprite->getContentSize().height*fabsf(layerSprite->getScaleY()), "* ComponentMultiLayer: 设置高度大于了精灵高度 *");
    
    float positionY = -(layerSprite->getContentSize().height * fabsf(layerSprite->getScaleY())/2 - height + _layerMaskSprite->getContentSize().height/2) + _currentLayerHeight;
    layerSprite->setPosition(Vec2(0.0, -(_layerMaskSprite->getContentSize().height/2 + layerSprite->getContentSize().height * fabsf(layerSprite->getScaleY())/2) + _currentLayerHeight));
    
    layerSprite->setPosition(layerSprite->getPosition() + positionOffset);
    
    _layerClippingNode->addChild(layerSprite, zOrder);
    _currentLayerHeight += height;
    
    if (_enableAnimation)
    {
        _isAppending = true;
        
        auto duration = height / _animationSpeed;
        layerSprite->runAction(Sequence::create(MoveTo::create(duration, Vec2(positionOffset.x, positionY + positionOffset.y)), CallFunc::create([=](){
            
            _isAppending = false;
            if (finishCallback)
                finishCallback();
            
        }), NULL));
    }
    else
    {
        layerSprite->setPosition(Vec2(0.0, positionY));
        
        if (finishCallback)
            finishCallback();
    }
}
