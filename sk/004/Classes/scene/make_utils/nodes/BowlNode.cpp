//
//  BowlNode.cpp
//  BHUG1064
//
//  Created by maxiang on 5/25/16.
//
//

#include "BowlNode.h"
#include "component/makeComponent/basicComponent/ComponentAccelerateRotate.h"
#include "component/makeComponent/basicComponent/ComponentTouchMove.h"
#include "component/makeComponent/combinedComponent/ComponentTouchMoveToTarget.h"
#include "component/actions/ActionHelper.h"

BowlNode::~BowlNode()
{

}

BowlNode* BowlNode::create(const std::string& bottom, const std::string& top)
{
    BowlNode *pRet = new BowlNode();
    if (pRet && pRet->init(bottom, top))
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

bool BowlNode::init(const std::string& bottom, const std::string& top)
{
    if (!MakeBaseNode::init())
        return false;
    
    _bottom = Sprite::create(bottom);
    addChild(_bottom, 1);
    
    if (!top.empty())
    {
        _top = Sprite::create(top);
        addChild(_top, 99);
    }
    
    _mainContent = _bottom;
    return true;
}

Node* BowlNode::addContent(const std::string& file,
                          const std::string& name,
                          const int zorder,
                          const Vec2& position)
{
    auto contentSprite = Sprite::create(file);
    contentSprite->setPosition(position);
    contentSprite->setName(name);
    contentSprite->setOpacity(0);
    addChild(contentSprite, zorder);
    return contentSprite;
}

void BowlNode::addContent(Node *content,
                          const std::string& name,
                          const int zorder,
                          const Vec2& position)
{
    content->setPosition(position);
    content->setName(name);
    content->setOpacity(0);
    addChild(content, zorder);
}

void BowlNode::setPourWorldPoint(const Vec2& worldPoint, const std::function<void()>& showCallback)
{
    _originalPosition = getPosition();
    
    auto compMove = ComponentTouchMoveToTarget::create(worldPoint);
    compMove->setMaxDistanceForMoveToTarget(_moveToTargetTriggerDistance);
    addComponent(compMove);
    compMove->start();
    
    compMove->componentTouchMoveToTargetDoneCall = [=]()
    {
        if (showCallback)
            showCallback();
    };
}

void BowlNode::beginPour(float rotateTo, const std::function<void()>& callback)
{
    _rotateTo = rotateTo;
    _pourCallback = callback;
    
    Device::setAccelerometerEnabled(true);
    auto accelerateComp = ComponentAccelerateRotate::create(_rotateTo);
    this->addComponent(accelerateComp);
    accelerateComp->setStartRotation(0.0);
    accelerateComp->start();
    accelerateComp->componentAccelerateRotateDoneCall = [=]()
    {
        accelerateRotateDone();
    };
    
    auto touchComp = ComponentTouchMove::create();
    touchComp->setIsCanMove(false);
    this->addComponent(touchComp);
    touchComp->touchEndedCall = [=](Touch* touch)
    {
        touchComp->setEnabled(false);
        accelerateRotateDone();
    };
}

void BowlNode::accelerateRotateDone()
{
    Device::setAccelerometerEnabled(false);
    this->getActionManager()->removeAllActionsFromTarget(this);
    this->runAction(Sequence::create(RotateTo::create(0.5, _rotateTo), CallFunc::create([=](){
        
        if (_pourCallback)
            _pourCallback();
    }), NULL));
}

void BowlNode::endPour()
{
    
}

void BowlNode::setContentVisible(const std::string& name,
                                 const std::function<void()>& callback,
                                 bool visible,
                                 bool animation)
{
    auto contentSprite = getChildByName(name);
    if (!contentSprite)
        return;
    
    if (visible)
    {
        if (animation)
            contentSprite->runAction(Sequence::create(FadeIn::create(1.5), CallFunc::create([=](){
                
                if (callback)
                    callback();
            }), NULL));
        else
        {
            contentSprite->setOpacity(255);
            if (callback)
                callback();
        }
    }
    else
    {
        if (animation)
            contentSprite->runAction(Sequence::create(FadeOut::create(0.3), CallFunc::create([=](){
                
                if (callback)
                    callback();
            }), NULL));
        else
        {
            contentSprite->setOpacity(0);
            if (callback)
                callback();
        }
    }
}

Rect BowlNode::getBowlRect()
{
    return _bottom->getBoundingBox();

}


void BowlNode::initMaskNode(const std::string& file)
{
    _mask = Sprite::create(file);
    _maskNode = ClippingNode::create(_mask);
    _maskNode->setAlphaThreshold(0);
    this->addChild(_maskNode,2);
}

void BowlNode::addContentToMask(const std::string& file,
                                const std::string& name,
                                const int zorder,
                                const Vec2& position)
{
    auto contentSprite = Sprite::create(file);
    this->addContentToMask(contentSprite, name, zorder,position);
}

void BowlNode::addContentToMask(Node *content,
                                const std::string& name,
                                const int zorder,
                                const Vec2& position)
{
    content->setPosition(position);
    content->setName(name);
    _maskNode->addChild(content, zorder);
}

void BowlNode::showContentInMask(const std::string& name)
{
    auto contentSprite = _maskNode->getChildByName(name);
    if (!contentSprite)
        return;

    auto pos = contentSprite->getPosition();
    auto size = contentSprite->getContentSize();
    contentSprite->setPosition(pos+Vec2(0, -size.height));
    contentSprite->runAction(MoveTo::create(1.3, pos));
}


void BowlNode::setContentInMaskVisible(const std::string& name,
                                 const std::function<void()>& callback,
                                 bool visible,
                                 bool animation)
{
    auto contentSprite = _maskNode->getChildByName(name);
    if (!contentSprite)
        return;
    
    if (visible)
    {
        if (animation)
            contentSprite->runAction(Sequence::create(FadeIn::create(0.3), CallFunc::create([=](){
                
                if (callback)
                    callback();
            }), NULL));
        else
        {
            contentSprite->setOpacity(255);
            if (callback)
                callback();
        }
    }
    else
    {
        if (animation)
            contentSprite->runAction(Sequence::create(FadeOut::create(0.3), CallFunc::create([=](){
                
                if (callback)
                    callback();
            }), NULL));
        else
        {
            contentSprite->setOpacity(0);
            if (callback)
                callback();
        }
    }
}

void BowlNode::updateBowlConfig(
                      const string &bottom,
                      const string &cover,
                      const string &mask
                                )
{
    _bottom->setTexture(bottom);
    _top->setTexture(cover);
    if (!mask.empty())
        _mask->setTexture(mask);

}
