//
//  ScribbleWithCallBack.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 9/12/14.
//
//

#include "ScribbleWithCallBack.h"

ScribbleWithCallBack *ScribbleWithCallBack::create(Size pCanvasSize){
    ScribbleWithCallBack *ret = new ScribbleWithCallBack();
    if(ret && ret->init(pCanvasSize)){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ScribbleWithCallBack::ScribbleWithCallBack(){
    _enableTouchBegan = true;
    _enableTouchMoved = true;
    _enableTouchEnded = true;
    callBack = nullptr;
}

ScribbleWithCallBack::~ScribbleWithCallBack(){
    this->getEventDispatcher()->removeEventListener(_touchListener);
    _touchListener->release();
}

bool ScribbleWithCallBack::init(Size pCanvasSize){
    if ( !ScribbleNode::init(pCanvasSize) ){
        return false;
    }
    
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->retain();
    _touchListener->setSwallowTouches(true);
    _touchListener->onTouchBegan = CC_CALLBACK_2(ScribbleWithCallBack::touchBegan, this);
    _touchListener->onTouchMoved = CC_CALLBACK_2(ScribbleWithCallBack::touchMoved, this);
    _touchListener->onTouchEnded = CC_CALLBACK_2(ScribbleWithCallBack::touchEnded, this);
    
    this->getEventDispatcher()->addEventListenerWithFixedPriority(_touchListener, 1);
    return true;
}

bool ScribbleWithCallBack::touchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    if (this->isEnabled()) {
        if (_enableTouchBegan) {
            Point lTouchPoint = pTouch->getLocation();
            this->paint(lTouchPoint);
            if(callBack != nullptr)
                callBack(pTouch,TouchType::eTouchBegan);
        }
        return true;
    }
    
    return false;
}

void ScribbleWithCallBack::touchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    if (_enableTouchMoved) {
        Point lTouchPoint = pTouch->getLocation();
        Point lPreTouchPoint = pTouch->getPreviousLocation();
        this->paint(lTouchPoint, lPreTouchPoint);
        if(callBack != nullptr)
            callBack(pTouch,TouchType::eTouchMoved);
    }
}

void ScribbleWithCallBack::touchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    if (_enableTouchEnded) {
        Point lTouchPoint = pTouch->getLocation();
        this->paint(lTouchPoint);
        if(callBack != nullptr)
            callBack(pTouch,TouchType::eTouchEnded);
    }
}

