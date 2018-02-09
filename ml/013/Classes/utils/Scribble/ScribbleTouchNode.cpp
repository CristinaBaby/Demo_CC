//
//  ScribbleTouchNode.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 9/12/14.
//
//

#include "ScribbleTouchNode.h"

ScribbleTouchNode *ScribbleTouchNode::create(Size pCanvasSize){
    ScribbleTouchNode *ret = new ScribbleTouchNode();
    if(ret && ret->init(pCanvasSize))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ScribbleTouchNode::init(Size pCanvasSize){
    if ( !ScribbleNode::init(pCanvasSize) ){
        return false;
    }
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(ScribbleTouchNode::touchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(ScribbleTouchNode::touchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(ScribbleTouchNode::touchEnded, this);
    
    this->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
    return true;
}

bool ScribbleTouchNode::touchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    if (this->isEnabled()) {
        Point lTouchPoint = pTouch->getLocation();
        this->paint(lTouchPoint);
        return true;
    }
    
    return false;
}

void ScribbleTouchNode::touchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    Point lTouchPoint = pTouch->getLocation();
    Point lPreTouchPoint = pTouch->getPreviousLocation();
    this->paint(lTouchPoint, lPreTouchPoint);
}

void ScribbleTouchNode::touchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    
}