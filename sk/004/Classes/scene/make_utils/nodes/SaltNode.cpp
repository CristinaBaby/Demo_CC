//
//  SaltNode.cpp
//  BHUG1064
//
//  Created by maxiang on 5/26/16.
//
//

#include "SaltNode.h"
#include "component/makeComponent/combinedComponent/ComponentTouchMoveToTarget.h"

SaltNode::SaltNode()
{
    
}

SaltNode::~SaltNode()
{
    
}

SaltNode* SaltNode::create(const std::string& file)
{
    SaltNode *pRet = new SaltNode();
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

bool SaltNode::init(const std::string& file)
{
    if (!MakeBaseNode::init())
        return false;
    
    _salt = Sprite::create(file);
    
    addChild(_salt, 1);
    
    _mainContent = _salt;
    return true;
}

void SaltNode::addContent(Node* content)
{
    _salt->addChild(content);

}


void SaltNode::setPourWorldPoint(const Vec2& worldPoint, const std::function<void()>& showCallback,const std::function<void()>& backCallback)
{
    _originalPosition = getPosition();
    
    auto compMove = ComponentTouchMoveToTarget::create(worldPoint);
    compMove->setMaxDistanceForMoveToTarget(_moveToTargetTriggerDistance);
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
        auto rotateDown = RotateBy::create(0.5, -120);
        auto moveDown1 = EaseSineIn::create(MoveBy::create(0.3, Vec2(0, -50)));
        auto moveUp2 = EaseSineOut::create(MoveBy::create(0.3, Vec2(0, 30)));
        auto moveDown2 = EaseSineIn::create(MoveBy::create(0.3, Vec2(0, -30)));
        auto roteteUp = rotateDown->reverse();
        this->runAction(Sequence::create(Spawn::create(rotateDown, moveDown1, nullptr), CallFunc::create([=](){
            
            if (showCallback)
                showCallback();
        }), moveUp2, moveDown2, DelayTime::create(0.3), roteteUp, MoveTo::create(0.3, _originalPosition), CallFunc::create([=](){
            
            if (backCallback)
                backCallback();
        }),nullptr));
    };
}
