//
//  CombActions.cpp
//
//  Created by tanshoumei on 15/5/18.
//
//

#include "component/actions/CombActions.h"
NS_CombAction_BEGIN

CombActionMoveBase::CombActionMoveBase()
{
}

bool CombActionMoveBase::init(cocos2d::Node *aTargetNode, const cocos2d::Vec2 &atTarget, ActionDirection aDirection)
{
    _targetPosition = atTarget;
    _direction = aDirection;
    setTargetNode(aTargetNode);
    
    return true;
}

MoveTo* CombActionMoveBase::_getMoveAction()
{
    return MoveTo::create(_actionDuration, _targetPosition);
}

CombActionThrowBackIn* CombActionThrowBackIn::create(cocos2d::Node *aTargetNode, const cocos2d::Vec2 &atTarget, ActionDirection aDirection)
{
    CombActionThrowBackIn* ret = new CombActionThrowBackIn();
    ret->init(aTargetNode, atTarget, aDirection);
    ret->setBackDistance(40);
    ret->setBackDuration(0.15);
    ret->autorelease();
    ActionHelper::putNodeToSceneOutBorder(aTargetNode, atTarget, aDirection);
    return ret;
}

ActionInterval* CombActionThrowBackIn::_getAction()
{
    Vec2 vec = _targetPosition - _targetNode->getPosition();
    vec.normalize();
    vec *= _backDistance;
    Vec2 lTargetPos1 = _targetPosition + vec;
    auto aciton1 = Spawn::create(EaseBackOut::create(MoveTo::create(_actionDuration, _targetPosition)), nullptr);
    auto call = CallFunc::create(CC_CALLBACK_0(CombActionThrowBackIn::_actionDone, this));
    auto seq = Sequence::create(aciton1, call ,NULL);
    return seq;
}

CombActionMoveInBounce* CombActionMoveInBounce::create(Node* aTargetNode, const Vec2& atTarget, ActionDirection aDirection)
{
    CombActionMoveInBounce* ret = new CombActionMoveInBounce();
    ret->init(aTargetNode, atTarget, aDirection);
    ret->autorelease();
    ActionHelper::putNodeToSceneOutBorder(aTargetNode, atTarget, aDirection);
    return ret;
}

ActionInterval* CombActionMoveInBounce::_getAction()
{
    auto call = CallFunc::create(CC_CALLBACK_0(CombActionMoveInBounce::_actionDone, this));
    auto seq = Sequence::create(EaseBounceOut::create(_getMoveAction()), call ,NULL);
    return seq;
}

CombActionMoveInEaseExponential* CombActionMoveInEaseExponential::create(Node* aTargetNode, const Vec2& atTarget, ActionDirection aDirection)
{
    auto ret = new CombActionMoveInEaseExponential();
    ret->init(aTargetNode, atTarget, aDirection);
    ret->autorelease();
    ActionHelper::putNodeToSceneOutBorder(aTargetNode, atTarget, aDirection);
    return ret;
}

ActionInterval* CombActionMoveInEaseExponential::_getAction()
{
    auto call = CallFunc::create(CC_CALLBACK_0(CombActionMoveInEaseExponential::_actionDone, this));
    auto seq = Sequence::create(EaseExponentialOut::create(_getMoveAction()), call ,NULL);
    return seq;
}

CombActionMoveOutEaseExponential* CombActionMoveOutEaseExponential::create(cocos2d::Node *aTargetNode, ActionDirection aDirection)
{
    auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
    Vec2 topLeft =   rect.origin + Vec2(0,rect.size.height);
    Vec2 bottomRight = rect.origin + Vec2(rect.size.width,0);
    Vec2 curentPosition = aTargetNode->getPosition();
    Vec2 endPosiont;
    
    switch (aDirection) {
        case ActionDirection::kActionDirectionBottom:
            endPosiont = Vec2(curentPosition.x,bottomRight.y - aTargetNode->getContentSize().height*(1-aTargetNode->getAnchorPoint().y));
            break;
        case ActionDirection::kActionDirectionLeft:
            endPosiont = Vec2(topLeft.x - aTargetNode->getContentSize().width * (1- aTargetNode->getAnchorPoint().x), curentPosition.y);
            break;
        case ActionDirection::kActionDirectionRight:
            endPosiont = Vec2(bottomRight.x+ aTargetNode->getContentSize().width * aTargetNode->getAnchorPoint().x, curentPosition.y);
            break;
        case ActionDirection::kActionDirectionTop:
            endPosiont = Vec2(curentPosition.x, topLeft.y +aTargetNode->getContentSize().width*(aTargetNode->getAnchorPoint().y));
            break;
        default:
            break;
    }
    auto ret = new CombActionMoveOutEaseExponential();
    ret->init(aTargetNode, endPosiont, aDirection);
    ret->autorelease();
    return ret;
}

ActionInterval* CombActionMoveOutEaseExponential::_getAction()
{
    auto call = CallFunc::create(CC_CALLBACK_0(CombActionMoveOutEaseExponential::_actionDone, this));
    auto seq = Sequence::create(_getMoveAction(), call ,NULL);
    return seq;
}


CombActionJumpBy* CombActionJumpBy::create(Node* aTargetNode, float duration, const cocos2d::Vec2 &position, float height, int jumps)
{
    auto ret = new CombActionJumpBy();
    ret->init(aTargetNode, duration, position, height, jumps);
    ret->autorelease();
    return ret;
}

bool CombActionJumpBy::init(Node* aTargetNode, float duration, const cocos2d::Vec2 &jumpPosition, float height, int jumps)
{
    _duration = duration;
    _jumpPosition = jumpPosition;
    _height = height;
    _jumpTimes = jumps;
    setTargetNode(aTargetNode);
    return true;
}

ActionInterval* CombActionJumpBy::_getAction()
{
    auto call = CallFunc::create(CC_CALLBACK_0(CombActionJumpBy::_actionDone, this));
    auto seq = Sequence::create(JumpBy::create(_duration, _jumpPosition, _height, _jumpTimes), call ,NULL);
    return seq;
}

CombActionScaleBounce* CombActionScaleBounce::create(cocos2d::Node *aTargetNode, unsigned int times)
{
    auto ret = new CombActionScaleBounce();
    ret->init(aTargetNode, times);
    ret->autorelease();
    return ret;
}

bool CombActionScaleBounce::init(cocos2d::Node *aTargetNode, unsigned int times)
{
    _targetNode = aTargetNode;
    _repeatTimes = times;
    _bigFactor = 1.05;
    _smallFactor = 0.95;
    return true;
}

ActionInterval* CombActionScaleBounce::_getAction()
{
    auto big = ScaleTo::create(_actionDuration * 0.5, _bigFactor * _targetNode->getScale());
    auto small = ScaleTo::create(_actionDuration * 0.5, _smallFactor * _targetNode->getScale());
    auto normal = ScaleTo::create(_actionDuration * 0.25, _targetNode->getScale());

    auto call = CallFunc::create(CC_CALLBACK_0(CombActionScaleBounce::_actionDone, this));
    auto seq = Sequence::create(Repeat::create(Sequence::create(big, small, NULL), _repeatTimes), normal, call, nullptr);
    return seq;
}

NS_CombAction_END