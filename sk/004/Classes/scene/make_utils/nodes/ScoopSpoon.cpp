
//
//  ScoopSpoon.cpp
//  bhug1071
//
//  Created by maxiang on 9/7/16.
//
//

#include "ScoopSpoon.h"
#include "ComponentTouchMoveToTarget.h"

ScoopSpoon::~ScoopSpoon()
{
    
}

ScoopSpoon* ScoopSpoon::create(const std::string& spoon)
{
    ScoopSpoon *pRet = new ScoopSpoon();
    if (pRet && pRet->init(spoon))
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

bool ScoopSpoon::init(const std::string& spoon)
{
    if (!MakeBaseNode::init())
        return false;
    
    auto spoonSprite = Sprite::create(spoon);
    addChild(spoonSprite, 1);
    
    _mainContent = spoonSprite;
    return true;
}

Sprite* ScoopSpoon::setContent(const std::string& content, const Vec2& position)
{
    _content = Sprite::create(content);
    addChild(_content, 2);
 
    _content->setOpacity(0);
    _content->setPosition(position);
    
    return _content;
}

void ScoopSpoon::setMoveContentOffset(const Vec2& offset)
{
    _contentMoveOffset = offset;
}

void ScoopSpoon::startDragToWorldPositon(const Vec2& worldPoint)
{
    _backPosition = this->getPosition();
    this->removeAllComponents();
    
    auto moveComp = ComponentTouchMoveToTarget::create(worldPoint);
    moveComp->setName("moveComp");
    moveComp->setMaxDistanceForMoveToTarget(200);
    this->addComponent(moveComp);
    moveComp->start();
    moveComp->componentTouchMoveToTargetDoneCall = [=]()
    {
        ActionInterval *scoopAction = DelayTime::create(0.0);
        if (_enableScoopAnimation)
            scoopAction = Spawn::create(MoveBy::create(0.4, Vec2(80.0, 80.0)), RotateBy::create(0.4, -20.0), NULL);
        
        this->runAction(Sequence::create(scoopAction, scoopAction->reverse(), CallFunc::create([=](){
            
            _content->runAction(FadeIn::create(0.3));
            if (onScroopedCallback)
                onScroopedCallback();
        }), DelayTime::create(0.5), CallFunc::create([=](){
            
            this->removeComponent(moveComp);
            
            auto moveComp = ComponentTouchMoveToTarget::create(_backPosition);
            moveComp->setMaxDistanceForMoveToTarget(200);
            this->addComponent(moveComp);
            moveComp->setName("moveComp");
            moveComp->start();
            moveComp->componentTouchMoveToTargetDoneCall = [=]()
            {
                ActionInterval *pourAction = DelayTime::create(0.0);
                if (_enablePourAnimation)
                    pourAction = RotateBy::create(0.3, _rotateAngel);
                
                this->runAction(Sequence::create(pourAction, CallFunc::create([=](){

                    ActionInterval *moveAction = DelayTime::create(0.0);
                    if (_enableContentMoveAnimation)
                    {
                        moveAction = MoveBy::create(0.3, _contentMoveOffset);
                    }
      
                    _content->runAction(Sequence::create(Spawn::create(moveAction, FadeOut::create(0.3), NULL), CallFunc::create([=](){
                        
                        if (onFinishPourCallback)
                            onFinishPourCallback();
                    }), nullptr));
                }), DelayTime::create(1.0),
                    pourAction->reverse(),
                    MoveTo::create(0.3, _backPosition),
                    CallFunc::create([=](){
                    
                    _content->setPosition(_content->getPosition() + -_contentMoveOffset);
                    
                    if (backToOriginalPositionCallback)
                        backToOriginalPositionCallback();
                }), nullptr));
            };
        }), NULL));
    };
}







