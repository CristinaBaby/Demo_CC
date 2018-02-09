//
//  IceContainerNode.cpp
//  BHUG1066
//
//  Created by maxiang on 6/24/16.
//
//

#include "IceContainerNode.h"
#include "component/makeComponent/combinedComponent/ComponentTouchMoveToTarget.h"
#include "component/actions/ActionHelper.h"
#include "component/makeComponent/basicComponent/ComponentAccelerateRotate.h"
#include "component/makeComponent/basicComponent/ComponentTouchMove.h"
#include "TipsUIController.h"
#include "ESDataManager.h"
#include "ComponetFade.h"

IceContainerNode::~IceContainerNode()
{
    
}

bool IceContainerNode::init()
{
    if (!MakeBaseNode::init())
        return false;

    auto food = kESDataManager->getSodaFood();
    string boxPath = StringUtils::format("res/rainbow_slushy/ice/mould_%d.png",food.iceIndex);
    
    auto container = Sprite::create(boxPath);
    addChild(container, 1);

    vector<vector<Vec2>> tempVec;
    vector<Vec2> vec0 =
    {
        Vec2(-132,62),
        Vec2(-2,26),
        Vec2(120,58),
        Vec2(126,-16),
        Vec2(-140,-11),
    };
    tempVec.push_back(vec0);
    vector<Vec2> vec1 =
    {
        Vec2(-119,64),
        Vec2(4,25),
        Vec2(123,64),
        Vec2(135,-13),
        Vec2(-127,-10),
    };
    tempVec.push_back(vec1);
    vector<Vec2> vec2 =
    {
        Vec2(-125,64),
        Vec2(0,22),
        Vec2(125,64),
        Vec2(135,-13),
        Vec2(-135,-13),
    };
    tempVec.push_back(vec2);
    vector<Vec2> vec3 =
    {
        Vec2(-125,64),
        Vec2(4,28),
        Vec2(127,64),
        Vec2(140,-11),
        Vec2(-133,-12),
    };
    tempVec.push_back(vec3);
    vector<Vec2> vec4 =
    {
        Vec2(-125,64),
        Vec2(4,28),
        Vec2(127,64),
        Vec2(144,-14),
        Vec2(-140,-12),
    };
    tempVec.push_back(vec4);
    vector<Vec2> vec5 =
    {
        Vec2(-134,64),
        Vec2(-2,25),
        Vec2(132,64),
        Vec2(146,-14),
        Vec2(-143,-12),
    };
    tempVec.push_back(vec5);

    _icePosVec = tempVec.at(food.iceIndex-1);
    
    _mainContent = container;
    
    return true;
}

void IceContainerNode::setPourWorldPoint(const Vec2& worldPoint, const std::function<void()>& showCallback)
{
    _originalPosition = getPosition();
    _showContentCallback = showCallback;

    auto compMove = ComponentTouchMoveToTarget::create(worldPoint);
    compMove->setMaxDistanceForMoveToTarget(_moveToTargetTriggerDistance);
    addComponent(compMove);
    compMove->start();
 
    compMove->componentTouchMoveToTargetDoneCall = [=]()
    {
        auto tips = TipsUIController::getInstance()->showAccelerateTip(this->getParent(), Vec2(this->getPosition().x - 200.0, this->getPosition().y),false);
        tips->setZOrder(100);
        
        Device::setAccelerometerEnabled(true);
        auto accelerateComp = ComponentAccelerateRotate::create(-16.0);
        this->addComponent(accelerateComp);
        accelerateComp->setStartRotation(0.0);
        accelerateComp->start();
        accelerateComp->componentAccelerateRotateDoneCall = [=]()
        {
            TipsUIController::getInstance()->removeAllTips();
            accelerateRotateDone();
        };
        
        auto touchComp = ComponentTouchMove::create();
        touchComp->setIsCanMove(false);
        this->addComponent(touchComp);
        touchComp->touchEndedCall = [=](Touch* touch)
        {
            TipsUIController::getInstance()->removeAllTips();
            touchComp->setEnabled(false);
            accelerateRotateDone();
        };
    };
}

