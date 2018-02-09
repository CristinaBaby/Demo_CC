//
//  WriteTouchNode.cpp
//  WriteDemo_3_2
//
//  Created by renhong on 9/19/14.
//
//

#include "WriteTouchNode.h"

WriteTouchNode *WriteTouchNode::create(Size pCanvasSize){
    WriteTouchNode *pRet = new WriteTouchNode();
    if (pRet && pRet->init(pCanvasSize)){
        pRet->autorelease();
        return pRet;
    }
    else{
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool WriteTouchNode::init(Size pCanvasSize){
    if ( !WriteNode::init(pCanvasSize) ){
        return false;
    }
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(WriteTouchNode::touchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(WriteTouchNode::touchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(WriteTouchNode::touchEnded, this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

bool WriteTouchNode::touchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    Point lTouchPoint = pTouch->getLocation();
    
    
     WriteNode::touchBegan(lTouchPoint);
   
    return true;
}

void WriteTouchNode::touchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    Point lTouchPoint = pTouch->getLocation();
    Point lPreTouchPoint = pTouch->getPreviousLocation();
    
    WriteNode::touchMoved(lTouchPoint, lPreTouchPoint);
}

void WriteTouchNode::touchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    Point lTouchPoint = pTouch->getLocation();
    WriteNode::touchEnded(lTouchPoint);
}