//
//  Sugar.cpp
//  TOYBM0205
//
//  Created by huangwen on 15/10/23.
//
//

#include "Sugar.hpp"

Sugar* Sugar::create(const std::string filename){
    Sugar* sugar = new (std::nothrow)Sugar();
    if (sugar && sugar->initWithFile(filename)) {
        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(Sugar::onTouchBegan, sugar);
        listener->setSwallowTouches(true);
        sugar->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, sugar);
        sugar->autorelease();
        return sugar;
    }
    CC_SAFE_DELETE(sugar);
    return nullptr;
}

bool Sugar::onTouchBegan(Touch* touch, Event* event){
    return false;

    auto target = (Sprite*)event->getCurrentTarget();
    auto point = touch->getLocation();
    if (target->getBoundingBox().containsPoint(point)) {
        
        //animation
        // music
        target->stopAllActions();
        scheduleOnce(schedule_selector(Sugar::destroy), 0.2f);
        return true;
    }
    return false;
}

void Sugar::destroy(float dt){
    removeFromParent();
}