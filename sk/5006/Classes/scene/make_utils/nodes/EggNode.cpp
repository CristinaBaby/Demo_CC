//
//  EggNode.cpp
//  BHUG1064
//
//  Created by maxiang on 5/26/16.
//
//

#include "EggNode.h"
#include "component/makeComponent/combinedComponent/ComponentTouchMoveToTarget.h"
#include "component/makeComponent/basicComponent/ComponentTouchMove.h"
#include "component/actions/ActionHelper.h"
#include "component/makeComponent/basicComponent/ComponentAccelerateRotate.h"

EggNode::EggNode()
{
    
}

EggNode::~EggNode()
{
    
}

EggNode* EggNode::create(const std::string& file)
{
    EggNode *pRet = new EggNode();
    if (pRet && pRet->init(file))
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

bool EggNode::init(const std::string& file)
{
    if (!MakeBaseNode::init())
        return false;
    
    _egg = Sprite::create(file);
    addChild(_egg, 1);
    
    _mainContent = _egg;
    return true;
}

void EggNode::setPourWorldPoint(const Vec2& worldPoint, const std::function<void()>& showCallback)
{
    _originalPosition = getPosition();
    _showContentCallback = showCallback;
    
    auto compMove = ComponentTouchMoveToTarget::create(worldPoint);
    compMove->setMaxDistanceForMoveToTarget(_moveToTargetTriggerDistance);
    compMove->setName("compMove");
    addComponent(compMove);
    compMove->start();
    
    compMove->touchBeganCall = [=](Touch* touch)
    {
        this->runAction(ScaleTo::create(0.3, 1.2));
    };
    
    compMove->touchEndedCall = [=](Touch* touch)
    {
        this->runAction(ScaleTo::create(0.3, 1.0));
    };
    
    compMove->componentTouchMoveToTargetDoneCall = [=]()
    {
        if (onMoveToTargetCallback)
            onMoveToTargetCallback();
        
        this->runAction(CallFunc::create([=](){
            compMove->pause();
//            compMove->setEnabled(false);
//            this->removeComponent(compMove);
            auto touchComp = ComponentTouchMove::create();
            touchComp->setIsCanMove(false);
            touchComp->setName("touchComp");
            this->addComponent(touchComp);
            touchComp->touchEndedCall = [=](Touch* touch)
            {
                log("====1");
                touchComp->setEnabled(false);
                if (showCallback)
                    showCallback();
                log("====2");

            };
        }));
    };
}

