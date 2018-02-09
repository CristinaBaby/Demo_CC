//
//  ScribbleArmatureNode.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 14-10-29.
//
//

#include "ScribbleArmatureNode.h"

ScribbleArmatureNode *ScribbleArmatureNode::create(Size pCanvasSize){
    ScribbleArmatureNode *ret = new ScribbleArmatureNode();
    if(ret && ret->init(pCanvasSize,0))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ScribbleArmatureNode::ScribbleArmatureNode() : _armaturePosition(0,0){
}

bool ScribbleArmatureNode::touchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    if (this->isEnabled()) {
        if (_enableTouchBegan) {
            Point lTouchPoint = pTouch->getLocation();
            cocos2d::log("lTouchPoint %f %f", lTouchPoint.x, lTouchPoint.y);
            lTouchPoint.subtract(_armaturePosition);
            this->paint(lTouchPoint);
        }
        return true;
    }
    
    return false;
}

void ScribbleArmatureNode::touchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    if (_enableTouchMoved) {
        Point lTouchPoint = pTouch->getLocation();
        Point lPreTouchPoint = pTouch->getPreviousLocation();
        lTouchPoint.subtract(_armaturePosition);
        lPreTouchPoint.subtract(_armaturePosition);
        this->paint(lTouchPoint, lPreTouchPoint);
    }
}

void ScribbleArmatureNode::touchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    if (_enableTouchEnded) {
        Point lTouchPoint = pTouch->getLocation();
        lTouchPoint.subtract(_armaturePosition);
        this->paint(lTouchPoint);
    }
}