void IceContainerNode::accelerateRotateDone()
{
    Device::setAccelerometerEnabled(false);
    this->getActionManager()->removeAllActionsFromTarget(this);
    this->runAction(Sequence::create(RotateTo::create(0.5, -16.0), CallFunc::create([=](){
        
        int index = 0;
        for (auto ice : _iceVector)
        {
            ice->runAction(Sequence::create(DelayTime::create(0.6f * index), Spawn::create(MoveBy::create(0.2, Vec2(-100.0, 0.0)), FadeOut::create(0.2), NULL), NULL));
            index++;
        }
        
        if (_showContentCallback)
            _showContentCallback();
    }), NULL));
}

void IceContainerNode::pourWaterIn(int index)
{
    float duration = 2.4f;
    auto food = kESDataManager->getSodaFood();
    string iceLiquidPath = StringUtils::format("res/rainbow_slushy/ice/water_%d.png",food.iceIndex);

    auto ice = Sprite::create(iceLiquidPath);
    addChild(ice, 2);
    ice->setPosition(_icePosVec.at(index));
    if (food.iceIndex == 4)
    {
        ice->setOpacity(0);
        ice->runAction(FadeIn::create(duration));
    }
    else
    {
        ice->setScale(0.f);
        ice->runAction(ScaleTo::create(duration, 1.f));
    }
    _iceVector.push_back(ice);
}

void IceContainerNode::freeze(float duration)
{
    auto food = kESDataManager->getSodaFood();
    string iceSolidPath = StringUtils::format("res/rainbow_slushy/ice/mould_ice%d.png",food.iceIndex);
    string iceLiquidPath = StringUtils::format("res/rainbow_slushy/ice/water_%d.png",food.iceIndex);

    for (auto ice :_iceVector)
    {
        auto cFade = ComponetFade::create(iceLiquidPath, iceSolidPath);
        ice->addComponent(cFade);
        cFade->setFadeDuration(duration);
        cFade->start();
        cFade->setName("cFade");
        _fadeVec.push_back(cFade);
    }
}

void IceContainerNode::resetFreeze()
{
    auto food = kESDataManager->getSodaFood();
    string iceLiquidPath = StringUtils::format("res/rainbow_slushy/ice/water_%d.png",food.iceIndex);
    
    for (auto cFade : _fadeVec)
    {
        cFade->stop();
    }
    _fadeVec.clear();
    
    for (auto ice :_iceVector)
    {
        ice->stopAllActions();
        ice->setOpacity(255);
        ice->setTexture(iceLiquidPath);
        ice->removeAllComponents();
    }

}

void IceContainerNode::showIce()
{
    auto food = kESDataManager->getSodaFood();
    string iceLiquidPath = StringUtils::format("res/rainbow_slushy/ice/mould_ice%d.png",food.iceIndex);
    
    for (int index=0; index<_icePosVec.size(); index++)
    {
        auto ice = Sprite::create(iceLiquidPath);
        addChild(ice, 2);
        ice->setPosition(_icePosVec.at(index));
        _iceVector.push_back(ice);
    }
}

void IceContainerNode::showRotateCallback(float rotate, const std::function<void()>& callback)
{
    
    _showContentCallback = callback;

    auto tips = TipsUIController::getInstance()->showAccelerateTip(this->getParent(), Vec2(this->getPosition().x - 200.0, this->getPosition().y),false);
    tips->setZOrder(100);
    
    Device::setAccelerometerEnabled(true);
    auto accelerateComp = ComponentAccelerateRotate::create(rotate);
    this->addComponent(accelerateComp);
    accelerateComp->setStartRotation(0.0);
    accelerateComp->start();
    accelerateComp->componentAccelerateRotateDoneCall = [=]()
    {
        TipsUIController::getInstance()->removeAllTips();
        accelerateRotateDone();
    };
    
    auto touchComp = ComponentTouchMove::create();
    touchComp->setIsCanMove(false);
    this->addComponent(touchComp);
    touchComp->touchEndedCall = [=](Touch* touch)
    {
        TipsUIController::getInstance()->removeAllTips();
        touchComp->setEnabled(false);
        accelerateRotateDone();
    };

    
}

string IceContainerNode::getIcePath()
{
    auto food = kESDataManager->getSodaFood();
    string iceLiquidPath = StringUtils::format("res/rainbow_slushy/ice/blender_ice%d.png",food.iceIndex);

    return iceLiquidPath;
}




