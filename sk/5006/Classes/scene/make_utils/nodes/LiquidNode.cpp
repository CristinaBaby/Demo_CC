//
//  LiquidNode.cpp
//  BHUG1064
//
//  Created by maxiang on 5/26/16.
//
//

#include "LiquidNode.h"
#include "component/makeComponent/combinedComponent/ComponentTouchMoveToTarget.h"
#include "component/makeComponent/basicComponent/ComponentTouchMove.h"
#include "component/actions/ActionHelper.h"
#include "component/makeComponent/basicComponent/ComponentAccelerateRotate.h"

LiquidNode::LiquidNode():
_liquid(nullptr),
_cover(nullptr),
_mask(nullptr),
_inLiquid(nullptr),
_lid(nullptr),
_shadow(nullptr),
_rotateDegree(-45.f),
_duration(6.f)
{
    
}

LiquidNode::~LiquidNode()
{
    
}

LiquidNode* LiquidNode::create(const std::string& file,
                               const std::string& cover,
                               const std::string& mask,
                               const std::string& inLiquid,
                               const std::string& shadow,
                               const std::string& lid)
{
    LiquidNode *pRet = new LiquidNode();
    if (pRet && pRet->init(file,cover,mask,inLiquid,shadow,lid))
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

bool LiquidNode::init(const std::string& file,
                      const std::string& cover,
                      const std::string& mask,
                      const std::string& inLiquid,
                      const std::string& shadow,
                      const std::string& lid)
{
    if (!MakeBaseNode::init())
        return false;
    
    _liquid = Sprite::create(file);
    addChild(_liquid, 1);
    
    _mainContent = _liquid;
    
    if (!cover.empty())
    {
        _cover = Sprite::create(cover);
        addChild(_cover, 4);
    }
    if (!mask.empty())
    {
        _mask = Sprite::create(mask);
        auto cNode = ClippingNode::create(_mask);
        cNode->setAlphaThreshold(0);
        this->addChild(cNode,2);
        
        if (!inLiquid.empty())
        {
            _inLiquid = Sprite::create(inLiquid);
            cNode->addChild(_inLiquid, 3);
        }
    }
    
    if (!lid.empty())
    {
        _lid = Sprite::create(lid);
        addChild(_lid, 10);
    }
    if (!shadow.empty())
    {
        _shadow = Sprite::create(shadow);
        addChild(_shadow, -1);
    }


    return true;
}

void LiquidNode::setLiquidPos(const Vec2 &Pos)
{
    if (_inLiquid)
    {
        _inLiquid->setPosition(Pos);
    }

}


void LiquidNode::setPourWorldPoint(const Vec2& worldPoint, const std::function<void()>& showCallback)
{
    _originalPosition = getPosition();
    _showContentCallback = showCallback;
    
    auto compMove = ComponentTouchMoveToTarget::create(worldPoint);
    compMove->setMaxDistanceForMoveToTarget(_moveToTargetTriggerDistance);
    addComponent(compMove);
    compMove->start();
    
    compMove->touchBeganCall = [=](Touch* touch)
    {
        if (onTouchBegan)
            onTouchBegan();
        auto scaleCall = [=]
        {
            if (_shadow)
            {
                _shadow->runAction(ScaleTo::create(0.1, 0,1));
            }
            this->runAction(ScaleTo::create(0.3, 1.2));
        };
        
        if (_lid)
        {
            _lid->runAction(Sequence::create(MoveBy::create(0.4f, Vec2(0, 100)),
                                             CallFunc::create([=]
            {
                _lid->removeFromParentAndCleanup(true);_lid=nullptr;
                scaleCall();
            }),
                                             NULL));
        }
        else
        {
            scaleCall();
        }
    };
    
    compMove->touchEndedCall = [=](Touch* touch)
    {
        this->runAction(ScaleTo::create(0.3, 1.0));
    };
    
    compMove->componentTouchMoveBackDoneCall = [=]
    {
        if (_shadow)
        {
            _shadow->runAction(ScaleTo::create(0.1, 1));
        }
    };
    compMove->componentTouchMoveToTargetDoneCall = [=]()
    {
        if (onMoveToTargetCallback)
            onMoveToTargetCallback();
        
        Device::setAccelerometerEnabled(true);
        auto accelerateComp = ComponentAccelerateRotate::create(_rotateDegree);
        this->addComponent(accelerateComp);
        
        auto touchComp = ComponentTouchMove::create();
        touchComp->setIsCanMove(false);
        this->addComponent(touchComp);

        accelerateComp->setStartRotation(0.0);
        accelerateComp->start();
        accelerateComp->componentAccelerateRotateDoneCall = [=]()
        {
            touchComp->setEnabled(false);
            accelerateComp->pause();
            accelerateRotateDone();
        };
        
        touchComp->touchEndedCall = [=](Touch* touch)
        {
            touchComp->setEnabled(false);
            accelerateComp->pause();
            accelerateRotateDone();
        };
    };
}

void LiquidNode::accelerateRotateDone()
{
    Device::setAccelerometerEnabled(false);
    this->getActionManager()->removeAllActionsFromTarget(this);
    auto degree = _rotateDegree>0?110.f:-75.f;
    
    this->runAction(Sequence::create(RotateTo::create(0.5, degree), CallFunc::create([=](){
        
        if (_inLiquid)
        {
            _inLiquid->runAction(MoveBy::create(_duration, Vec2(0, -(_inLiquid->getContentSize().height*0.6f))));
        }
        
        if (_showContentCallback)
            _showContentCallback();
    }), NULL));
}

void LiquidNode::backToOriginal(const std::function<void()>& callback)
{
    this->runAction(Sequence::create(RotateTo::create(0.5, 0.0),
                                     EaseBackOut::create(MoveTo::create(0.3, _originalPosition)),
                                     CallFunc::create([=]()
    {
        if (_shadow)
        {
            _shadow->runAction(ScaleTo::create(0.1, 1));
        }
        if (callback)
            callback();
    }),
                                     NULL));
}

void LiquidNode::openLid()
{
    if (!_lid)
    {
        return;
    }

    
    _lid->runAction(Spawn::create(MoveBy::create(0.3f, Vec2(0, 100)),
                                  FadeOut::create(0.3f),
                                  NULL));
}


