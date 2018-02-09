//
//  CBGameLoadingLayer.cpp
//  ColorBook
//
//  Created by maxiang on 5/15/15.
//
//

#include "CBGameLoadingLayer.h"

USING_NS_CC;
CB_BEGIN_NAMESPACE

bool GameLoadingLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 200))) {
        return false;
    }
    
    bool rcode = false;
    do {
        
        //touch event listen
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [](Touch* touch, Event* event) {
     
            return true;
        };
        
        touchEventListener->onTouchMoved = [](Touch* touch, Event* event) {
            
        };
        
        touchEventListener->onTouchEnded = [](Touch* touch, Event* event) {
            
        };
        
        touchEventListener->onTouchCancelled = [](Touch* touch, Event* event) {
            
        };
        
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        rcode = true;
    } while (0);
    
    return rcode;
}


CB_END_NAMESPACE