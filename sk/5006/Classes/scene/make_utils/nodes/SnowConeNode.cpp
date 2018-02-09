//
//  SnowConeNode.cpp
//  BHUG1066
//
//  Created by maxiang on 6/27/16.
//
//

#include "SnowConeNode.h"

SnowConeNode::~SnowConeNode()
{
    
}

SnowConeNode* SnowConeNode::create(int index)
{
    SnowConeNode *pRet = new SnowConeNode();
    if (pRet && pRet->init(index))
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

bool SnowConeNode::init(int index)
{
    if (!MakeBaseNode::init())
        return false;
    
    auto fileBottom = _cupPath + "cones" + std::to_string(index) + "_0.png";
    auto cupBottom = Sprite::create(fileBottom);
    
    auto fileTop = _cupPath + "cones" + std::to_string(index) + "_1.png";
    auto cupTop = Sprite::create(fileTop);
    
    addChild(cupBottom, 1, 1);
    addChild(cupTop, 5, 2);

    _mainContent = cupBottom;
    return true;
}

void SnowConeNode::setShape(int index)
{
    if (!_iceShapeNode)
    {
        _iceShapeNode = Node::create();
        _iceShapeNode->setPosition(Vec2(10.0, 230.0));
        addChild(_iceShapeNode, 2);
        
        //由3个部分组成
        auto iceShapeFile = _shapePath + "ice_" + std::to_string(index) + ".png";
        auto addClipping = [=](int tag)
        {
            auto clipping = ClippingNode::create();
            clipping->setStencil(Sprite::create(_shapePath + "cut_shadow" + std::to_string(tag) + ".png"));
            auto child = Sprite::create(iceShapeFile);
            clipping->addChild(child);
            child->setTag(123);
            clipping->setCascadeOpacityEnabled(true);
            clipping->setOpacity(0);
            clipping->setTag(tag);
            clipping->setAlphaThreshold(0.2);
            
//            if (tag == 1)
//                clipping->setPosition(Vec2(-2.0, 2.0));
//            if (tag == 2)
//                clipping->setPosition(Vec2(-4.0, 4.0));

            _iceShapeNode->addChild(clipping);
        };
        addClipping(0);
        addClipping(1);
        addClipping(2);
    }
}

void SnowConeNode::setCupVisible(bool visible)
{
    getChildByTag(1)->setVisible(visible);
    getChildByTag(2)->setVisible(visible);
}

void SnowConeNode::showPartAnimation(int partindex)
{
    for (auto child : _iceShapeNode->getChildren())
    {
        getActionManager()->removeAllActionsFromTarget(child);
    }
    
    auto clipping = _iceShapeNode->getChildByTag(partindex);
    clipping->runAction(Repeat::create(Sequence::create(FadeOut::create(0.3), FadeIn::create(0.3), NULL), 3));
}

void SnowConeNode::showPart(int partindex,
                            const std::string& juiceFile,
                            bool animation)
{
    if (_juiceShapeSprite)
    {
        _juiceShapeSprite->removeFromParent();
        _juiceShapeSprite = nullptr;
    }

    for (auto child : _iceShapeNode->getChildren())
    {
        getActionManager()->removeAllActionsFromTarget(child);
    }
    
    auto clipping = ClippingNode::create();
    clipping->setStencil(Sprite::create(_shapePath + "cut_shadow" + std::to_string(partindex) + ".png"));
    auto child = Sprite::create(juiceFile);
    clipping->addChild(child);
    clipping->setCascadeOpacityEnabled(true);
    clipping->setOpacity(0);
    clipping->setAlphaThreshold(0.2);
    _iceShapeNode->addChild(clipping, 10);

    if (animation)
    {
        clipping->runAction(Sequence::create(FadeIn::create(1.5), CallFunc::create([=](){
            
            auto oldclipping = _iceShapeNode->getChildByTag(partindex);
            oldclipping->removeChildByTag(123);
            oldclipping->addChild(Sprite::create(juiceFile));
            oldclipping->setOpacity(255);
            
            clipping->removeFromParent();
        }), NULL));
    }
    else
    {
        auto oldclipping = _iceShapeNode->getChildByTag(partindex);
        oldclipping->removeChildByTag(123);
        oldclipping->addChild(Sprite::create(juiceFile));
        oldclipping->setOpacity(255);
        
        clipping->removeFromParent();
    }
}

void SnowConeNode::showJuiceShape(const std::string& file)
{
    if (!_juiceShapeSprite)
    {
        _juiceShapeSprite = Sprite::create();
        _juiceShapeSprite->setPosition(Vec2(10.0, 230.0));
        addChild(_juiceShapeSprite, 1);
    }
    
    _juiceShapeSprite->setTexture(file);
}

void SnowConeNode::setShapeOpacity(int opacity)
{
    for (auto child : _iceShapeNode->getChildren())
    {
        child->setOpacity(opacity);
    }
}

void SnowConeNode::setIceClipping(const std::string& maskFile, const Vec2& position)
{
    if (!_iceClippingNode)
    {
        _iceClippingNode = ClippingNode::create();
        auto stencil = Sprite::create(maskFile);
        _iceClippingNode->setStencil(stencil);
        _iceClippingNode->setAlphaThreshold(0.9);
        _iceClippingNode->setInverted(false);
        _iceClippingNode->setPosition(position);
        _iceClippingNode->setCascadeOpacityEnabled(true);
        addChild(_iceClippingNode, 3);
    }
}

void SnowConeNode::setIce(const std::string& iceFile, const Vec2& position)
{
    static int index = 1;
    auto ice = Sprite::create(iceFile);
    ice->setPosition(position);
    _iceClippingNode->addChild(ice, index++);
}

void SnowConeNode::showIce(const Vec2& position, bool animation)
{
    
}








