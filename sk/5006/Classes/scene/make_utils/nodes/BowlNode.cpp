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
#include "SoundConfig.h"
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
    
    _top = Sprite::create();
    addChild(_top, 99);

    if (!top.empty())
    {
        _top->setTexture(top);
    }
    
    _mainContent = _bottom;
    return true;
}

void BowlNode::adjustPostion(Vec2 pos)
{
    for (auto node : this->getChildren())
    {
        node->setPosition(node->getPosition()+pos);
    }

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
    
    compMove->touchBeganCall = [=](Touch* touch)
    {
        if (_touchBeginCall)
        {
            _touchBeginCall();
        }
    };
    compMove->touchEndedCall = [=](Touch* touch)
    {
        if (_touchEndedCall)
        {
            _touchEndedCall();
        }
    };
    
    compMove->componentTouchMoveToTargetDoneCall = [=]()
    {
        if (showCallback)
            showCallback();
    };
}

void BowlNode::beginPour(float rotateTo, const std::function<void()>& callback,bool needClicked)
{
    _rotateTo = rotateTo;
    _pourCallback = callback;
    
    Device::setAccelerometerEnabled(true);
    auto accelerateComp = ComponentAccelerateRotate::create(_rotateTo);
    this->addComponent(accelerateComp);
    accelerateComp->setStartRotation(0.0);
    accelerateComp->start();
    
    auto touchComp = ComponentTouchMove::create();
    touchComp->setIsCanMove(false);
    this->addComponent(touchComp);
    touchComp->touchEndedCall = [=](Touch* touch)
    {
        touchComp->setEnabled(false);
        accelerateRotateDone();
    };
    accelerateComp->componentAccelerateRotateDoneCall = [=]()
    {
        touchComp->setEnabled(false);
        accelerateRotateDone();
    };
    
    if (!needClicked)
    {
        touchComp->setEnabled(false);
        accelerateRotateDone();
    
    }
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
            contentSprite->runAction(Sequence::create(FadeIn::create(2.2f), CallFunc::create([=](){
                
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


void BowlNode::initMaskNode(const std::string& file,Vec2 pos)
{
    _mask = Sprite::create(file);
    _maskNode = ClippingNode::create(_mask);
    _maskNode->setAlphaThreshold(0);
    _maskNode->setPosition(pos);
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

void BowlNode::showContentInMask(const std::string& name,float duration)
{
    auto contentSprite = _maskNode->getChildByName(name);
    if (!contentSprite)
        return;

    auto pos = contentSprite->getPosition();
    auto size = contentSprite->getContentSize();
    contentSprite->setPosition(pos+Vec2(0, -size.height));
    contentSprite->runAction(MoveTo::create(duration, pos));
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
//    if (!bottom.empty())
        _bottom->setTexture(bottom);
//    if (!cover.empty())
        _top->setTexture(cover);
    if (!mask.empty())
        _mask->setTexture(mask);

}

void BowlNode::showHeart(Vec2 heartPos,const std::function<void()>& showCallback)
{
    for (int i = 0; i<3; i++)
    {
        auto heart = Sprite::create("ui/heart.png");
        this->addChild(heart,999);
        auto pos = heartPos+Vec2(-90+90*i, 50);
        
        heart->setPosition(heartPos);
        heart->setScale(0);
        auto sp = Spawn::create(EaseBackOut::create(ScaleTo::create(0.4f, 1.f)),
                                ActionHelper::moveToPostionByBezier(heartPos, pos, 0.8f,Vec2(90,120)),
                                NULL);
        
        heart->runAction(Sequence::create(sp,
                                          CallFunc::create([=]
        {
            if (i==2 && showCallback)
            {
                showCallback();
            }
        }),
                                          RemoveSelf::create(),
                                          nullptr));
    }
    
    kAudioUtil->playEffect(kSoundAddPop);
    kSoundInstance->playRandomStepFinishVoice();
    this->runAction(Sequence::create(ScaleTo::create(0.2f, 0.9f),
                                     EaseBackOut::create(ScaleTo::create(0.2f, 1.f)),
                                     NULL));
}

