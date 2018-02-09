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

IceContainerNode::~IceContainerNode()
{
    
}

bool IceContainerNode::init()
{
    if (!MakeBaseNode::init())
        return false;
    
    auto container = Sprite::create(CONTAINER);
    addChild(container, 1);
    
    for (int column = 0; column < 6; ++column)
    {
        for (int row = 0; row < 2; ++row)
        {
            float x = 75.0 + 68.0 * column - row * 3;
            float y = 142.0 - 43.0 * row;
            
            auto ice = Sprite::create(ICE_PIECE);
            ice->setPosition(Vec2(x, y));
            _iceVector.push_back(ice);
            container->addChild(ice, 1);
        }
    }
    
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
        auto tips = TipsUIController::getInstance()->showAccelerateTip(this->getParent(), Vec2(this->getPosition().x - 200.0, this->getPosition().y));
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
            ice->runAction(Sequence::create(DelayTime::create(0.2 * index), Spawn::create(MoveBy::create(0.2, Vec2(-100.0, 0.0)), FadeOut::create(0.2), NULL), NULL));
            index++;
        }
        
        if (_showContentCallback)
            _showContentCallback();
    }), NULL));
}


