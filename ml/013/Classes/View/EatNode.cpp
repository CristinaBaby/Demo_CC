//
//  EatNode.cpp
//  Make
//
//  Created by QAMAC01 on 15-2-12.
//
//

#include "EatNode.h"


bool EatNode::init()
{
    if (!ClippingNode::init())
    {
        return false;
    }
    
    stencil_ = Node::create();
    this -> setStencil(stencil_);
    this -> setInverted(true);//设置底板可见
    this -> setAlphaThreshold(0);//设置绘制底板的Alpha值为0

    listener  = EventListenerTouchOneByOne::create();
    listener -> setSwallowTouches(false);
    
    listener -> onTouchBegan = CC_CALLBACK_2(EatNode::onTouchBegan, this);
    listener -> onTouchMoved = CC_CALLBACK_2(EatNode::onTouchMoved, this);
    listener -> onTouchEnded = CC_CALLBACK_2(EatNode::onTouchEnded, this);

    listener -> retain();
    return true;
}

#pragma mark Touch

bool EatNode::onTouchBegan(Touch* touch, Event* event)
{
    auto target = static_cast<Node*>(event->getCurrentTarget());//获取的当前触摸的目标
    
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    if (rect.containsPoint(locationInNode))
    {
        log("%f,%f",locationInNode.x,locationInNode.y);
        
        return true;
    }
    return true;
}

void EatNode::onTouchMoved(Touch* touch, Event* event)
{
    
    
};


void EatNode::onTouchEnded(Touch* touch, Event* event)
{
    
    auto target = static_cast<Node*>(event->getCurrentTarget());//获取的当前触摸的目标
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());

    auto eat =  Sprite::create("start/eat_mask.png");
    eat -> setPosition(locationInNode);
    stencil_ -> addChild(eat);
    
    CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/eat.mp3");

}

void EatNode::allowEat()
{
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener -> clone(), this);
}

void EatNode::forbidEat()
{
    _eventDispatcher -> removeEventListenersForTarget(this);
}


void EatNode::reset()
{
    if (stencil_)
    {
        stencil_ -> removeAllChildren();
    }
}